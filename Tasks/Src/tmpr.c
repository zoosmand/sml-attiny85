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
volatile static uint8_t* _dsreg;
static uint16_t taskCnt = TMPR_SRV_STEP;
static uint8_t curAddr[8];
static uint8_t spad[9];


/* Private function definitions */
static uint8_t GetTemperatur_Handler(uint8_t);


/**
 * @brief   Sets up a scheduler for temperature measurement.
 * @retval  (uint8_t) status of operation
 */
uint8_t GetTemperature_Scheduler(void) {
  if (!FLAG_CHECK(*Get_PREG(), _OWBUSRF_)) return 1;
  if (FLAG_CHECK(*Get_DSREG(), _DSDF_)) return 1;

  if (!(--taskCnt)) {
    _owreg = Get_OWREG();
    // for (uint8_t i = 0; i < (*_owreg & 0x0f); i++) {
      //   if (GetTemperatur_Handler(i)) printf("Fail:%u\n", i);
      // }
    /* --- Get tepmperatur from the given device "0" --- */
    if (GetTemperatur_Handler(0)) {
      /* --- on error, set up -128.00 C --- */
      spad[0] = 0x00;
      spad[1] = 0x08;
    }
    taskCnt = TMPR_SRV_STEP;
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

  if (EEPROM_ReadBuffer(EE_OW_ADDR + (num * 8), curAddr, 8)) return 1;
  if (DS18B20_ConvertTemperature(curAddr)) return 1;
  if (DS18B20_ReadScrachpad(curAddr, spad)) return 1;

  return 0;
}


/* Getters */
uint8_t* Get_Spad(void) {
  if (!FLAG_CHECK(*Get_PREG(), _OWBUSRF_)) {
    spad[0] = 0x00;
    spad[1] = 0x08;
  }
  return spad;
}

