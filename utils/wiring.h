//
// Created by Denis Bilyk on 8/30/15.
//

#ifndef _WIRING_H
#define _WIRING_H

#include "stdint.h"

#include "avr/common.h"
#include "avr/interrupt.h"




static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK = 13;


#define min(a, b) ((a)<(b)?(a):(b))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )


void digitalWrite(uint8_t pin, uint8_t val);

void pinMode(uint8_t pin, uint8_t mode);

unsigned long millis();

unsigned long micros();

void delay(unsigned long ms);

void delayMicroseconds(unsigned int us);

void init();

#endif //_WIRING_H
