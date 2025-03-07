/*
 * PWM0.c
 *
 * Created: 25/02/2025 17:12:49
 *  Author: Adrián Pascual y Victor Vanegas
 */ 

#include "PWM0.h"
#include <avr/interrupt.h>

void initPWM0FastB(uint8_t inverted, uint16_t prescaler){
	// Configuración del pin PB1 como salida (OC1A)
	DDRB |= (1 << DDB1);
		
	if(inverted){
		// Configurando OC1A como invertido
		TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
		
		}else{
		// Configurando OC1A como no invertido
		TCCR1A |= (1 << COM1A1);
		
	}
	
	// Configurando Modo PWM FAST TOP = ICR1
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	TCCR1A |= (1 << WGM11) | (0 << WGM10);
	
	ICR1 = 20000;  // TOP = 20000 ? Periodo de 20ms (50Hz)
	
	if(prescaler == 8){
		// Prescaler de 8
		TCCR1B |= (1 << CS11); 

	}
}

void updateDutyCycleB(uint16_t duty2){
	// duty será el ángulo que se ingrese
	
	if (duty2 == 0) {
		OCR1A = 2300; 
		
	} else if (duty2 == 90) {
		OCR1A = 2800;
	}else if (duty2 == 180){
		OCR1A = 3300;
	}
		  
	
}