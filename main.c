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

static volatile uint8_t   _GREG_  = 0;
static volatile uint16_t  sysCnt  = 0;
static volatile uint16_t  secCnt  = 0;

static void Cron_Handler(void);
static void Second_Handler(void);

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
    sysCnt &= SEC_TICK_MASK;

    if (!sysCnt) {
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

    stdout = &dsplout;
    printf("sec:%d\n", secCnt);

    // static uint8_t digs[4] = {0x0b, 0x0b, 0x0b, 0x0b};
    // digs[0] = secCnt/1000%10;
    // if (secCnt < 1000) digs[0] = 11;
    // digs[1] = secCnt/100%10;
    // if (secCnt < 100) digs[1] = 11;
    // digs[2] = secCnt/10%10;
    // if (secCnt < 10) digs[2] = 11;
    // digs[3] = secCnt%10;
    // DigitalDisplaySend(digs, 0);

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
