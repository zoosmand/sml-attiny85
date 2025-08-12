/*
 * Filename: eeprom.c
 * Description: The file contains EEPROM code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 04:58:39 AM
 * Author: Dmitry Slobodchikov
 */
#include "eeprom.h"


/**
 * @brief   Writes the given buffer into EEPROM.
 * @param   addr EEPROM start address to write to
 * @param   buf pointer to the buffer source data
 * @param   len length of data to write
 * @retval  (uint8_t) status of operation
 */
uint8_t EEPROM_WriteBuffer(uint16_t addr, uint8_t* buf, uint16_t len) {
  while (len--) {
    /* TODO implement timeout with error exit */
    while (EECR & _BV(EEPE));
    cli();
    EEAR = addr;
    EEDR = *buf;
    EECR |= _BV(EEMPE);
    EECR |= _BV(EEPE);
    sei();
    addr++;
    buf++;
  }
  return 0;
}


/**
 * @brief   Reads EEPROM data into the given buffer.
 * @param   addr EEPROM start address to read from
 * @param   buf pointer to the buffer destination data
 * @param   len length of data to read
 * @retval  (uint8_t) status of operation
 */
uint8_t EEPROM_ReadBuffer(uint16_t addr, uint8_t *buf, uint16_t len) {
  while (len--) {
    /* TODO implement timeout with error exit */
    while(EECR & _BV(EEPE));
    EEAR = addr;
    EECR |= _BV(EERE);
    *buf = EEDR;
    addr++;
    buf++;
  }
  return 0;
}

