/*
 * Filename: led.c
 * Description: The file contains LED logic code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 17.08.2025 08:12:19 AM
 * Author: Dmitry Slobodchikov
*/ 
#include "led.h"

/* Private variables */
static uint16_t taskCnt = LED_SRV_STEP;

/* Private function definitions */
static uint8_t LedToggle_Handler(void);



/**
 * @brief   Sets up a scheduler for LED toggling.
 * @retval  (uint8_t) status of operation
 */
uint8_t LedToggle_Scheduler(void) {
  if (!(--taskCnt)) {
    if (LedToggle_Handler()) return 1;
     taskCnt = LED_SRV_STEP;
  }
  return 0;
}


/**
 * @brief   Handles LED toggling.
 * @retval  (uint8_t) status of operation
 */
static uint8_t LedToggle_Handler(void) {
  if (LEDPIN & _BV(LED0PIN)) {
    LEDPORT &= ~_BV(LED0PIN);
  } else {
    LEDPORT |= _BV(LED0PIN);
  }

  return 0;
}
