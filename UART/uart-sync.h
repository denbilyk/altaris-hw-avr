//
// Created by Denis Bilyk on 8/23/15.
//

#ifndef _UART_H
#define _UART_H

#include "stdint.h"
#include "print.h"
#include "uart-sync-native.h"
#include "hardware.h"


class UART : public Print {
protected:

    size_t write(uint8_t u8_data);

public:

    void begin(int baudrate);

    bool available();

    const char *readString();

};

#endif //_UART_H