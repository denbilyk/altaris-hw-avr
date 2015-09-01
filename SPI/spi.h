//
// Created by Denis Bilyk on 8/30/15.
//

#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

class SPI {

private:
    static uint8_t initialized;

public:

    void spi_init();

    void spi_transfer_sync(uint8_t *dataout, uint8_t *datain, uint8_t len);

    void spi_transmit_sync(uint8_t *dataout, uint8_t len);

    uint8_t spi_fast_shift(uint8_t data);

    void setBitOrder(uint8_t bitOrder);

    void setDataMode(uint8_t dataMode);

    void setClockDivider(uint8_t clockDiv);

    uint8_t transfer(uint8_t data);

    void begin();
};

#endif //_SPI_H
