/*
 * Filename: led.h
 * Description: A set of definitions LED logic code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 17.08.2025 08:11:41 PM
 * Author: Dmitry Slobodchikov
*/ 
#ifndef LED_H_
#define LED_H_


#include "main.h"

#define LEDDDR    DDRB
#define LEDPORT   PORTB
#define LEDPIN    PINB
#define LED0PIN   PINB1

/* --- Periodial step value --- */
#define LED_SRV_STEP  500 // here is a sec value that derives from sysCnt


uint8_t LedToggle_Scheduler(void);


#endif /* LED_H_ */