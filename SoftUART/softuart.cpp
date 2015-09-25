//
// Created by Denis Bilyk on 9/25/15.
//

#include "softuart.h"


SUART::SUART(uint8_t tx_pin, uint8_t rx_pin) {
    init(tx_pin, rx_pin);
}

size_t SUART::write(uint8_t u8_data) {
    putc(u8_data);
    return 0;
}

bool SUART::available() {
    return kbhit();
}

const char *SUART::readString() {
    static char rxstr[SOFTUART_IN_BUF_SIZE];
    static char *temp;
    memset(rxstr, 0, SOFTUART_IN_BUF_SIZE);
    temp = rxstr;
    uint8_t len = rx_length();
    uint8_t c = 0;
    while (c < len) {
        *temp = getc();
        ++temp;
        len--;
    }
    flush_input_buffer();
    *temp = '\r';
    *temp = '\n';
    return rxstr;
}
