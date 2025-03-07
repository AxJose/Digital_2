/*
 * PWM0.c
 *
 * Created: 25/02/2025 17:12:49
 *  Author: Adrián Pascual y Victor Vanegas
 */ 

#include "PWM0.h"
#include <avr/interrupt.h>

void initPWM0FastB(uint8_t inverted, uint16_t prescaler){
	// Configuración del pin PD5 como salida (OC0B)
	DDRD |= (1 << DDD5);

	
	if(inverted){
		// Configurando OC0B como invertido
		TCCR0A |= (1 << COM0B1) | (1 << COM0B0);
		
		}else{
		// Configurando OC0B como no invertido
		TCCR0A |= (1 << COM0B1);
	}
	
	// Configurando Modo PWM FAST TOP = 0xFF
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	
	if (prescaler == 1024){
		// Prescaler de 1024
		TCCR0B |= (1 << CS02) | (1 << CS00);
	}
}

void updateDutyCycleB(uint8_t duty){
	// duty será el ángulo que se ingrese
	
	uint8_t OCR0B_value;
	OCR0B_value = (duty * 5) / 9 + 25; // Mapeo de 0° -> 25, 180° -> 75
	
	// Actualizamos el ciclo de trabajo del PWM
	OCR0B = OCR0B_value;
}