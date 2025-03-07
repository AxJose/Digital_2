/*
 * HCSR04.c
 *
 * Created: 2/03/2025 18:06:59
 *  Author: adria
 */ 


#include "HCSR04.h"

#define TRIG_PIN1 PB0  // TRIG del Sensor 1
#define ECHO_PIN1 PB2  // ECHO del Sensor 1

#define TRIG_PIN2 PB3  // TRIG del Sensor 2
#define ECHO_PIN2 PB4  // ECHO del Sensor 2

void Ultrasonic_Init(void) {
	 
	 DDRB |= (1 << TRIG_PIN1) | (1 << TRIG_PIN2);  // TRIG1 y TRIG2 como salida
	 DDRB &= ~((1 << ECHO_PIN1) | (1 << ECHO_PIN2)); // ECHO1 y ECHO2 como entrada
}

uint8_t Ultrasonic_Read(uint8_t trigPin, uint8_t echoPin) {
	
	uint16_t tiempo;

	// Generar pulso de 10us en TRIG
	PORTB &= ~(1 << trigPin);
	_delay_us(2);
	PORTB |= (1 << trigPin);
	_delay_us(10);
	PORTB &= ~(1 << trigPin);

	// Esperar a que ECHO se active
	while (!(PINB & (1 << echoPin)));

	// Medir tiempo mientras ECHO está en alto
	tiempo = 0;
	while (PINB & (1 << echoPin)) {
		tiempo++;
		_delay_us(1);
	}

	// Convertir tiempo a distancia en cm (8 bits)
	uint8_t distancia = (tiempo * 343) / 20000;
	return distancia;
}