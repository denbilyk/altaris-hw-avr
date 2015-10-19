//
// Created by Denis Bilyk on 8/23/15.
//
#include "uart-sync.h"

#define F_CPU 16000000UL  // 16 MHz
#define RX_BUFF 30

void uart_sync_begin(int baudrate) {

    uint8_t baud_prescale = (uint8_t) ((F_CPU / (baudrate * 16UL)) - 1);
    // Load lower 8-bits into the low byte of the UBRR register
    UBRR0L = baud_prescale;
    /* Load upper 8-bits into the high byte of the UBRR register
     * Default frame format is 8 data bits, no parity, 1 stop bit
     * to change use UCSRC, see AVR datasheet*/
    UBRR0H = (baud_prescale >> 8);
    UCSR0B = ((1 << TXEN0) | (1 << RXEN0));

}


/*size_t UART::write(uint8_t u8_data) {
    while ((UCSR0A & (1 << UDRE0)) == 0);
    UDR0 = u8_data;
    return 0;
}*/

const char *uart_sync_readString(void) {
    static char rxstr[UART_RX0_BUFFER_SIZE];
    static char *temp;
    temp = rxstr;

    while ((*temp = uart_sync_readByte()) != '\n') {
        ++temp;
    }
    return rxstr;
}

char uart_sync_readByte() {
    while (!(UCSR0A & _BV(RXC0)));
    return (char) UDR0;
}


void uart_sync_putByte(uint8_t data) {
    while (!(UCSR0A & _BV(UDRE0)));
    UDR0 = data;

}

void uart_sync_writeString(char *str) {
    while (*str != '\0') {
        uart_sync_putByte(*str);
        ++str;
    }
}

bool uart_sync_available() {
    return (UCSR0A & _BV(RXC0)) > 0;
}

