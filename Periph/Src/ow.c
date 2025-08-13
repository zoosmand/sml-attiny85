/*
 * Filename: ow.c
 * Description: The file contains OneWire code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 05:06:33 AM
 * Author: Dmitry Slobodchikov
 */
#include "ow.h"

/* Private variables */
static volatile uint8_t _OWREG_ = 0; // OW device counter is in [3:0], OW devices in alarm mode in [7:4]
static uint8_t addr[8];
static uint8_t tmpAddr[8];
static uint8_t lastfork;
static uint8_t addrBufLen = 8;

/* Private function definitions */
static void OneWire_WriteBit(uint8_t);
static void OneWire_CollectAddresses(uint16_t);
static uint8_t OneWire_Enumerate(uint8_t);


/**
 * @brief   Initializes OneWire bus.
 * @retval  (uint8_t) status of operation
 */
uint8_t Init_OneWire(void) {
  OW_SP_DOWN;
  
  if (!OneWire_Reset()) {
    OneWire_CollectAddresses(EE_OW_ADDR);
    return 0; // no error during initialization
  }
  return 1; // initialization error
}


/**
 * @brief   Runs reset condition on OneWire bus.
 * @retval  (uint8_t) status of operation
 */
uint8_t OneWire_Reset(void) {
  OW_UP;
  _delay_us(480);
  OW_DOWN;
  _delay_us(70);
  if (OW_LEVEL) return 1; // error on the bus
  OW_DOWN;
  _delay_us(410);
  return 0; // no error on the bus
}


/**
 * @brief   Write a bit into OneWire bus.
 * @param   bit a bit to write
 * @retval  none
 */
static void OneWire_WriteBit(uint8_t bit) {
  OW_L;
  if (bit) {
    _delay_us(6);
    OW_H;
    _delay_us(64);
    } else {
    _delay_us(60);
    OW_H;
    _delay_us(10);
  }
}


/**
 * @brief   Reads a bit from OneWire bus.
 * @retval  (uint8_t) bit to be read
 */
uint8_t OneWire_ReadBit(void) {
  uint8_t bit = 0;

  OW_L;
  _delay_us(6);
  OW_H;
  _delay_us(9);
  bit = OW_LEVEL;
  _delay_us(55);
  
  return bit;
}


/**
 * @brief   Write a byte into OneWire bus.
 * @param   data a byte to write
 * @retval  none
 */
void OneWire_WriteByte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteBit((data >> i) & 1);
  }
}


/**
 * @brief   Reads a byte from OneWire bus.
 * @retval  (uint8_t) byte to be read
 */
uint8_t OneWire_ReadByte(void) {
  uint8_t data = 0;
  for (uint8_t i = 0; i < 8; i++) {
    data >>= 1;
    data |= (OneWire_ReadBit()) ? 0x80 : 0;
  }
  return data;
}


/**
 * @brief   Collects OneWire device addresses and writes them to EEPROM.
 * @param   eepromAddr EEPROM pointer for storing addresses
 * @retval  none
 */
static void OneWire_CollectAddresses(uint16_t eepromAddr) {
  lastfork = 65;
  
  while (!OneWire_Enumerate(SearchROM)) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < addrBufLen; i++) {
      crc = OneWire_CRC(crc, addr[i]);
    }
    
    if (!crc) {
      EEPROM_ReadBuffer(eepromAddr, tmpAddr, addrBufLen);
      if (cmpBBufs(addr, tmpAddr, addrBufLen)) {
        EEPROM_WriteBuffer(eepromAddr, addr, addrBufLen);
      }
      eepromAddr += addrBufLen;
      /* Increment OneWire device counter */
      _OWREG_ = (_OWREG_ & 0xf0) | ((_OWREG_ & 0x0f) + 1);
    }
  }
}


/**
 * @brief   Collects OneWire device addresses in alarm state and writes them to EEPROM.
 * @param   eepromAddr EEPROM pointer for storing addresses
 * @retval  none
 */
void OneWire_CollectAlarms(uint16_t eepromAddr) {
  lastfork = 65;
  
  while (OneWire_Enumerate(SearchAlarmROM)) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < addrBufLen; i++) {
      crc = OneWire_CRC(crc, addr[i]);
    }
    
    if (!crc) {
      EEPROM_ReadBuffer(eepromAddr, tmpAddr, addrBufLen);
      if (cmpBBufs(addr, tmpAddr, addrBufLen)) {
        EEPROM_WriteBuffer(eepromAddr, addr, addrBufLen);
      }
      eepromAddr += 8;
      /* Increment OneWire in alarm mode device counter */
      _OWREG_ = ((_OWREG_ & 0xf0) + (1 << 4)) | (_OWREG_ & 0x0f);
    }
  }
}


/**
 * @brief   Calculates CRC of a OneWire device.
 * @param   crc current CRC value
 * @param   data source value
 * @retval  (uint8_t) calculated CRC value
 */
uint8_t OneWire_CRC(uint8_t crc, uint8_t data) {
  // 0x8c - it's a bit reversed of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
    crc = ((crc ^ (data >> i)) & 0x01) ? ((crc >> 1) ^ 0x8c) : (crc >> 1);
  }
  return crc;
}


/**
 * @brief   Enumerates OneWire device addresses. Realizes BTREE traversal method.
 * @param   cmd OneOre bus command
 * @retval  (uint8_t) status of operation
 */
static uint8_t OneWire_Enumerate(uint8_t cmd) {
  if (!lastfork) return 1;
  
  if (OneWire_Reset()) return 1;

  uint8_t bp = 7;
  uint8_t prev = addr[0];
  uint8_t curr = 0;
  uint8_t fork = 0;
  uint8_t bit0 = 0;
  uint8_t bit1 = 0;
  uint8_t cnt = 0;

  OneWire_WriteByte(cmd);

  for(uint8_t i = 1; i < 65; i++) {
    bit0 = OneWire_ReadBit();
    bit1 = OneWire_ReadBit();

    if (!bit0) {
      if (!bit1) {
        if (i < lastfork) {
          if (prev & 1) {
            curr |= 0x80;
          } else {
            fork = i;
          }
        } else if (i == lastfork) {
          curr |= 0x80;
          } else {
          fork = i;
        }
      }
    } else {
      if (!bit1) {
        curr |= 0x80;
      } else {
        return 1;
      }
    }
    
    OneWire_WriteBit(curr & 0x80);
    
    if (!bp) {
      addr[cnt] = curr;
      curr = 0;
      prev = addr[++cnt];
      bp = 8;
    } else {
      prev >>= 1;
      curr >>= 1;
    }
    bp--;
  }
  lastfork = fork;
  return 0;
}


/**
 * @brief   Defines parasitic powered devices on OnWire bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of power supply
 */
uint8_t OneWire_ReadPowerSupply(uint8_t* addr) {
  OneWire_MatchROM(addr);
  OneWire_WriteByte(ReadPowerSupply);
  
  return !OneWire_ReadBit();
}


/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
uint8_t OneWire_MatchROM(uint8_t* addr) {
  if (!OneWire_Reset()) return 1;
  
  OneWire_WriteByte(MatchROM);
  for (uint8_t i = 0; i < addrBufLen; i++) {
    OneWire_WriteByte(addr[i]);
  }

  return 0;
}

