/*
 * Filename: main.c
 * Description: The main frogram file.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 9:53:17 AM
 * Author: Dmitry Slobodchikov
 */ 

#include "main.h"

/* Private variables */
static volatile uint8_t   _GREG_  = 0;
static volatile uint16_t  sysCnt  = 0;
static volatile uint16_t  secCnt  = 0;

/* Private function definitions */
static void Cron(void);
static void SysTick_Handler(void);
static void Second_Handler(void);

/* STDOUT definition */
static FILE dsplout = FDEV_SETUP_STREAM(putc_dspl, NULL, _FDEV_SETUP_WRITE);



/**
 * @brief   The application entry point.
 * @retval  (int)
 */
int main(void) {
  /* Initialization block */
  cli();
  _INIT_MCU;
  _INIT_WDG;
  _INIT_LED;
  _INIT_TIMERS;
  _INIT_I2C;
  Init_ISR();
  Init_Display();
  // Init_DigitalDisplay();
  Init_OneWire();
  sei();

  /* --- Init default standard output into display --- */
  stdout = Init_DsplOut();
  Cron();

}


static void Cron(void) {
   while (1) {
    SysTick_Handler();
    Second_Handler();
    // PrintDigitalDisplay_Scheduler();
  }
}


/**
 * @brief   The application system cron service handler.
 * @retval  none
 */
static void SysTick_Handler(void) {
  cli();
  if (FLAG_CHECK(_GREG_, _SYSTF_)) {
    FLAG_CLR(_GREG_, _SYSTF_);
    sysCnt &= SEC_TICK_MASK;

    if (!sysCnt) {
      secCnt++;
      FLAG_SET(_GREG_, _SECTF_);
    }
    GetTemperature_Scheduler();
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

    // printf("sec:%u\n", secCnt);
  }
}


/**
 * @brief   Simple millisecond-step delay.
 * @param   delay delay value in millis
 * @param   reg pointer to the control register
 * @param   flag control flag
 * @retval  none
 */
void _delay_ms(uint16_t delay, volatile uint8_t* reg, uint8_t flag) {
  FLAG_SET(*reg, flag);
  uint16_t tmpDelay = (sysCnt + delay) & SEC_TICK_MASK;
  while (tmpDelay != sysCnt) {
    SysTick_Handler();
    Second_Handler();
  }
  FLAG_CLR(*reg, flag);
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

FILE* Init_DsplOut(void) {
  return &dsplout;
}


/* Setters */
