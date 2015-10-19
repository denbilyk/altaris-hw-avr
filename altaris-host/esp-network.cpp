//
// Created by Denis Bilyk on 10/1/15.
//

#include "esp-network.h"

SUART suart;

extern UART uart;

ESP::ESP(uint8_t tx, uint8_t rx) {
    suart.begin(tx, rx);
}

bool ESP::send_AT() {
    return cmd(ESP_AT, 300);
}

bool ESP::send_RST() {
    return cmd(ESP_RST, 300);
}

bool ESP::send_CWMODE() {
    return cmd(ESP_CWMODE, 300);
}

bool ESP::send_CWJAP(String ssid, String pass) {
    String command = String(ESP_CWJAP);
    command.replace("{0}", ssid);
    command.replace("{1}", pass);
    return cmd(command, 7000);
}

String ESP::send_CIPSTA() {
    return result(ESP_CIPSTA, 1000);
}

bool ESP::send_CIPMUX(uint8_t value) {
    return value == 0 ? cmd(ESP_CIPMUX_0, 300) : cmd(ESP_CIPMUX_1, 300);
}

bool ESP::esp_init(String ssid, String ssid_pass) {
    uart.println("Start init ESP...");
    if (!send_RST()) {
        uart.println("RST - Failed");
        return false;
    }
    if (!send_AT()) {
        uart.println("AT - Failed");
        return false;
    }
    if (!send_CWMODE()) {
        uart.println("CWMODE - Failed");
        return false;
    }
    if (!send_CWJAP(ssid, ssid_pass)) {
        uart.println("CWJAP - Failed");
        return false;
    }
    String s = send_CIPSTA();
    uart.println(s);
    if (!send_CIPMUX(0)) {
        uart.println("CIPMUX=0 = Failed");
        return false;
    }
    if (!send_CWMODE()) {
        uart.println("CWMODE - Last Failed");
        return false;
    }
    if (!send_CIPMUX(1)) {
        uart.println("CIPMUX=1 = Failed");
        return false;
    }
    uart.println("ESP init complete");
    return true;
}

void delay_ms(uint16_t count) {
    while (count--) {
        _delay_ms(1);

    }
}

bool connect_to_host() {
    uint8_t counter = 10;
    bool is_connected = false;
    while (counter > 0) {
        --counter;
        suart.println(CMD_TCP_HOST);
        delay_ms(300);
        const char *resp = suart.readString();
        if (strstr(resp, "CONNECT") != 0) {
            uart.println("Connected!");
            is_connected = true;
            break;
        } else {
            uart.print("Err Res - ");
            uart.println(resp);
        }

        delay_ms(1000);
    }
    return is_connected;
}

bool sendTcp(String tcp_packet) {
    bool res = false;
    if (connect_to_host()) {
        uart.print("Tcp paket - ");
        uart.println(tcp_packet);

        String cmd_send = String(ESP_CIPSEND);
        cmd_send.replace("{0}", String(tcp_packet.length()));
        suart.println(cmd_send);
        delay_ms(300);
        String resp = suart.readString();
        if (resp.indexOf(">") != -1) {
            suart.print(tcp_packet);
            uart.println("Packet sent");
            res = true;
        } else {
            delay_ms(500);
            suart.println("AT+CIPCLOSE=0");
            uart.print("Close - ");
            uart.println(suart.readString());
        }
    }
    return res;
}


bool ESP::send_data(String auth, String id, String temp, String humid) {
    String packet = String(GET_REQUEST);
    packet.replace("{0}", auth);
    packet.replace("{1}", id);
    packet.replace("{2}", temp);
    packet.replace("{3}", humid);
    return sendTcp(packet);
}


bool ESP::cmd(String cmd, uint16_t delay) {
    suart.println(cmd);
    delay_ms(delay);
    String resp = suart.readString();
    return resp.indexOf(ESP_RESP_ERROR) == -1;
}

String ESP::result(String cmd, uint16_t delay) {
    suart.println(cmd);
    delay_ms(delay);
    return suart.readString();
}
