//
// Created by Denis Bilyk on 9/4/15.
//


#include "altaris-host.h"

#define DEBUG

#define CE D9
#define CSN D10
#define SUART_TX D3
#define SUART_RX D2
#define CONFIGURATOR_EXISTS D8
#define PACKET_LENGTH 7

#define RX_ADDR_0 0x27272727E0
#define RX_ADDR_1 0xE7E7E7E7E1


uint8_t data_array[7];
uint8_t rx_address_0[5] = {0x27, 0x27, 0x27, 0x27, 0xE0};
uint8_t rx_address_1[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE1};


ESP esp(SUART_TX, SUART_RX);
UART uart;
RF24 rf24(CE, CSN);

const String s = "data_array[{0}] = {1}";
bool is_esp_init = false;
String ssid = "";
String ssid_pass = "";
String auth = "";

void setup(void) {

    uart.begin(9600);
    uart.println("Init UART...OK");

    uart.println("Init NRF24...");
    //nrf24_init(CE, CSN);
    rf24.begin();
    rf24.setRetries(15,15);
    rf24.setAutoAck(true);
    rf24.setCRCLength(RF24_CRC_8);
    rf24.setDataRate(RF24_250KBPS);

    rf24.openReadingPipe(0, RX_ADDR_0);
    rf24.openReadingPipe(1, RX_ADDR_1);
    rf24.startListening();
    rf24.printDetails();

    /* Channel #2 , payload length: 4 */
    //nrf24_config(2, PACKET_LENGTH);

    /* Set the device addresses */
    //nrf24_tx_address(tx_address);
    //nrf24_rx_address_0(rx_address_0);
    //nrf24_rx_address_1(rx_address_1);
    //nrf24_writeRegister(RX_ADDR_P0, rx_address_0, nrf24_ADDR_LEN);
    //nrf24_writeRegister(RX_ADDR_P1, rx_address_1, nrf24_ADDR_LEN);

    //pinMode(CONFIGURATOR_EXISTS, INPUT_PULLUP);

    readEepromData();

}

void loop(void) {
    if (!is_esp_init) {
        //is_esp_init = esp.esp_init("infinity", "0672086028");
    }

    if (rf24.available()) {
        rf24.read(&data_array, PACKET_LENGTH);
        uart.println("> ");
        for (uint8_t i = 0; i < PACKET_LENGTH; i++) {
            String tmp = String(s);
            tmp.replace("{0}", String(i));
            tmp.replace("{1}", String(data_array[i]));
            uart.println(tmp);
        }
        //esp.send_data(auth, String(data_array[0]), String(data_array[1]), String(data_array[2]));
    }

    //isConfigMode();
    uart.println("ACK");
    _delay_ms(1000);
}


void readEepromData() {
    unsigned char buff[32];
    eeprom_read_block(&buff, (const void *) 10, 32);
    ssid = String((const char *) buff);
    uart.println("SSID: " + ssid);
    eeprom_read_block(&buff, (const void *) 50, 32);
    ssid_pass = String((const char *) buff);
    uart.println("SSID PASS: " + ssid_pass);
    uint32_t res = eeprom_read_dword((const uint32_t *) 0);
    auth = String(res);
    uart.println("AUTH: " + auth);
}


void writeAuth(String data) {
    uint32_t auth = (uint32_t) data.toInt();
    uart.println(auth);
    eeprom_write_dword((uint32_t *) 0, auth);
}

void writeSSID(String data) {
    unsigned char *p_buff;
    unsigned char buff[32];
    p_buff = buff;
    data.getBytes(p_buff, data.length(), 0);
    eeprom_write_block(&buff, (void *) 10, 32);

    /* do {
          uart.println(*p_buff);
      } while (*p_buff++ != 0);*/
}

void writeSsidPassword(String data) {
    unsigned char *p_buff;
    unsigned char buff[32];
    p_buff = buff;
    data.getBytes(p_buff, data.length(), 0);
    eeprom_write_block(&buff, (void *) 50, 32);

}

/*void isConfigMode() {
    if (digitalReadAndShift(CONFIGURATOR_EXISTS) == 0) {
        _delay_ms(500);
        while (digitalReadAndShift(CONFIGURATOR_EXISTS) == 0) {
            if (esp.available()) {
                uart.print(esp.readString());
            }
            if (uart.available()) {
                String response = uart.readString();
                uart.print("Res - ");
                uart.println(response);
                String cmd = response.substring(0, 2);
                String data = response.substring(3);
                uart.println(cmd);
                uart.println(data);
                if (cmd.equals("0A")) esp.print(data);
                if (cmd.equals("0B")) writeAuth(data);
                if (cmd.equals("0S")) writeSSID(data);
                if (cmd.equals("0P")) writeSsidPassword(data);
            }
        }
    }
}*/

int main(void) {
    //cli();
    //init_hardware();
    setup();
    sei();
    do {
        loop();
    } while (true);
}