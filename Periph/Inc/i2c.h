/*
 * Filename: i2c.h
 * Description: The file contains I2C definitions.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 11:05:17 PM
 * Author : Dmitry Slobodchikov
 */
#ifndef I2C_H_
#define I2C_H_

#include "main.h"


/* Flags definitions */
#define _ACKF_    0 // ACK/NACK Flag
#define _WRF_     1 // Write Flag
#define _BEF_     2 // Bus Error Flag


#define I2CDDR    DDRB
#define I2CPORT   PORTB
#define I2CPIN    PINB
#define I2CSDA    PINB0
#define I2CSCL    PINB2


#define SDA_H     I2CPORT |= _BV(I2CSDA)
#define SDA_L     I2CPORT &= ~_BV(I2CSDA)
#define SCL_H     I2CPORT |= _BV(I2CSCL)
#define SCL_L     I2CPORT &= ~_BV(I2CSCL)

#define	SDA_OUT   I2CDDR |= _BV(I2CSDA)
#define	SDA_IN    I2CDDR &= ~_BV(I2CSDA)


void I2C_Start(void);
void I2C_Stop(void);
void I2C_Transfer(void);
uint8_t I2C_ReceiveAckNack(void);
void I2C_SendAckNack(void);
void I2C_SendAddress(uint8_t);
void I2C_SendByte(uint8_t);
uint8_t I2C_ReceiveByte(void);

volatile uint8_t* Get_I2CREG(void);


#endif /* I2C_H_ */