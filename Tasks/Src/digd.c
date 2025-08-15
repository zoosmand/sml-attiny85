/*
 * Filename: digd.c
 * Description: The file contains digital display service code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 15.08.2025 06:25:48 AM
 * Author: Dmitry Slobodchikov
*/ 
#include "digd.h"

/* Private variables */
// volatile static uint8_t* _ddreg;
static uint16_t sysStep = SRV_STEP;

/* Private function definitions */
static uint8_t PrintDigitalDisplay_Handler(void);



/**
 * @brief   Sets up a scheduler for digital display printing.
 * @retval  (uint8_t) status of operation
 */
uint8_t PrintDigitalDisplay_Scheduler(void) {
  volatile uint16_t* sysCnt = Get_SysCnt();

  if (sysStep == *sysCnt) {
    if (PrintDigitalDisplay_Handler()) return 1;
    sysStep = *sysCnt + SRV_STEP;
  }

  return 0;
}


/**
 * @brief   Handles a digital display printing.
 * @retval  (uint8_t) status of operation
 */
static uint8_t PrintDigitalDisplay_Handler(void) {
  static uint8_t digs[4] = {0x0b, 0x0b, 0x0b, 0x0b};
  uint16_t secCnt = Get_SecCnt();

  
  digs[0] = secCnt/1000%10;
  if (secCnt < 1000) digs[0] = 11;
  digs[1] = secCnt/100%10;
  if (secCnt < 100) digs[1] = 11;
  digs[2] = secCnt/10%10;
  if (secCnt < 10) digs[2] = 11;
  digs[3] = secCnt%10;
  DigitalDisplaySend(digs, 0);

  return 0;
}
