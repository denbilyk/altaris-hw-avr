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

//TODO: Doesnt work
void badPin(uint8_t pin) {
    if (!__builtin_constant_p(pin) || pin >= pinCount);

}
