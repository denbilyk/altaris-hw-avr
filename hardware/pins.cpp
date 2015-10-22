//
// Created by Denis Bilyk on 9/2/15.
//

#include "pins.h"

#define set_bit(reg, bit) reg |= (1 << bit)
#define clear_bit(reg, bit) reg &= ~(1 << bit)
#define check_bit(reg, bit) (reg & (1 << bit))

const uint8_t pinCount = sizeof(pin_map) / sizeof(port_map_t);


void pinMode(uint8_t pin, uint8_t mode) {
    badPin(pin);
    port_map_t port_o = pin_map[pin];

    if (mode == INPUT) {
        clear_bit(*port_o.ddr, port_o.bit);
    }
    else if (mode == OUTPUT) {
        set_bit(*port_o.ddr, port_o.bit);
    } else if (mode == INPUT_PULLUP) {
        clear_bit(*port_o.ddr, port_o.bit);
        set_bit(*port_o.port, port_o.bit);
    } else if (mode == TRI_STATE) {
        clear_bit(*port_o.ddr, port_o.bit);
        clear_bit(*port_o.port, port_o.bit);
    }
}

void digitalWrite(uint8_t pin, uint8_t level) {
    badPin(pin);
    port_map_t port_o = pin_map[pin];
    if (level) {
        set_bit(*port_o.port, port_o.bit);
    } else {
        clear_bit(*port_o.port, port_o.bit);
    }
}

uint8_t digitalRead(uint8_t pin) {
    badPin(pin);
    port_map_t port_o = pin_map[pin];
    return check_bit(*port_o.pin, port_o.bit);
}

uint8_t digitalReadAndShift(uint8_t pin) {
    badPin(pin);
    port_map_t potr_o = pin_map[pin];
    return check_bit(*potr_o.pin, potr_o.bit) >> potr_o.bit;

}

long analogRead(uint8_t pin) {
    badPin(pin);
    ADMUX = 0x00;
    ADCSRA = 0x00;

    switch (pin) {
        case A0:
            break;
        case A1:
            ADMUX |= (1 << MUX0);
            break;
        case A2:
            ADMUX |= (1 << MUX1);
            break;
        case A3:
            ADMUX |= (1 << MUX0) | (1 << MUX1);
            break;
        case A4:
            ADMUX |= (1 << MUX2);
            break;
        case A5:
            ADMUX |= (1 << MUX2) | (1 << MUX0);
            break;
        default:
            break;
    }

    /* Light sensor init */
    ADMUX |= (1 << REFS0);
    // pascale to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    _delay_ms(75);
    ADCSRA |= _BV(ADSC) | _BV(ADEN); // Start conversion
    while (ADCSRA & (1 << ADSC)); // measuring

    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    long result = (high << 8) | low;
    return result;
}

//TODO: Doesnt work
/*
void badPinNumber(void)
        __attribute__((error("Pin number is too large or not a constant")));
*/
void badPin(uint8_t pin) {
    if (!__builtin_constant_p(pin) || pin >= pinCount);
    //badPinNumber();
}

