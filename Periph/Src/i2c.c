/*
 * Filename: i2c.c
 * Description: The file contains I2C bus code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 11:05:17 PM
 * Author : Dmitry Slobodchikov
 */

#include "i2c.h"

/* Private variables */
static volatile uint8_t _I2CREG_ = 0;

/* Private function definitions */
static void I2C_TransferBuffer(void);


/**
 * @brief   I2C start condition.
 * @retval  none
 */
void I2C_Start(void) {
  SCL_H;
  _delay_us(4);
  SDA_L;
  _delay_us(4);
  SCL_L;
  SDA_H;
  USISR |= (1 << USISIF);
}


/**
 * @brief   I2C stop condition.
 * @retval  none
 */
void I2C_Stop(void) {
  USIDR = 0x80;
  SDA_L;
  SCL_H;
  while (!(I2CPIN & (1 << I2CSCL)));
  _delay_us(4);
  SDA_H;
  _delay_us(4);
  USISR |= (1 << USISIF);
}


/**
 * @brief   I2C bus transfer data buffer.
 * @retval  none
 */
static void I2C_TransferBuffer(void) {
  uint8_t tmp = 0;

  tmp = USICR;
  tmp |= (1 << USITC);
  while (!(USISR & (1 << USIOIF))) {
    _delay_us(4);
    USICR = tmp;
    while (!(I2CPIN & (1 << I2CSCL)));
    _delay_us(4);
    USICR = tmp;
  }
  _delay_us(6);
}


/**
 * @brief   I2C bus transfer.
 * @retval  none
 */
void I2C_Transfer(void) {
  SCL_L;
  uint8_t tmp = 0;
  tmp = USISR;
  tmp |= (1 << USIOIF);
  tmp &= ~(0xe << USICNT0);
  USISR = tmp;
  
  I2C_TransferBuffer();
}


/**
 * @brief   I2C bus receving ACK/NACK.
 * @retval  (uint8_t) 1 = ACK, 0 = NACK
 */
uint8_t I2C_ReceiveAckNack(void) {
  SDA_IN;
  USISR |= ((1 << USIOIF)|(0xe << USICNT0));
  USIDR = 0;
  I2C_TransferBuffer();
  SDA_OUT;
  return (USIDR & 0x01);
}


/**
 * @brief   I2C sending ACK/NACK.
 * @retval  none
 */
void I2C_SendAckNack(void) {
  SDA_OUT;
  USISR |= (1 << USIOIF)|(0xe << USICNT0);
  
  if (FLAG_CHECK(_I2CREG_, _ACKF_)) {
    USIDR = 0x00;
  } else {
    USIDR = 0xff;
  }
  I2C_TransferBuffer();
}


/**
 * @brief   I2C bus send slave address.
 * @param   addr Stave I2C address 
 * @retval  none
 */
void I2C_SendAddress(uint8_t addr) {
  if (!(FLAG_CHECK(_I2CREG_, _WRF_))) {
    addr |= 0x01;
  }
  I2C_SendByte(addr);
}


/**
 * @brief   I2C bus send data byte.
 * @param   byte Data byte to send to slave 
 * @retval  none
 */
void I2C_SendByte(uint8_t byte) {
  USIDR = byte;
  I2C_Transfer();
  if (I2C_ReceiveAckNack()) {
    FLAG_SET(_I2CREG_, _BEF_);
    I2C_Stop();
  }
}


/**
 * @brief   I2C bus receive a byte from slave.
 * @retval  (uint8_t) Received data byte
 */
uint8_t I2C_ReceiveByte(void) {
  SDA_IN;
  I2C_Transfer();
  uint8_t byte = USIDR;
  I2C_SendAckNack();
  return (byte);
}


/* Getters */
volatile uint8_t* Get_I2CREG(void) {
  return &_I2CREG_;
}

