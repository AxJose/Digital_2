/*
 * ADC.h
 *
 * Created: 2/03/2025 12:11:18
 *  Author: Adrián Pascual y Victor Vanegas
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdlib.h>

// Función para inicializar el ADC
void ADC_init(void);

// Función para leer un canal ADC 
uint8_t ADC_read(uint8_t canal);


#endif /* ADC_H_ */