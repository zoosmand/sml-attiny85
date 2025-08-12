/*
 * Filename: ow.c
 * Description: The file contains definitions for OneWire code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 12.08.2025 05:06:33 AM
 * Author: Dmitry Slobodchikov
 */
#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "main.h"


#define OWDDR       DDRB
#define OWPORT      PORTB
#define OWPIN       PINB
#define OW0PIN      PB4


#define OW_UP       (OWDDR |= _BV(OW0PIN))
#define OW_DOWN     (OWDDR &= ~_BV(OW0PIN))
#define OW_LEVEL    (OWPIN & _BV(OW0PIN))
//#define OW_INPUT    (OWDDR &= ~_BV(OW0PIN))
//#define OW_OUTPUT   (OWDDR |= _BV(OW0PIN))

#define OW_L        OW_UP
#define OW_H        OW_DOWN


//////////////////////////////////////////////////////////////////////////
// Strong PullUp might be reassign to another PIN
//////////////////////////////////////////////////////////////////////////
#define OW_SP_UP  do { \
  OWPORT |= _BV(OW0PIN); \
  OWDDR |= _BV(OW0PIN); \
} while (0)

#define OW_SP_DOWN  do { \
  OWDDR &= ~_BV(OW0PIN); \
  OWPORT &= ~_BV(OW0PIN); \
} while (0)



//////////////////////////////////////////////////////////////////////////
// Flags
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#define EE_OW_ADDR        0x0040
#define EE_OW_ALAD        0x00e0



//////////////////////////////////////////////////////////////////////////
#define SearchROM         0xf0
#define SearchAlarmROM    0xec
#define ReadROM           0x33
#define MatchROM          0x55
#define SkipROM           0xcc
#define ReadPowerSupply   0xb4




//////////////////////////////////////////////////////////////////////////
uint8_t Init_OneWire(void);
uint8_t OneWire_Reset(void);
uint8_t OneWire_ReadBit(void);
void OneWire_WriteByte(uint8_t);
uint8_t OneWire_ReadByte(void);
uint8_t OneWire_CRC(uint8_t, uint8_t);
void OneWire_CollectAlarms(uint16_t);
uint8_t OneWire_ReadPowerSupply(uint8_t*);
uint8_t OneWire_MatchROM(uint8_t*);



#endif /* ONEWIRE_H_ */