//
// Created by Denis Bilyk on 8/23/15.
//

#ifndef _UART_H
#define _UART_H

#include "stdint.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "print.h"


class UART : public Print {

public:

    void begin(int baundrate);

    size_t write(uint8_t u8_data);

    uint8_t receive();
};

#endif //_UART_H