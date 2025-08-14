/*
 * Filename: tmpr.c
 * Description: THis file contains themperature measurment code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 13.08.2025 08:55:29 PM
 * Author: Dmitry Slobodchikov
*/ 
#include "tmpr.h"

/* Private variables */
volatile static uint8_t* _owreg;


uint8_t GetTemperature(void) {
  _owreg = Get_OWREG();
  uint8_t addr[8];
  uint8_t spad[9];

  for (uint8_t i = 0; i < (* _owreg & 0x0f); i++) {
    if (EEPROM_ReadBuffer(EE_OW_ADDR, addr, 8)) return 1;
    if (DS18B20_ConvertTemperature(addr)) return 1;
    if (DS18B20_ReadScrachpad(addr, spad)) return 1;
    uint16_t* t1 = spad[0];
    printf("T: %d.%02u\n", (int8_t)(*t1 >> 4), (uint8_t)((*t1 & 0x0f) >> 4));
  }

  return 0;
}