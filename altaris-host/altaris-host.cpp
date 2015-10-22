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

#define RX_ADDR_0 0x27272727E0
#define RX_ADDR_1 0xE7E7E7E7E1
#define RX_ADDR_2 0xE7E7E7E7E2
#define RX_ADDR_3 0xE7E7E7E7E3
#define RX_ADDR_4 0xE7E7E7E7E4
#define RX_ADDR_5 0xE7E7E7E7E5


uint16_t received_data[6];

ESP esp(SUART_TX, SUART_RX);
UART uart;
RF24 rf24(CE, CSN);

const String s = "received_data[{0}] = {1}";
bool is_esp_init = false;
String ssid = "";
String ssid_pass = "";
String auth = "";

void setup(void) {

    uart.begin(9600);
    uart.println("Init UART...OK");

    uart.println("Init NRF24...");
    rf24.begin();
    rf24.setRetries(15, 15);
    rf24.setAutoAck(true);
    rf24.setCRCLength(RF24_CRC_8);
    rf24.setDataRate(RF24_250KBPS);

    rf24.openReadingPipe(0, RX_ADDR_0);
    rf24.openReadingPipe(1, RX_ADDR_1);
    rf24.openReadingPipe(2, RX_ADDR_2);
    rf24.openReadingPipe(3, RX_ADDR_3);
    rf24.openReadingPipe(4, RX_ADDR_4);
    rf24.openReadingPipe(5, RX_ADDR_5);
    rf24.startListening();
    rf24.printDetails();

    //pinMode(CONFIGURATOR_EXISTS, INPUT_PULLUP);

    readEepromData();

}

void loop(void) {
    if (!is_esp_init) {
        //is_esp_init = esp.esp_init("infinity", "0672086028");
    }

    if (rf24.available()) {
        bool done = false;
        while (!done) {
            done = rf24.read(&received_data, sizeof(received_data));
        }
        uart.println("> ");
        for (uint8_t i = 0; i < 6; i++) {
            String tmp = String(s);
            tmp.replace("{0}", String(i));
            tmp.replace("{1}", String(received_data[i]));
            uart.println(tmp);
        }
        //esp.send_data(auth, String(received_data[0]), String(received_data[1]), String(received_data[2]));
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
    setup();
    sei();
    do {
        loop();
    } while (true);
}