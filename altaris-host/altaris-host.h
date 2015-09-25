//
// Created by Denis Bilyk on 9/4/15.
//

#ifndef _ALTARIS_HOST_H
#define _ALTARIS_HOST_H

#include "avr/interrupt.h"
#include "RF24.h"
#include "uart.h"
#include "softuart.h"
#include "pins.h"
#include "util/delay.h"
#include "WString.h"
#include "avr/eeprom.h"

void setup(void);

void loop(void);

void isConfigMode();

void esp_init();

void readEepromData() ;

//int serial_putc(char c, FILE *);
#endif //_ALTARIS_HOST_H
