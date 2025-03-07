/*
 * ADC.h
 *
 * Created: 2/03/2025 12:11:18
 *  Author: Adri�n Pascual y Victor Vanegas
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdlib.h>

// Funci�n para inicializar el ADC
void ADC_init(void);

// Funci�n para leer un canal ADC 
uint8_t ADC_read(uint8_t canal);


#endif /* ADC_H_ */