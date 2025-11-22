/*
 * Filename: prnt.h
 * Description: A set of definitions for printing service code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 17.08.2025 05:15:22 PM
 * Author: Dmitry Slobodchikov
*/ 
#ifndef PRNT_H_
#define PRNT_H_


#include "main.h"

/* --- Periodial step value --- */
#define PRNT_SRV_STEP  1 // here is a millis value that derives from sysCnt


uint8_t Print_Scheduler(void);


#endif /* PRNT_H_ */