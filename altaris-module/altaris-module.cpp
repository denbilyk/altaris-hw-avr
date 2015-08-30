//
// Created by Denis Bilyk on 8/22/15.
//

#include "altaris-module.h"

#define DEBUG


#define CSN_PIN 10
#define CE_PIN 9
#define WR_PIPE_ADDR 0xE0
#define PIPE_BASE(MSB) 0xF0F0F0F000LL + MSB
#define WR_PIPE PIPE_BASE(WR_PIPE_ADDR)
#define RD_PIPE PIPE_BASE(RD_PIPE_ADDR)
#define RD_PIPE_ADDR 0xA0 //160


UART uart;
WDT_POWER wdtPower;
//RF24 radio(CE_PIN, CSN_PIN);

void setup(void) {
    uart.begin(9600);
    uart.println("Init UART....");
    uart.println("Init WDT....");
    wdtPower.wdtInit();
    uart.println("Init NRF24....");
    /*radio.begin();
    radio.setRetries(15, 15);
    radio.setAutoAck(true);
    radio.setDataRate(RF24_250KBPS);
    radio.setCRCLength(RF24_CRC_8);
    radio.openWritingPipe(WR_PIPE);
    radio.openReadingPipe(1, RD_PIPE);
    radio.startListening();
    radio.printDetails();*/
    _delay_ms(200);

}

void loop(void) {
    uart.println("Available....");
    _delay_ms(100);
    wdtPower.sleep_for(&uart, 24);
}


int main(void) {
    setup();
    sei();
    do {
        loop();
    } while (true);
}







