//
// Created by Denis Bilyk on 9/4/15.
//

#ifndef _ALTARIS_HOST_H
#define _ALTARIS_HOST_H

#include "avr/interrupt.h"
#include "RF24.h"
#include "uart.h"
#include "esp-network.h"
#include "util/delay.h"
#include "WString.h"
#include "avr/eeprom.h"
#include "pins.h"

void setup(void);

void loop(void);

void isConfigMode();

void readEepromData() ;

#endif //_ALTARIS_HOST_H
