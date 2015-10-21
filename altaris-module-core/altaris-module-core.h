//
// Created by Denis Bilyk on 8/22/15.
//


#ifndef _ALTARIS_MODULE_H
#define _ALTARIS_MODULE_H

#include "pins.h"
#include "avr/io.h"
#include "uart-async.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "wdt_power.h"
#include "RF24-arduino.h"
#include "dht11.h"
#include "ds1820.h"

long readVcc();

void nrf(uint8_t *data_array) ;

void setup(void);

void loop(void);

#endif //_ALTARIS_MODULE_H