//
// Created by Denis Bilyk on 8/22/15.
//


#ifndef _ALTARIS_MODULE_H
#define _ALTARIS_MODULE_H

#include "pins.h"
#include "avr/io.h"
#include "uart.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "wdt_power.h"
#include "RF24.h"

long readVcc();

void nrf() ;

void setup(void);

void loop(void);

#endif //_ALTARIS_MODULE_H