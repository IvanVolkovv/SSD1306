/*!
 * @file:    		i2c_init.c
 * @brief:     	API for I2C
 * @version:  	V1.0.0
 * @date:    		00.00.0000
 * @attention:	
 * The file contains functions for configuring and managing the module.
 * As an example, a simple way to configure and manage the module is shown here.
 * Pay attention to infinite loops (while - continue)! You should avoid using infinite loops (use timeout for module response) otherwise any error will crash the program.
 * Improve the code for your projects!
 *
 */

#ifndef __I2C_INIT_H
#define __I2C_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

// for direction
#define READ_DATA		1U
#define WRITE_DATA	0

/* --- prototypes --- */ 
void init_I2C2(void);
int8_t I2C_SendStartBit(I2C_TypeDef*);
int8_t I2C_SendStopBit(I2C_TypeDef*); 
int8_t I2C_SendAddr(I2C_TypeDef*, uint8_t, uint8_t); 
int8_t I2C_ReadData(I2C_TypeDef*, uint8_t, uint8_t, uint8_t*, uint8_t); 
int8_t I2C_WriteBytes(I2C_TypeDef*, uint8_t, uint8_t*, uint8_t); 
int8_t I2C_WriteByte(I2C_TypeDef*, uint8_t, uint8_t); 

void sendCommand(I2C_TypeDef*, uint8_t, uint8_t*, uint8_t); 
void sendData(I2C_TypeDef*, uint8_t, uint8_t*, uint32_t); 

int8_t scanAddrDevice(I2C_TypeDef *i2c, uint8_t addr); 


#ifdef __cplusplus
}
#endif

#endif /* __I2C_INIT_H */
