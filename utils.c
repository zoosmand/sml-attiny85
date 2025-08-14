/*
 * Filename: utils.c
 * Description: The program utilities.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 03.08.2025 11:37:29 AM
 * Author: Dmitry Slobodchikov
 */ 

#include "main.h"


/**
 * @brief   Simple microsecond-step delay.
 * @retval  none
 */
void _delay_us(uint16_t delay) {
  while (delay) {
    _NOP;
    _NOP;
    _NOP;
    _NOP;
    _NOP;
    _NOP;
    _NOP;
    _NOP;
    delay--;
  }
}


/**
 * @brief   Compares two 8-bit type buffres for the given length.
 * @param   buf1 first buffer to compare
 * @param   buf2 second buffer to compare
 * @param   len length / depth of comparison
 * @retval  (uint8_t) status of operation
 */
uint8_t cmpBBufs(uint8_t* buf1, uint8_t* buf2, uint16_t len) {
  for (uint16_t i = 0; i < len; i++) {
    if (buf1[i] != buf2[i]) return 1;
  }
  return 0;
}