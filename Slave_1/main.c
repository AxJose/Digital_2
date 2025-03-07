/*
 * Slave_1.c
 *
 * Author: Adrián Pascual y Victor Vanegas 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"
#include "ADC/ADC.h"
#include "PWM0/PWM0.h"
#include "UART/UART.h"

#define SlaveAddress 0x30

volatile uint8_t contador = 0;
volatile uint16_t valorADC = 0;
volatile uint8_t distancia = 0;
uint8_t buffer = 0;

//*********************************************************************************
// Prototipos de funcion
//*********************************************************************************
void Ports_init(void);

//*********************************************************************************
// Codigo principal
//*********************************************************************************
int main(void){
	
	Ports_init();
	ADC_init();
	initPWM0FastB(no_invertido, 1024);
	UART_Init(9600);
	
	DDRB |= (1<<DDB5);
	DDRD |= (1<<DDD2);
	DDRD |= (1<<DDD3);
	
	PORTD &= ~(1 << PIND2);
	PORTD &= ~(1 << PIND3);
	PORTB &= ~(1 << PINB5);
	
	I2C_Slave_Init(SlaveAddress);
	
    while (1) 
    {
		// Código para leer el sensor de distancia
		if(buffer == 'R'){
			
			valorADC = ADC_read(0);
			
			valorADC = (valorADC * 35/255);
			_delay_ms(5);
			
			char bufer[10];
			sprintf(bufer, "%d", valorADC);  // Convierte el número a texto
			UART_Write_Text(bufer);
			UART_Write_Text("\r\n"); 
			
			if (valorADC > 15) {  
				
				PORTB &= ~(1 << PINB5);
				distancia = 0;
				
				// Detener motor
				PORTD &= ~(1 << PIND2); // Conectar al IN4 del puente H
				PORTD &= ~(1 << PIND3); // Conectar al IN3 del puente H
			
				buffer = 0;         // Reiniciar el buffer
			
			}else{
				// Si ADC < 15, entonces hay objeto y se activa el motor
				distancia = 1;
				
				PORTB |= (1 << PINB5); 
				
				// Encender motor
				PORTD |= (1 << PIND2); // Conectar al IN4 del puente H
				PORTD &= ~(1 << PIND3); // Conectar al IN3 del puente H
				
				updateDutyCycleB(43); // Modificar la velocidad del DC
				
				_delay_ms(5000); // Agregar tiempo para que termine de pasar el objeto
			}
		}else if (buffer == 'S'){
			
			// Encender motor
			PORTD |= (1 << PIND2); // Conectar al IN4 del puente H
			PORTD &= ~(1 << PIND3); // Conectar al IN3 del puente H
			
			updateDutyCycleB(30); // Modificar la velocidad del DC
			
		}
	}
}

//*********************************************************************************
// Sub Rutinas
//*********************************************************************************
void Ports_init(void){
	// Marcar como entrada el puerto B
	DDRB |= (1<< PORTB0)|(1<< PORTB1)|(1<< PORTB2)|(1<< PORTB3)|(1<< PORTB4);
	// Los ponemos el puerto en 0
	PORTB = 0x00;
	// Marcar como salida el puerto C
	DDRC &= ~(1<< PORTC0)|(1<< PORTC1);
	// Ponemos los botones como pullups
	PORTC |= (1<<DDC0)|(1<<DDC1);
	// Habilitar interrupcion PCIE0
	PCICR |= (1<<PCIE1);
	// Seleccionamos el PC0 y PC1 que sea la interrucion
	PCMSK1 |= (1<<PCINT8)|(1<<PCINT9);
	
	// Activar las interrupciones
	sei();
}

//*********************************************************************************
// ISR
//*********************************************************************************

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xFC;
	switch(estado){
		case 0x60:
		case 0x70:
		TWCR |= (1<< TWINT);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;
		TWCR |= (1<< TWINT); // Se limpia la bandera
		break;
		case 0xA8:
		case 0xB8:
		TWDR = distancia; // Cargar el dato
		TWCR = (1 << TWEN)|(1 << TWIE)|(1 << TWINT)|(1 << TWEA); // Inicia el envío
		break;
		default: // Se libera el bus de cualquier error
		TWCR |= (1 << TWINT)|(1 << TWSTO);
		break;
	}
}