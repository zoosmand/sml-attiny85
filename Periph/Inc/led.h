/*
 * Filename: led.h
 * Description: A set of definitions for LED implementation. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 11:02:27 PM
 * Author : Dmitry Slobodchikov
*/ 
#ifndef LED_H_
#define LED_H_


#include "main.h"
#include "init_led.h"



#define LEDDDR    DDRB
#define LEDPORT   PORTB
#define LEDPIN    PINB
#define LED0PIN   PINB1

void LedToggle_Handler(void);


#endif /* LED_H_ */