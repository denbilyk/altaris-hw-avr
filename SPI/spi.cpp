//
// Created by Denis Bilyk on 8/30/15.
//


#include "spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <wiring.h>
#include <pins.h>

#define PORT_SPI    PORTB
#define DDR_SPI     DDRB
#define DD_MISO     DDB4
#define DD_MOSI     DDB3
#define DD_SS       DDB2
#define DD_SCK      DDB5

uint8_t SPI::initialized = 0;

void SPI::spi_init()
// Initialize pins for spi communication
{
    DDR_SPI &= ~((1 << DD_MOSI) | (1 << DD_MISO) | (1 << DD_SS) | (1 << DD_SCK));
    // Define the following pins as output
    DDR_SPI |= ((1 << DD_MOSI) | (1 << DD_SS) | (1 << DD_SCK));

    SPCR = ((1 << SPE) |               // SPI Enable
            (0 << SPIE) |              // SPI Interupt Enable
            (0 << DORD) |              // Data Order (0:MSB first / 1:LSB first)
            (1 << MSTR) |              // Master/Slave select
            (0 << SPR1) | (1 << SPR0) |    // SPI Clock Rate
            (0 << CPOL) |              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0 << CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

    SPSR = (1 << SPI2X);              // Double Clock Rate
}

void SPI::spi_transfer_sync(uint8_t *dataout, uint8_t *datain, uint8_t len)
// Shift full array through target device
{
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while ((SPSR & (1 << SPIF)) == 0);
        datain[i] = SPDR;
    }
}

void SPI::spi_transmit_sync(uint8_t *dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while ((SPSR & (1 << SPIF)) == 0);
    }
}

uint8_t SPI::spi_fast_shift(uint8_t data)
// Clocks only one byte to target device and returns the received one
{
    SPDR = data;
    while ((SPSR & (1 << SPIF)) == 0);
    return SPDR;
}

void SPI::begin()
{
    uint8_t sreg = SREG;
    cli(); // Protect from a scheduler and prevent transactionBegin
    if (!initialized) {
        // Set SS to high so a connected chip will be "deselected" by default
        digitalWrite(SS, HIGH);

        // When the SS pin is set as OUTPUT, it can be used as
        // a general purpose output port (it doesn't influence
        // SPI operations).
        pinMode(SS, OUTPUT);

        // Warning: if the SS pin ever becomes a LOW INPUT then SPI
        // automatically switches to Slave, so the data direction of
        // the SS pin MUST be kept as OUTPUT.
        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);

        // Set direction register for SCK and MOSI pin.
        // MISO pin automatically overrides to INPUT.
        // By doing this AFTER enabling SPI, we avoid accidentally
        // clocking in a single bit since the lines go directly
        // from "input" to SPI control.
        // http://code.google.com/p/arduino/issues/detail?id=888
        pinMode(SCK, OUTPUT);
        pinMode(MOSI, OUTPUT);
    }
    initialized++; // reference count
    SREG = sreg;
}

void SPI::setBitOrder(uint8_t bitOrder) {
    if (bitOrder == LSBFIRST) SPCR |= _BV(DORD);
    else
        SPCR &= ~(_BV(DORD));
}

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
void SPI::setDataMode(uint8_t dataMode) {
    SPCR = (SPCR & ~SPI_MODE_MASK) | dataMode;
}

// This function is deprecated.  New applications should use
// beginTransaction() to configure SPI settings.
void SPI::setClockDivider(uint8_t clockDiv) {
    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clockDiv & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clockDiv >> 2) & SPI_2XCLOCK_MASK);
}

// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPI::transfer(uint8_t data) {
    SPDR = data;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))); // wait
    return SPDR;
}

