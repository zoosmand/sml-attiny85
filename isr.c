/*
 * Filename: isr.c
 * Description: The file contains interruption routines.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author : Dmitry Slobodchikov
 */ 

#include "main.h"

/* Private variables */
volatile static uint8_t* _greg;
volatile static uint16_t* _sysCnt;


/**
 * @brief   Critical variables initialization.
 * @retval  none
 */
void Init_ISR(void) {
  _greg = Get_GREG();
  _sysCnt = Get_SysCnt();
}


/**
 * @brief   Timer0 (TIM0) interrupt routine.
 * @retval  none
 */
ISR(TIMER0_OVF_vect) {
  TCNT0 = SYS_TICK_THOLD;
  FLAG_SET(*_greg, _SYSTF_);
  (*_sysCnt)++;
}


/**
 * @brief   Watchdog (WDG) interrupt routine.
 * @retval  none
 */
ISR(WDT_vect) {
  wdt_reset();
  WDTCR |= _BV(WDCE)|_BV(WDIE);
}
