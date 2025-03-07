/*
 * main.c
 *
 * Created: 2/25/2025 4:40:55 PM
  *  Author: Adrián Pascual - 22045 y Victor Vanegas - 22839
  * MASTER
  */

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C/I2C.h"
#include "LCD/LCD.h"
#include "UART/UART.h"
//#include "TCS3472/TCS3472.h"

#define slave1 0x30
#define slave2 0x40
#define TCS3472_ADDR 0x29 // Dirección I2C del sensor TCS3472

uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;
uint8_t valorI2C = 0;
uint8_t contador = 0;
uint8_t contador2 = 0;
char colorDetectado = 'N';
char temp_color = 'N';

uint8_t valor = 0;
uint8_t adaf = 0;

void initPorts(void);
void TCS3472_Init(void); 
uint16_t readColorData(uint8_t reg);

int main(void)
{
	I2C_Master_Init(100000, 1); // Inicializar como Master Fscl 100kHz, prescaler 1
	UART_Init(115200); // Inicializar UART con baudrate 9600
		
	initLCD8bits();
	LCD_CMD(0x01);
	LCD_CMD(0x06);
	_delay_ms(100);
	
	char buffer[16];
	
	// Mostrar en la LCD
	LCD_Set_Cursor(1, 1);
	LCD_Write_String("S1:");
	LCD_Set_Cursor(7, 1);
	LCD_Write_String("R/A:");
	LCD_Set_Cursor(14, 1);
	LCD_Write_String("S3:");
	
	TCS3472_Init(); // Inicializar sensor de color
	
    while(1)
    {
		// adaf = UART_Read();
		if (adaf == 1){
			valor = 1;
		}
		// UART_Write(adaf);
	
     /*********** ESCLAVO 1 ************/
		PORTB |= (1 << PORTB5);
		
		I2C_Master_Start();
		// Escritura
		bufferI2C = slave1 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if (temp != 1){
			I2C_Master_Stop();
		}else{
			if (adaf == 2){
				I2C_Master_Write('S'); // Controlar desde adafruit
			}else{
				I2C_Master_Write('R'); // Por default
			}
			
			I2C_Master_Stop();
		}
		_delay_ms(500);
		
		PORTB &= ~(1 << PORTB5);
		I2C_Master_Start();
		// Lectura
		bufferI2C = slave1 << 1 | 0b00000001;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}else{
		
		TWCR = (1 << TWINT) | (1 << TWEN);
		while(!(TWCR & (1 << TWINT)));
		
		valorI2C = TWDR; // Distancia que envía el esclavo 1
		
		I2C_Master_Stop();
		}
		
		// Mostrar a la distancia que se encuentra

		sprintf(buffer, "%d", valorI2C); // Envia 1 o 0 
		UART_Write_Text("\r\n");
		LCD_Set_Cursor(14,2);
		LCD_Write_String(buffer);
		//UART_Write_Text(buffer); // Mandarlo a la terminal para que lo lea el ESP
		//UART_Write_Text("\r\n");
		
	/*********** ESCLAVO 2 ***********/
		
		
		I2C_Master_Start();
		// Escritura
		bufferI2C = slave2 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if (temp != 1){
			I2C_Master_Stop();
			}else{
			UART_Write_Text("\r\n");
			
			/*** Lectura de valores RGB ***/
			uint16_t red = readColorData(0x16);
			uint16_t green = readColorData(0x18);
			uint16_t blue = readColorData(0x1A);
			
			I2C_Master_Start();
			// Determinar el color del objeto
			
			temp = I2C_Master_Write(bufferI2C);
			
			if ((adaf >= 4) & (valor == 1)){
				valor = adaf;	
				PORTB |= (1 << PORTB5);
				
			}else if (adaf < 1){
				valor = 0;
			}
			
			if ((valor == 4) | (valor == 6) | (valor == 8)){
				if (valor == 4){
					I2C_Master_Write('R');	// Enviar R para colocar el servo en 0º
					valor = 0;
					
					}else if (adaf == 6){
					I2C_Master_Write('N');	// Enviar R para colocar el servo en 90º
					valor = 0;
					
					}else if (adaf == 8){
					I2C_Master_Write('B');	// Enviar R para colocar el servo en 180º
					valor = 0;
					
					}
			}else {	
			
			if (red > 150 && red > green && red > blue) {
				colorDetectado = 'R'; // Rojo
				temp_color = 'R'; // Asignarlo a la variable temporal
				LCD_Set_Cursor(1, 2);
				LCD_Write_Char(colorDetectado);
				//if (adaf == 0){UART_Write('R');}
				I2C_Master_Write('R');	// Enviar R porque se detectó colo rojo
				_delay_ms(10);
				
				} else if (blue > 150 && blue > red && blue > green) {
				colorDetectado = 'B'; // Azul
				temp_color = 'B'; // Asignarlo a la variable temporal
				LCD_Set_Cursor(1, 2);
				LCD_Write_Char(colorDetectado);
				//if (adaf == 0){UART_Write('B');}
				I2C_Master_Write('B');	// Enviar B porque se detectó colo azul (Blue)
				_delay_ms(10);
				
				} else {
				colorDetectado = 'N'; // Ninguno
				LCD_Set_Cursor(1, 2);
				LCD_Write_Char(colorDetectado);
				//if (adaf == 0){UART_Write('N');}
				I2C_Master_Write(colorDetectado); // Enviar N porque no se detectó ni azul ni rojo
				_delay_ms(10);
			}
		
			

			_delay_ms(500);
		
			I2C_Master_Stop();
		}
		_delay_ms(500);
		
		PORTB &= ~(1 << PORTB5);
		I2C_Master_Start();
		
		// Lectura
		bufferI2C = slave2 << 1 | 0b00000001;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			}else{
			
			TWCR = (1 << TWINT) | (1 << TWEN);
			while(!(TWCR & (1 << TWINT)));
			
			valorI2C = TWDR; // Se recibe el valor de cualquiera de los sensores ultrasónicos
			
			if (temp_color == 'R'){ 
				if (valorI2C <= 0){
					contador ++;	// Si previamente se detectó color rojo, aumentar el contador de ese color
					if (contador == 5){
						contador = 0;
					}
				}
				temp_color = 'N';	// Limpiar la variable temporal
			}else if (temp_color == 'B'){
				if (valorI2C <= 0){
					contador2 ++;	// Si previamente se detectó color azul, aumentar el contador de ese color
					if (contador2 == 5){
						contador2 = 0;
					}
				}
				temp_color = 'N';	// Limpiar la variable temporal
			}
			
			
			I2C_Master_Stop();
		}
	}
	// Mostrar en la LCD
		
		// Tal vez podria hacer valorI2C + 2, Para usar 2 y 3 en lugar de 0 y 1
		
		// Mostrar en la terminal el valor del sensor ultrasónico
		sprintf(buffer, "%d", valorI2C);
		//UART_Write_Text(buffer);  // Envia 1 o 0
		UART_Write_Text("\r\n");
		
		// Mostrar en el LCD el contador de los rojos
		LCD_Set_Cursor(7, 2);
		sprintf(buffer, "%d", contador);
		LCD_Write_String(buffer);
		
		// Mostrar en el LCD el contador de los azules
		LCD_Set_Cursor(9, 2);
		sprintf(buffer, "%d", contador2);
		LCD_Write_String(buffer);
		
	}
}


/************ Inicialización del Sensor TCS3472 ************/
void TCS3472_Init(void){
	I2C_Master_Start();
	I2C_Master_Write(TCS3472_ADDR << 1); // Dirección del sensor
	I2C_Master_Write(0x80); // Registro ENABLE
	I2C_Master_Write(0x03); // Activar el sensor y ADC
	I2C_Master_Stop();
}

/************ Función para Leer Datos de Color ************/
uint16_t readColorData(uint8_t reg)
{
	uint8_t low, high;

	// Seleccionar el registro del color
	I2C_Master_Start();
	I2C_Master_Write(TCS3472_ADDR << 1); // Dirección del sensor (escritura)
	I2C_Master_Write(0x80 | reg); // Registro del color
	I2C_Master_Stop();

	// Leer el valor de 16 bits (LSB + MSB)
	I2C_Master_Start();
	I2C_Master_Write((TCS3472_ADDR << 1) | 1); // Dirección del sensor (lectura)
	low = I2C_Master_Read_Ack();  // Leer LSB
	high = I2C_Master_Read_Nack(); // Leer MSB
	I2C_Master_Stop();

	return (high << 8) | low;
}
