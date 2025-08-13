/*
 * Filename: ds18b20.h
 * Description: The file contains definitions for DS18B20 OneWire temperature sensor code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 09:53:10 PM
 * Author: Dmitry Slobodchikov
 */
#ifndef DS18B20_H_
#define DS18B20_H_


#include "ow.h"
#include "eeprom.h"

//////////////////////////////////////////////////////////////////////////
#define ConvertT          0x44
#define WriteScratchpad   0x4e
#define ReadScratchpad    0xbe
#define CopyScratchpad    0x48
#define RecallEeprom      0xb8



//////////////////////////////////////////////////////////////////////////
uint8_t* DS18B20_ReadScrachpad(uint8_t*);
uint8_t DS18B20_WriteScratchpad(uint8_t*, uint8_t*);
uint8_t DS18B20_ConvertTemperature(uint8_t*);
uint8_t DS18B20_CopyScratchpad(uint8_t*);
uint8_t DS18B20_RecallEeprom(uint8_t*);


#endif /* DS18B20_H_ */