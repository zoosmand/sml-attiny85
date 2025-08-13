/*
 * Filename: ds18b20.c
 * Description: The file contains DS18B20 OneWire temperature sensor code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 09:53:10 PM
 * Author: Dmitry Slobodchikov
 */
#include "ds18b20.h"




/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t DS18B20_ReadScrachpad(uint8_t* addr, uint8_t* buf) {
  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(ReadScratchpad);

  uint8_t crc = 0;
  for (int8_t i = 0; i < 9; i++) {
    buf[i] = OneWire_ReadByte();
    crc = OneWire_CRC(crc, buf[i]);
  }
  if (crc) return 1;
  
  return 0;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t DS18B20_WriteScratchpad(uint8_t* addr, uint8_t* buf) {
  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(WriteScratchpad);

  for (uint8_t i = 0; i < 3; i++) {
    OneWire_WriteByte(buf[i]);
  }
  
  return 0;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t DS18B20_ConvertTemperature(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  uint8_t pps = OneWire_ReadPowerSupply(addr);

  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(ConvertT);
  
  if (pps) {
    OW_SP_UP;
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    _delay_us(50000);
    OW_SP_DOWN;
  } else {
    /* TDOD implement timeout, handle independently */
    while(!OneWire_ReadBit());
  }
  
  return 1;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t DS18B20_CopyScratchpad(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  uint8_t pps = OneWire_ReadPowerSupply(addr);

  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(CopyScratchpad);

  if (pps) {
    OW_SP_UP;
    _delay_us(10000);
    OW_SP_DOWN;
  } else {
    /* TDOD implement timeout, handle independently */
    while(!OneWire_ReadBit());
  }
  
  return 0;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t DS18B20_RecallEeprom(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(RecallEeprom);
  
  /* TDOD implement timeout, handle independently */
  while(!OneWire_ReadBit());
  
  return 0;
}
