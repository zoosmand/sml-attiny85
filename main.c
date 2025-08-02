/*
 * Filename: main.c
 * Description: The main frogram file.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author : Dmitry Slobodchikov
 */ 

#include "main.h"

static volatile uint8_t   _GREG_  = 0;
static volatile uint16_t  sysCnt  = 0;
static volatile uint16_t  secCnt  = 0;

static void Cron_Handler(void);
static void Second_Handler(void);
static void LedToggle_Handler(void);


/**
 * @brief   The application entry point.
 * @retval  (int)
 */
int main(void) {
  /* Initialization block */
  cli();
  _INIT_WDG;
  _INIT_LED;
  _INIT_TIMERS;
  Init_ISR();
  sei();

  while (1) {
    Cron_Handler();
    Second_Handler();
  }

}


/**
 * @brief   The application system cron service handler.
 * @retval  none
 */
static void Cron_Handler(void) {
  cli();
  if (FLAG_CHECK(_GREG_, _SYSTF_)) {
    FLAG_CLR(_GREG_, _SYSTF_);

    if (sysCnt >= 1000) {
      sysCnt = 0;
      secCnt++;
      FLAG_SET(_GREG_, _SECTF_);
    }
  }
  sei();
}


/**
 * @brief   The application periodical event handler (secondly.)
 * @retval  none
 */
static void Second_Handler(void) {
  if (FLAG_CHECK(_GREG_, _SECTF_)) {
    FLAG_CLR(_GREG_, _SECTF_);
    LedToggle_Handler();
  }
}


/**
 * @brief   The toggling LED handler.
 * @retval  none
 */
static void LedToggle_Handler(void) {
  if (LEDPIN & _BV(LED0PIN)) {
    LEDPORT &= ~_BV(LED0PIN);
  } else {
    LEDPORT |= _BV(LED0PIN);
  }
}



/* Getters */
volatile uint8_t* Get_GREG(void) {
  return &_GREG_;
}

volatile uint16_t* Get_SysCnt(void) {
  return &sysCnt;
}

volatile uint16_t Get_SecCnt(void) {
  return secCnt;
}


/* Setters */
