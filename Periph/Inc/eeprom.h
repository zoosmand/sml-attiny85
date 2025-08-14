/*
 * Filename: eeprom.h
 * Description: The file contains definition for EEPROM code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 04:58:39 AM
 * Author: Dmitry Slobodchikov
 */
#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"


uint8_t EEPROM_WriteBuffer(uint16_t, uint8_t*, uint16_t);
uint8_t EEPROM_ReadBuffer(uint16_t, uint8_t*, uint16_t);


#endif /* EEPROM_H_ */