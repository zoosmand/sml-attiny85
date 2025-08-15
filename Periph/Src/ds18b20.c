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


/**
 * @brief   Reads the data scratchpad of the given device.
 * @param   addr a pointer to address of the given device
 * @param   buf a pointer to buffer to write data that has been read from the given device
 *          the size of the buffer is 9, the last byte is CRC
 * @retval  (uint8_t) status of operation
 */
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


/**
 * @brief   Writes a buffer data to the given device.
 * @param   addr a pointer to address of the given device
 * @param   buf a pointer to buffer with the data to write
 * @retval  (uint8_t) status of operation
 */
uint8_t DS18B20_WriteScratchpad(uint8_t* addr, uint8_t* buf) {
  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(WriteScratchpad);

  for (uint8_t i = 0; i < 3; i++) {
    OneWire_WriteByte(buf[i]);
  }
  
  return 0;
}


/**
 * @brief   Writes the convert temperature command to the given device.
 * @param   addr a pointer to address of the given device
 * @retval  (uint8_t) status of operation
 */
uint8_t DS18B20_ConvertTemperature(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  uint8_t pps = OneWire_ReadPowerSupply(addr);

  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(ConvertT);
  
  if (pps) {
    OW_SP_UP;
    /* TODO implement a proper pause of 750ms */
    OW_SP_DOWN;
  } else {
    /* TDOD implement timeout, handle independently */
    while(!OneWire_ReadBit());
  }
  
  return 0;
}


/**
 * @brief   Copies the scratchpad data of the given device.
 * @param   addr a pointer to address of the given device
 * @retval  (uint8_t) status of operation
 */
uint8_t DS18B20_CopyScratchpad(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  uint8_t pps = OneWire_ReadPowerSupply(addr);

  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(CopyScratchpad);

  if (pps) {
    OW_SP_UP;
    /* TODO implement a proper pause of 10ms */
    OW_SP_DOWN;
  } else {
    /* TDOD implement timeout, handle independently */
    while(!OneWire_ReadBit());
  }
  
  return 0;
}


/**
 * @brief   Recalls the EEPROM data of the given device.
 * @param   addr a pointer to address of the given device
 * @retval  (uint8_t) status of operation
 */
uint8_t DS18B20_RecallEeprom(uint8_t* addr) {
  if (OneWire_MatchROM(addr)) return 1;
  OneWire_WriteByte(RecallEeprom);
  
  /* TDOD implement timeout, handle independently */
  while(!OneWire_ReadBit());
  
  return 0;
}
