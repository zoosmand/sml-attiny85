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