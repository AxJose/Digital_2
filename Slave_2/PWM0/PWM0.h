/*
 * PWM0.h
 *
 * Created: 25/02/2025 17:13:08
 *  Author: Adrián Pascual y Victor Vanegas
 */ 

#define F_CPU 16000000

#ifndef PWM0_H_
#define PWM0_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define invertido 1
#define no_invertido 0

// Funcion para configurar PWM0 Modo Fast Canal B
void initPWM0FastB(uint8_t inverted, uint16_t prescaler);

void updateDutyCycleB(uint16_t duty);

#endif /* PWM0_H_ */