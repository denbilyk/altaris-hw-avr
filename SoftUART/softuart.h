//
// Created by Denis Bilyk on 9/25/15.
//

#ifndef SOFTUART_H
#define SOFTUART_H

#include "print.h"
#include "softuart_native.h"

class SUART: public Print {

protected:

    size_t write(uint8_t u8_data);

public:

    bool available();

    const char *readString();

    SUART(uint8_t tx_pin, uint8_t rx_pin);

};

#endif //SOFTUART_H
