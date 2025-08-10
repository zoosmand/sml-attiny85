/*
* Filename: main.h
 * Description: A set of definitions for the mail program file. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author: Dmitry Slobodchikov
*/ 


#ifndef MAIN_H_
#define MAIN_H_


#ifndef F_CPU
#define F_CPU 16000000
#endif


#include <avr/io.h>
// #include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "def.h"
#include "macroses.h"
#include "init_periph.h"
#include "led.h"
#include "i2c.h"
#include "display.h"


/* Exported functions */
volatile uint8_t* Get_GREG(void);
volatile uint16_t* Get_SysCnt(void);
volatile uint16_t Get_SecCnt(void);

void Init_ISR(void);
void _delay_us(uint16_t);


#endif /* MAIN_H_ */