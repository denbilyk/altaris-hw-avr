#ifndef PINS_H
#define PINS_H

#include "stdint.h"
#include "avr/io.h"
#include "util/delay.h"

#define INPUT 0x00
#define INPUT_PULLUP 0x02
#define OUTPUT 0x01
#define TRI_STATE 0x03

#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif

#define MOSI 3
#define MISO 4
#define SCK 5

#define RX 15
#define TX 16
#define D2 17
#define D3 18
#define D4 19
#define D5 20
#define D6 21
#define D7 22
#define D8 0
#define D9 1
#define D10 2
#define A0 8
#define A1 9
#define A2 10
#define A3 11
#define A4 12
#define A5 13


void pinMode(uint8_t pin, uint8_t mode);

void digitalWrite(uint8_t port_o, uint8_t level);

uint8_t digitalRead(uint8_t port_o);

uint8_t digitalReadAndShift(uint8_t pin);

long analogRead(uint8_t pin);

void badPin(uint8_t pin);

struct port_map_t {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint8_t bit;
};

const static port_map_t pin_map[] = {
        {&DDRB, &PORTB, &PINB, 0}, //D8
        {&DDRB, &PORTB, &PINB, 1}, //D9
        {&DDRB, &PORTB, &PINB, 2}, //D10
        {&DDRB, &PORTB, &PINB, 3}, //D11 MOSI
        {&DDRB, &PORTB, &PINB, 4}, //D12 MISO
        {&DDRB, &PORTB, &PINB, 5}, //D13 SCK
        {&DDRB, &PORTB, &PINB, 6}, //XTAL1
        {&DDRB, &PORTB, &PINB, 7}, //XTAL2
        {&DDRC, &PORTC, &PINC, 0}, //A0
        {&DDRC, &PORTC, &PINC, 1}, //A1
        {&DDRC, &PORTC, &PINC, 2}, //A2
        {&DDRC, &PORTC, &PINC, 3}, //A3
        {&DDRC, &PORTC, &PINC, 4}, //A4
        {&DDRC, &PORTC, &PINC, 5}, //A5
        {&DDRC, &PORTC, &PINC, 6}, //RST
        {&DDRD, &PORTD, &PIND, 0}, //D0 (RX)
        {&DDRD, &PORTD, &PIND, 1}, //D1 (TX)
        {&DDRD, &PORTD, &PIND, 2}, //D2
        {&DDRD, &PORTD, &PIND, 3}, //D3
        {&DDRD, &PORTD, &PIND, 4}, //D4
        {&DDRD, &PORTD, &PIND, 5}, //D5
        {&DDRD, &PORTD, &PIND, 6}, //D6
        {&DDRD, &PORTD, &PIND, 7}, //D7
};


#endif