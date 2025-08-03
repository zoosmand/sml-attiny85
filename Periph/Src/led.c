/*
 * Filename: led.c
 * Description: The file contains LED implementation code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 02.08.2025 11:05:17 PM
 * Author : Dmitry Slobodchikov
 */

#include "led.h"


/**
 * @brief   The toggling LED handler.
 * @retval  none
 */
void LedToggle_Handler(void) {
  if (LEDPIN & _BV(LED0PIN)) {
    LEDPORT &= ~_BV(LED0PIN);
  } else {
    LEDPORT |= _BV(LED0PIN);
  }
}
