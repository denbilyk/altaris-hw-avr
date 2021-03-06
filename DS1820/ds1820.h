/*	Library to use ds18x20 with ATMEL Atmega family.
	For short ds18x20 wires there is no need for an external pullup resistor.
	If the wire length exceeds one meter you should use a 4.7k pullup resistor
	on the data line. This library does not work for parasite power.
	You can just use one ds18x20 per Atmega Pin.

	Copyright (C) 2010 Stefan Sicklinger

	For support check out http://www.sicklinger.com

	This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef _DS1820_H
#define _DS1820_H

#include "avr/io.h"
#include "math.h"
#include "util/delay.h"
#include "pins.h"

#define TEMP_LSB        0
#define TEMP_MSB        1

//-----------------------------------------
// Prototypes
//-----------------------------------------
uint8_t ds1820_reset(uint8_t);

void ds1820_wr_bit(uint8_t, uint8_t);

uint8_t ds1820_re_bit(uint8_t);

uint8_t ds1820_re_byte(uint8_t);

void ds1820_wr_byte(uint8_t, uint8_t);

int16_t ds1820_read_temp(uint8_t);

void ds1820_init(uint8_t);

float rawToCelsius(int16_t raw);

#endif //_DS1820_H
