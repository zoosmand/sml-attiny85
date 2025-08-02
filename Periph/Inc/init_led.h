/*
 * Filename: init_led.h
 * Description: A set of definitions for LED initialization. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 8:40:57 PM
 * Author : Dmitry Slobodchikov
*/ 
#ifndef LED_INIT_H_
#define LED_INIT_H_


#include "main.h"


#define	_INIT_LED do { \
  LEDDDR |= _BV(LED0PIN); \
} while (0)


#endif /* LED_INIT_H_ */