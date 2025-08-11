/*
 * Filename: digit_display.h
 * Description: The file contains digit display definitions.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 10.08.2025 21:07:41 AM
 * Author : Dmitry Slobodchikov
 */
#ifndef _DIGIT_DISPLAY_H
#define _DIGIT_DISPLAY_H

#include "main.h"


#define DSPLDDR     DDRB
#define DSPLPORT    PORTB
#define DSPLPIN     PINB
#define DSPLDIO     PINB3
#define DSPLCLK     PINB4


#define DIO_H DSPLPORT |= _BV(DSPLDIO);
#define DIO_L DSPLPORT &= ~_BV(DSPLDIO);
#define CLK_H DSPLPORT |= _BV(DSPLCLK);
#define CLK_L DSPLPORT &= ~_BV(DSPLCLK);

#define	DIO_OUT DSPLDDR |= _BV(DSPLDIO)
#define	DIO_IN  DSPLDDR &= ~_BV(DSPLDIO)




uint8_t Init_DigitalDisplay(void);
void DigitalDisplaySend(const uint8_t *buf, int8_t dot);


#endif /* _DIGIT_DISPLAY_H */