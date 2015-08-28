//
// Created by Denis Bilyk on 8/22/15.
//

#include "altaris-module.h"

#define DEBUG
#define SERIAL_DELAY delay(300)

#define CSN_PIN 10
#define CE_PIN 9
#define WR_PIPE_ADDR 0xE0
#define PIPE_BASE(MSB) 0xF0F0F0F000LL + MSB
#define WR_PIPE PIPE_BASE(WR_PIPE_ADDR)
#define RD_PIPE PIPE_BASE(RD_PIPE_ADDR)
#define RD_PIPE_ADDR 0xA0 //160


UART uart;

int main(void) {

    setup();

    do {
        loop();
    } while (true);
}

void setup(void) {
    uart.begin(9600);
}

void loop(void) {
    //uart.println("Test-Test");

}
