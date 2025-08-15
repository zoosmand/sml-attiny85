/*
 * Filename: tmpr.h
 * Description: A set of definitions themperature measurment code. 
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 13.08.2025 08:55:29 PM
 * Author: Dmitry Slobodchikov
*/ 
#ifndef DIGD_H_
#define DIGD_H_


#include "main.h"

/* --- Periodial step value --- */
#define SRV_STEP  4 // here is a sec value that derives from secCnt


uint8_t PrintDigitalDisplay_Scheduler(void);


#endif /* DIGD_H_ */