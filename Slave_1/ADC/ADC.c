/*
 * ADC.c
 *
 * Created: 2/03/2025 12:11:12
 *  Author: Adrián Pascual y Victor Vanegas
 */ 

#define F_CPU 16000000UL
#include "ADC.h"
#include <util/delay.h>

void ADC_init(void) {
	// Configuracion de los pines PC0 como entrada
	DDRC &= ~(1 << PORTC0) ;
	ADMUX = (1 << REFS0); // Voltaje de referencia AVcc (5V)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC y preescaler de 128 (125kHz)
}

uint8_t ADC_read(uint8_t canal) {
	
	ADMUX = (ADMUX & 0xF8) | (canal & 0x07); // Seleccionar canal
	ADCSRA |= (1 << ADSC); // Iniciar conversión
	while (ADCSRA & (1 << ADSC)); // Esperar que termine la conversión
	
	return ADC;
}