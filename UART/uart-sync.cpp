//
// Created by Denis Bilyk on 8/23/15.
//
#include "uart-sync.h"

#define F_CPU 16000000UL  // 16 MHz

void UART::begin(int baudrate) {
    uart_sync_begin(baudrate);

}

const char *UART::readString() {
    return uart_sync_readString();
}

bool UART::available() {
    return uart_sync_available();
}

size_t UART::write(uint8_t u8_data) {
    uart_sync_putByte(u8_data);
    return 0;
}
