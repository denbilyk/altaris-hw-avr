//
// Created by Denis Bilyk on 10/20/15.
//

#include "stdint.h"
#include "pins.h"
#include "altaris-module-core.h"

uint8_t ce_pin = D9;
uint8_t csn_pin = D10;
uint8_t ds1820_pin = D6;
uint8_t ds1820_ctl_pin = D5;
uint8_t dht11_pin = A1;
uint8_t dht11_ctl_pin = D7;
uint8_t door_pin = D2;
uint8_t light_pin = A0;
uint8_t light_ctl_pin = D8;


uint64_t rx_address = 0x27272727E0;
uint64_t tx_address = 0x27272727E0;
const char *confID = "Configuration ID: 0xE0";
