//
// Created by Denis Bilyk on 8/31/15.
//

#ifndef _PINS_H
#define _PINS_H

#include <avr/interrupt.h>
#include "avr/io.h"
#include "stdint.h"

struct pin_map_t {
    volatile uint8_t* ddr;   /**< address of DDR for this pin */
    volatile uint8_t* pin;   /**< address of PIN for this pin */
    volatile uint8_t* port;  /**< address of PORT for this pin */
    uint8_t bit;             /**< bit number for this pin */
};

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2


#if defined(__AVR_ATmega168__)\
||defined(__AVR_ATmega168P__)\
||defined(__AVR_ATmega328P__)
// 168 and 328 Arduinos
const static pin_map_t pinMap[] = {
        {&DDRD, &PIND, &PORTD, 0},  // D0  0
        {&DDRD, &PIND, &PORTD, 1},  // D1  1
        {&DDRD, &PIND, &PORTD, 2},  // D2  2
        {&DDRD, &PIND, &PORTD, 3},  // D3  3
        {&DDRD, &PIND, &PORTD, 4},  // D4  4
        {&DDRD, &PIND, &PORTD, 5},  // D5  5
        {&DDRD, &PIND, &PORTD, 6},  // D6  6
        {&DDRD, &PIND, &PORTD, 7},  // D7  7
        {&DDRB, &PINB, &PORTB, 0},  // B0  8
        {&DDRB, &PINB, &PORTB, 1},  // B1  9
        {&DDRB, &PINB, &PORTB, 2},  // B2 10
        {&DDRB, &PINB, &PORTB, 3},  // B3 11
        {&DDRB, &PINB, &PORTB, 4},  // B4 12
        {&DDRB, &PINB, &PORTB, 5},  // B5 13
        {&DDRC, &PINC, &PORTC, 0},  // C0 14
        {&DDRC, &PINC, &PORTC, 1},  // C1 15
        {&DDRC, &PINC, &PORTC, 2},  // C2 16
        {&DDRC, &PINC, &PORTC, 3},  // C3 17
        {&DDRC, &PINC, &PORTC, 4},  // C4 18
        {&DDRC, &PINC, &PORTC, 5}   // C5 19
};
#endif

#endif //_PINS_H
