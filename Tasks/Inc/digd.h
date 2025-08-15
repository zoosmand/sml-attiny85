/*
 * Filename: digd.h
 * Description: A set of definitions digital display service code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 15.08.2025 06:25:48 AM
 * Author: Dmitry Slobodchikov
*/ 
#ifndef TMPR_H_
#define TMPR_H_


#include "main.h"

/* --- Periodial step value --- */
#define SRV_STEP  500 // here is a millis value that derives from sysCnt


uint8_t GetTemperature_Scheduler(void);


#endif /* TMPR_H_ */