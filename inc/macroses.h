/*
 * Filename: macroses.h
 * Description: A set of common macroses. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author : Dmitry Slobodchikov
*/
#ifndef MACROSES_H_
#define MACROSES_H_


#include "main.h"


/* Flag management macroses */
#define FLAG_SET(reg, flag)     reg |= _BV(flag)
#define FLAG_CLR(reg, flag)     reg &= ~_BV(flag)
#define FLAG_CHECK(reg, flag)   (reg & _BV(flag))


/* No operation masros */
#define _NOP do {__asm__ __volatile__ ("nop");} while(0)


/* Initialization macroses */
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
/* --- Timer0 counts 1ms system tick --- */
#define _INIT_TIMERS do { \
  TCCR0B  |= _BV(CS02)|_BV(CS00); \
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


#endif /* MACROSES_H_ */
