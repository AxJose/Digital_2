/*
 * PWM0.h
 *
 * Created: 25/02/2025 17:13:08
 *  Author: Adri�n Pascual y Victor Vanegas
 */ 

#ifndef PWM0_H_
#define PWM0_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define invertido 1
#define no_invertido 0

// Funcion para configurar PWM0 Modo Fast Canal B
void initPWM0FastB(uint8_t inverted, uint16_t prescaler);

void updateDutyCycleB(uint8_t duty2);

#endif /* PWM0_H_ */