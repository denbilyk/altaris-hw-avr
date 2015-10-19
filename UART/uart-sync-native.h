//
// Created by Denis Bilyk on 8/23/15.
//

#ifndef _UART_SYNC_H
#define _UART_SYNC_H

#include "stdint.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "print.h"
#include "hardware.h"

#define UART_RX0_BUFFER_SIZE 128

char uart_sync_readByte();

void uart_sync_putByte(uint8_t data);

void uart_sync_begin(int baundrate);

const char *uart_sync_readString(void);

void uart_sync_writeString(char *str);

bool uart_sync_available();

#endif //_UART_SYNC_H