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

uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};
uint8_t rx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

SUART esp(SUART_TX, SUART_RX);
UART uart;

const String s = "data_array[{0}] = {1}";
bool is_esp_init = false;
String ssid = "";
String ssid_pass = "";
String auth = "";

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

    pinMode(CONFIGURATOR_EXISTS, INPUT_PULLUP);

    readEepromData();

}

void loop(void) {
    if (!is_esp_init) {
        // esp_init();
        uart.println("ESP Failed!");
    }

    if (nrf24_dataReady()) {
        nrf24_getData(data_array);
        uart.println("> ");
        for (uint8_t i = 0; i < 4; i++) {
            String tmp = String(s);
            tmp.replace("{0}", String(i));
            tmp.replace("{1}", String(data_array[i]));
            uart.println(tmp);
        }
    }

    isConfigMode();
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

void esp_init() {
    esp.println("AT+RST");
    _delay_ms(200);
    uart.println("AT+RST...OK");

    esp.println("AT");
    _delay_ms(300);
    String resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT failed");
        is_esp_init = false;
    }
    uart.println("AT...OK");

    esp.println("AT+CWMODE=1");
    _delay_ms(300);
    resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT+CWMODE failed");
        is_esp_init = false;
    }
    uart.println("AT+CWMODE=1...OK");

    esp.println("AT+CWJAP=\"" + ssid + "\",\"" + ssid_pass + "\"");
    _delay_ms(7000);
    resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT+CWJAP failed");
        is_esp_init = false;
    }
    uart.println("AT+CWJAP...OK");

    esp.println("AT+CIPSTA?");
    _delay_ms(1000);
    uart.println("AT+CIPSTA?...OK");
    resp = esp.readString();
    uart.println(resp);
    esp.println("AT+CIPMUX=0");
    _delay_ms(300);
    resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT+CIPMUX failed");
        is_esp_init = false;
    }
    uart.println("AT+CIPMUX=0...OK");

    esp.println("AT+CWMODE=1");
    _delay_ms(300);
    resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT+CWMODE failed");
        is_esp_init = false;
    }
    uart.println("AT+CWMODE=1...OK");

    esp.println("AT+CIPMUX=1");
    _delay_ms(300);
    resp = esp.readString();
    if (resp.indexOf("ERROR") != -1) {
        uart.println("ESP command AT+CIPMUX failed");
        is_esp_init = false;
    }
    uart.println("AT+CIPMUX=1...OK");
    is_esp_init = true;
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

void isConfigMode() {
    if (digitalReadAndShift(CONFIGURATOR_EXISTS) == 0) {
        _delay_ms(500);
        while (digitalReadAndShift(CONFIGURATOR_EXISTS) == 0) {
            if (esp.available()) {
                uart.print(esp.readString());
            }
            if (uart.available()) {
                _delay_ms(500);
                String response = uart.readString();
                uart.println(response);
                String cmd = response.substring(0, 2);
                String data = response.substring(3);
                if (cmd.equals("0A")) esp.print(data);
                if (cmd.equals("0B")) writeAuth(data);
                if (cmd.equals("0S")) writeSSID(data);
                if (cmd.equals("0P")) writeSsidPassword(data);
            }
        }
    }
}

int main(void) {
    setup();
    sei();
    do {
        loop();
    } while (true);
}