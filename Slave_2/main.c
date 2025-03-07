/*
 * main.c
 *
 * Created: 3/1/2025 1:22:01 PM
 *  Author: Adrián Pascual y Victor Vanegas
 */ 


#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"
#include "PWM0/PWM0.h"
#include "HCSR04/HCSR04.h"

#define TRIG_PIN1 PB0  // TRIG del Sensor 1
#define ECHO_PIN1 PB2  // ECHO del Sensor 1

#define TRIG_PIN2 PB3  // TRIG del Sensor 2
#define ECHO_PIN2 PB4  // ECHO del Sensor 2

#define SlaveAddress 0x40

uint8_t buffer = 0;
uint8_t distancia = 2;


int main(void)
{
	DDRB |= (1<<DDB5);
	DDRD |= (1<<DDD2);
	PORTB &= ~(1<<PINB5);
	PORTD &= ~(1<<PIND2);
	
	I2C_Slave_Init(SlaveAddress);
	initPWM0FastB(no_invertido, 8);
	Ultrasonic_Init();
	
	sei();	//Habilitar interrupciones
	
    while (1) 
    {
			
		// Código para controlar el servo dependiendo del sensor de color I2C
 
		if(buffer == 'R'){
			updateDutyCycleB(0);  // Rojo -> 0°
			_delay_ms(3000);  // Agregar delay para esperar a que termine de pasar la caja
			
		}else if (buffer == 'B'){
			updateDutyCycleB(180);  // Azul -> 180°
			_delay_ms(3000);  // Agregar delay para esperar a que termine de pasar la caja
			
		}else{
			updateDutyCycleB(90);  // Ninguno -> 0°
		
		}
		
	// Código para leer los sensores ultrasónicos
		
		uint8_t dis_1 = Ultrasonic_Read(TRIG_PIN1, ECHO_PIN1); // Leer el primer sensor
		uint8_t dis_2 = Ultrasonic_Read(TRIG_PIN2, ECHO_PIN2); // Leer el segundo sensor
		
		if ((dis_1 <= 1) | (dis_2 > 1)){
			distancia = 0;	// Si ninguno de los dos cambia, dejarlo en 0
			PORTB &= ~(1<<PINB5);
			PORTD &= ~(1<<PIND2);
			
		}else if (dis_2 <= 1){
			distancia = 1; // Si la variable dis_2 cambia, asignarselo a distancia
			PORTB |= (1 << PINB5);
			
		}else if(dis_1 > 1){
			distancia = 1; // Si la variable dis_1 cambia, asignarselo a distancia
			PORTD |= (1 << PIND2);	
		}
	}
}

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
