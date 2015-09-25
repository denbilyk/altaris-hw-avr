#ifndef _SOFTUART_H
#define _SOFTUART_H

#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/pgmspace.h"
#include "pins.h"

#if !defined(F_CPU)
#warning "F_CPU not defined in makefile - now defined in softuart.h"
#define F_CPU 16000000UL
#endif

#define SOFTUART_BAUD_RATE      9600

#if defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)  \
 || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
 || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PA__) \
 || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)


#define SOFTUART_T_COMP_LABEL      TIMER0_COMPA_vect
#define SOFTUART_T_COMP_REG        OCR0A
#define SOFTUART_T_CONTR_REGA      TCCR0A
#define SOFTUART_T_CONTR_REGB      TCCR0B
#define SOFTUART_T_CNT_REG         TCNT0
#define SOFTUART_T_INTCTL_REG      TIMSK0
#define SOFTUART_CMPINT_EN_MASK    (1 << OCIE0A)
#define SOFTUART_CTC_MASKA         (1 << WGM01)
#define SOFTUART_CTC_MASKB         (0)

/* "A timer interrupt must be set to interrupt at three times
   the required baud rate." */
#define SOFTUART_PRESCALE (8)
// #define SOFTUART_PRESCALE (1)

#if (SOFTUART_PRESCALE == 8)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS01)
#elif (SOFTUART_PRESCALE == 1)
#define SOFTUART_PRESC_MASKA         (0)
#define SOFTUART_PRESC_MASKB         (1 << CS00)
#else
#error "prescale unsupported"
#endif
#else
#error "no defintions available for this AVR"
#endif

#define SOFTUART_TIMERTOP ( F_CPU/SOFTUART_PRESCALE/SOFTUART_BAUD_RATE/3 - 1)

#if (SOFTUART_TIMERTOP > 0xff)
#warning "Check SOFTUART_TIMERTOP: increase prescaler, lower F_CPU or use a 16 bit timer"
#endif

#define SOFTUART_IN_BUF_SIZE     32

    uint8_t rx_length();

// Init the Software Uart
    void init(uint8_t tx_pin, uint8_t rx_pin);

// Clears the contents of the input buffer.
    void flush_input_buffer(void);

// Tests whether an input character has been received.
    bool kbhit(void);

// Reads a character from the input buffer, waiting if necessary.
    char getc(void);

// To check if transmitter is busy
    unsigned char transmit_busy(void);

// Writes a character to the serial port.
    void putc(const char ch);

// Turns on the receive function.
    void turn_rx_on(void);

// Turns off the receive function.
    void turn_rx_off(void);

// Write a NULL-terminated string from RAM to the serial port
    void puts(const char *s);

// Write a NULL-terminated string from program-space (flash)
// to the serial port. example: puts_p(PSTR("test"))
    void puts_p(const char *prg_s);

// Helper-Macro - "automatically" inserts PSTR
// when used: include avr/pgmspace.h before this include-file
#define softuart_puts_P(s___) softuart_puts_p(PSTR(s___))


#endif //_SOFTUART_H
