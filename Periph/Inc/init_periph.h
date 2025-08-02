/*
 * Filename: init_led.h
 * Description: A set of definitions for LED initialization. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 8:40:57 PM
 * Author : Dmitry Slobodchikov
*/ 
#ifndef INIT_PERIPH_H_
#define INIT_PERIPH_H_


#include "main.h"


/* Initialization macroses */
/* --- LED --- */
#define	_INIT_LED do { \
  LEDDDR |= _BV(LED0PIN); \
} while (0)


/* --- Microcontroller unit --- */
/* --- Enable Sleep Mode (Idle mode) --- */
/* --- Reduce Power Consumption (16.2.2 p.120) --- */
/* --- Power reduce (7.5.2 p.38) --- */
#define _INIT_MCU do { \
  MCUSR   |= _BV(SE); \
  ACSR    |= _BV(ACD); \
  PRR     |= _BV(PRADC)|_BV(PRTIM1); \
} while (0)  


/* --- Timers --- */
/* --- Timer0 counts ~1ms system tick --- */
#define _INIT_TIMERS do { \
  TCCR0B  |= _BV(CS02); \
  TIMSK   |= _BV(TOIE0); \
  TCNT0   = SYS_TICK_THOLD; \
} while (0)


/* --- Watchdog (8.5.2 p.45) --- */
/* --- MCU to reboot in ~8s by an event --- */
#define _INIT_WDG do { \
  MCUSR   &= ~_BV(WDRF); \
  WDTCR   |= _BV(WDCE)|_BV(WDIE); \
  WDTCR   |= _BV(WDP3)|_BV(WDP0)|_BV(WDE); \
} while (0)


#endif /* INIT_PERIPH_H_ */