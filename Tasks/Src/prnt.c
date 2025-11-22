/*
 * Filename: prnt.c
 * Description: The file contains printing service code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 17.08.2025 05:18:48 PM
 * Author: Dmitry Slobodchikov
*/ 
#include "prnt.h"

/* Private variables */
static uint16_t taskCnt = PRNT_SRV_STEP;


/* Private function definitions */
static uint8_t PrintSec_Handler(void);
static uint8_t PrintTmpr_Handler(void);



/**
 * @brief   Sets up a scheduler for printing.
 * @retval  (uint8_t) status of operation
 */
uint8_t Print_Scheduler(void) {
  if (!(--taskCnt)) {
    if (FLAG_CHECK(*Get_PREG(), _DSPLRF_)) {
      if (Get_SecCnt() % 5) {
        if (PrintSec_Handler()) return 1;
      } else {
        if (PrintTmpr_Handler()) return 1;
      }
    }
    taskCnt = PRNT_SRV_STEP;
  }
  return 0;
}


/**
 * @brief   Handles printing.
 * @retval  (uint8_t) status of operation
 */
static uint8_t PrintSec_Handler(void) {
  printf("sec:%u\n", Get_SecCnt());
  return 0;
}


/**
 * @brief   Handles printing.
 * @retval  (uint8_t) status of operation
 */
static uint8_t PrintTmpr_Handler(void) {
  uint16_t* t1 = (uint16_t*)Get_Spad();
  printf("T:%d.%02u\n", (int8_t)(*t1 >> 4), (uint8_t)(((*t1 & 0x000f) * 100) >> 4));
  return 0;
}
