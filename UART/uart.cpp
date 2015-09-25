//
// Created by Denis Bilyk on 8/23/15.
//
#include "uart.h"

#define F_CPU 16000000UL  // 16 MHz

void UART::begin(int baudrate) {
    uart_init(UART_BAUD_SELECT(baudrate, F_CPU));
}

const char *UART::readString(void) {
    static char rxstr[UART_RX0_BUFFER_SIZE];
    static char *temp;
    memset(rxstr, 0, UART_RX0_BUFFER_SIZE);
    temp = rxstr;
    uint8_t len = uart_available();
    uint8_t c = 0;
    while (c < len) {
        *temp = uart_getc();
        ++temp;
        len--;
    }
    return rxstr;
}

bool UART::available() {
    return uart_available() > 0;
}

size_t UART::write(uint8_t u8_data) {
    uart_putc(u8_data);
    return 0;
}
