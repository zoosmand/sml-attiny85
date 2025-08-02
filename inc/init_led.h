/*
 * Filename: init_led.h
 * Description: A set of definitions for LED implementation. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author : Dmitry Slobodchikov
*/ 
#ifndef LED_INIT_H_
#define LED_INIT_H_


#include "main.h"


#define LEDDDR    DDRB
#define LEDPORT   PORTB
#define LEDPIN    PINB
#define LED0PIN   PINB1

#define	_INIT_LED do { \
  LEDDDR |= _BV(LED0PIN); \
} while (0)


#endif /* LED_INIT_H_ */