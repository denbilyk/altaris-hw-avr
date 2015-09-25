//
// Created by Denis Bilyk on 9/4/15.
//


#include "altaris-host.h"

#define DEBUG

#define CE D9
#define CSN D10
#define SUART_TX D8
#define SUART_RX D7

uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};
uint8_t rx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

SUART suart(SUART_TX, SUART_RX);
UART uart;


void setup(void) {

    uart.begin(9600);
    uart.println("Init UART...OK");

    uart.println("Init NRF24...");
    nrf24_init(CE, CSN);

    /* Channel #2 , payload length: 4 */
    nrf24_config(2, 4);

    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);

}

void loop(void) {
    if (nrf24_dataReady()) {
        nrf24_getData(data_array);
        uart.println("> ");
        for (uint8_t i = 0; i < 4; i++) {
            uart.print("data_array[");
            uart.print(i);
            uart.print("] = ");
            uart.println(data_array[i]);
        }
    }

    if (suart.available()) {
        uart.print(suart.readString());
    }

    if (uart.available()) {
        suart.println(uart.readString());
    }

    uart.println("ACK");
    _delay_ms(1000);

}

int main(void) {
    setup();
    sei();
    do {
        loop();
    } while (true);
}