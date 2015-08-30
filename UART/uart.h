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

private:

    char readByte();

    void putByte(unsigned char data);


protected:

    size_t write(uint8_t u8_data);


public:

    void begin(int baundrate);

    const char *readString(void);

    void writeString(char *str);
};

#endif //_UART_H