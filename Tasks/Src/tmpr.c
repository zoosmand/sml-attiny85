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
static uint16_t secStep = 4;

/* Private function definitions */
static uint8_t GetTemperatur_Handler(uint8_t);


/**
 * @brief   Sets up a scheduler for temperature measurement.
 * @retval  (uint8_t) status of operation
 */
uint8_t GetTemperature_Scheduler(void) {
  uint16_t secCnt = Get_SecCnt();

  if (secStep == secCnt) {
    _owreg = Get_OWREG();
    /* --- Init default standard output into display --- */
    stdout = Init_DsplOut();


    for (uint8_t i = 0; i < (*_owreg & 0x0f); i++) {
      if (GetTemperatur_Handler(i)) printf("Fail:%u\n", i);
    }
    secStep = secCnt + 4;
  }

  return 0;
}


/**
 * @brief   Handles a temperature measurement.
 * @param   num a sequential number in the list of devices enumerated by OneWire bus
 * @retval  (uint8_t) status of operation
 */
static uint8_t GetTemperatur_Handler(uint8_t num) {
  if (num >= *_owreg) return 1;

  uint8_t addr[8];
  uint8_t spad[9];

  if (EEPROM_ReadBuffer(EE_OW_ADDR + (num * 8), addr, 8)) return 1;
  if (DS18B20_ConvertTemperature(addr)) return 1;
  if (DS18B20_ReadScrachpad(addr, spad)) return 1;
  uint16_t* t1 = (uint16_t*)spad;
  
  printf("T:%d.%02u\n", (int16_t)(*t1 >> 4), (uint8_t)(((*t1 & 0x000f) * 100) >> 4));

  return 0;
}
