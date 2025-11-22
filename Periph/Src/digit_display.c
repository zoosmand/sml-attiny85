/*
 * Filename: digit_display.c
 * Description: The file contains digit display code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 10.08.2025 21:07:41 AM
 * Author : Dmitry Slobodchikov
 */

#include "digit_display.h"

/* Private constants */
const uint8_t digits[12] PROGMEM = {
  0x3f,
  0x06,
  0x5b,
  0x4f,
  0x66,
  0x6d,
  0x7d,
  0x07,
  0x7f,
  0x6f,
  0x80,
  0x00
};


/* Private function prototypes */
static void Dd_Start(void);
static void Dd_Stop(void);
static void Dd_WriteByte(uint8_t byte);


/**
 * @brief  Digital display start condition
 * @retval none
 */
static void Dd_Start(void) {
  CLK_H;
  DIO_H;
  _delay_us(2);
  DIO_L;
  _delay_us(2);
}


/**
 * @brief  Digital display stop condition
 * @retval none
 */
static void Dd_Stop(void) {
  CLK_L;
  _delay_us(2);
  DIO_L;
  _delay_us(2);
  CLK_H;
  _delay_us(2);
  DIO_H;
  _delay_us(2);
}


/**
 * @brief  Digital display write/send a by to the bus
 * @param  byte A byte to send/write
 * @retval none
 */
static void Dd_WriteByte(uint8_t byte) {
  
  for (uint8_t i = 0; i < 8; i++) {
    CLK_L;
    if (byte & 0x01) {
      DIO_H;
    } else {
      DIO_L;
    }
    byte >>= 1;
    _delay_us(3);
    CLK_H;
    _delay_us(3);
  }
  
  CLK_L;
  _delay_us(5);
  DIO_IN;
  if (!(DSPLPIN & _BV(DSPLDIO))) {
    CLK_H;
    _delay_us(2);
    CLK_L;
  }
  DIO_OUT;
}


/**
 * @brief  Digital display initialization code; clearing the display
 * @retval (uint8_t) the operation status
 */
uint8_t Init_DigitalDisplay(void) {
  /* OneWire bus and Display share PB4 pin
     if OneWire on board display to be ignored */
  if (FLAG_CHECK(*Get_PREG(), _OWBUSRF_)) return 1;

  _INIT_DIGIT_DSPL;
  Dd_Start();
  Dd_WriteByte(0x40);
  Dd_Stop();
  
  Dd_Start();
  Dd_WriteByte(0xc0);
  for (uint8_t i = 0; i < 4; i++) {
    Dd_WriteByte(0x00);
  }
  Dd_Stop();
  
  Dd_Start();
  Dd_WriteByte(0x8a);
  Dd_Stop();

  return 0;
}


/**
 * @brief  Digital display write/send data buffer
 * @param  buf A pointer to the buffer to send
 * @param  dot A boolean indicates the usage of a dot symbol
 * @retval (uint8_t) the operation status
 */
void DigitalDisplaySend(const uint8_t *buf, int8_t dot) {
  Dd_Start();
  Dd_WriteByte(0x40);
  Dd_Stop();
  
  Dd_Start();
  Dd_WriteByte(0xc0);
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t x = pgm_read_byte(&digits[(*buf++)]);
    if (i == dot) x |= pgm_read_byte(&digits[10]);
    Dd_WriteByte(x);
  }
  Dd_Stop();
}
