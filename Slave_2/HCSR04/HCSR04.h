/*
 * HCSR04.h
 *
 * Created: 2/03/2025 18:07:08
 *  Author: adria
 */ 

#define F_CPU 16000000

#ifndef HCSR04_H_
#define HCSR04_H_

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void Ultrasonic_Init(void);

uint8_t Ultrasonic_Read(uint8_t pin, uint8_t pin2);



#endif /* HCSR04_H_ */