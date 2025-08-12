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

static volatile uint8_t _OWREG_ = 0; // OW device counter is in [3:0]
static uint8_t addr[8];
static uint8_t lastfork;


//////////////////////////////////////////////////////////////////////////
// Routines prototypes
uint8_t OneWire_Reset(void);
uint8_t OneWire_Init(void);
static void OneWire_WriteBit(uint8_t);
uint8_t OneWire_ReadBit(void);
void OneWire_WriteByte(uint8_t);
uint8_t OneWire_ReadByte(void);
static void OneWire_CollectAddresses(uint16_t);
void OneWire_CollectAlarms(uint16_t);
uint8_t OneWire_CRC(uint8_t, uint8_t);
static uint8_t OneWire_Enumerate(uint8_t);
uint8_t OneWire_ReadPowerSupply(uint8_t*);
uint8_t OneWire_MatchROM(uint8_t*);




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


/************************************************************************/
/*                                                                      */
/************************************************************************/
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


/************************************************************************/
/*                                                                      */
/************************************************************************/
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


/************************************************************************/
/*                                                                      */
/************************************************************************/
void OneWire_WriteByte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteBit((data >> i) & 1);
  }
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t OneWire_ReadByte(void) {
  uint8_t data = 0;
  for (uint8_t i = 0; i < 8; i++) {
    data >>= 1;
    data |= (OneWire_ReadBit()) ? 0x80 : 0;
  }
  return data;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
static void OneWire_CollectAddresses(uint16_t eepromAddr) {
  lastfork = 65;
  uint8_t bufLen = 8;
  
  while (!OneWire_Enumerate(SearchROM)) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < bufLen; i++) {
      crc = OneWire_CRC(crc, addr[i]);
    }
    
    if (!crc) {
      static uint8_t tmpAddr[8];
      EEPROM_ReadBuffer(eepromAddr, tmpAddr, bufLen);
      if (cmpBBufs(addr, tmpAddr, bufLen)) {
        EEPROM_WriteBuffer(eepromAddr, addr, bufLen);
      }
      eepromAddr += bufLen;
      /* Increment OneWire device counter */
      _OWREG_ = (_OWREG_ & 0xf0) | ((_OWREG_ & 0x0f) + 1);
    }
  }
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
void OneWire_CollectAlarms(uint16_t eepromAddr) {
  lastfork = 65;
  // owAlDevCnt = 0;
  
  while (OneWire_Enumerate(SearchAlarmROM)) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < 8; i++) {
      crc = OneWire_CRC(crc, addr[i]);
    }
    
    if (!crc) {
      EEPROM_WriteBuffer(eepromAddr, addr, 8);
      eepromAddr += 8;
      // owAlDevCnt++;
    }
  }
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t OneWire_CRC(uint8_t crc, uint8_t data) {
  // 0x8c - it's a bit reversed of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
    crc = ((crc ^ (data >> i)) & 0x01) ? ((crc >> 1) ^ 0x8c) : (crc >> 1);
  }
  return crc;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
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



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t OneWire_ReadPowerSupply(uint8_t* addr) {
  OneWire_MatchROM(addr);
  OneWire_WriteByte(ReadPowerSupply);
  
  return !OneWire_ReadBit();
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
uint8_t OneWire_MatchROM(uint8_t* addr) {
  if (!OneWire_Reset()) return 0;
  
  OneWire_WriteByte(MatchROM);
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteByte(addr[i]);
  }

  return 1;
}

