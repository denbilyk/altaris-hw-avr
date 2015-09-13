/*
DHT11 Library 0x01

copyright (c) Davide Gironi, 2011

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef _DHT11_H
#define _DHT11_H

#include "string.h"
#include "avr/io.h"
#include "util/delay.h"
#include "pins.h"


#define DHT11 A0

extern uint8_t dht11_getTemp();
extern uint8_t dht11_getHumid();



#endif //_DHT11_H
