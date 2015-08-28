//
// Created by Denis Bilyk on 8/23/15.
//
#include "uart.h"

//#define F_CPU 16000000UL  // 16 MHz

volatile unsigned char value;


void UART::begin(int baudrate) {

    uint8_t baud_prescale = (uint8_t) ((F_CPU / (baudrate * 16UL)) - 1);
    // Load lower 8-bits into the low byte of the UBRR register
    UBRR0L = baud_prescale;
    /* Load upper 8-bits into the high byte of the UBRR register
     * Default frame format is 8 data bits, no parity, 1 stop bit
     * to change use UCSRC, see AVR datasheet*/
    UBRR0H = (baud_prescale >> 8);
    UCSR0B = ((1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0));

}


size_t UART::write(uint8_t u8_data) {
    while ((UCSR0A & (1 << UDRE0)) == 0);
    UDR0 = u8_data;
    return 0;
}

uint8_t UART::receive() {
    while ((UCSR0A & (1 << RXC0)) == 0);
    return UDR0;
}


ISR(USART_RX_vect) {
    value = UDR0;  //read UART register into value
}

