/*
 * I2C.h
 *
 * Created: 25/02/2025 16:46:22
 *  Author: Adrián Pascual y Victor Vanegas
 */ 

#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdint.h>

// Inicializar I2C Maestro
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);

// Iniciar comunicaci?n I2C
void I2C_Master_Start(void);

// Parada de la comunicacion I2C
void I2C_Master_Stop(void);

// Funcion de transmisi?n de datos del maestro al esclavo
// Esta funci?n dedvolvera un 0 si el esclavo ha recibido el dato
uint8_t I2C_Master_Write(uint8_t dato);

// Funcion de recepcion de datos enviados por el esclavo al maestro
// Esta funcion es para leer los datos que estan en el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);

// Funcion para inicializar I2C esclavo
void I2C_Slave_Init(uint8_t address);

uint8_t I2C_Master_Read_Ack(void);
uint8_t I2C_Master_Read_Nack(void);

#endif /* I2C_H_ */