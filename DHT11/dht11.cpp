//
// Created by Denis Bilyk on 9/10/15.
//


#include "dht11.h"


#define DHT11_ERROR false

/*
 * buffer[0] = get temperature (0..50C)
 * buffer[1] = get humidity (20..90%)
 * @return true - read OK
 */
bool dht11_read_data(uint8_t used_pin, uint8_t *buffer) {
    uint8_t bits[5];
    uint8_t i, j = 0;

    memset(bits, 0, sizeof(bits));

    //reset port
    pinMode(used_pin, OUTPUT);
    digitalWrite(used_pin, HIGH);
    _delay_ms(100);

    //send request
    digitalWrite(used_pin, LOW);
    _delay_ms(18);
    digitalWrite(used_pin, HIGH);
    _delay_us(1);
    pinMode(used_pin, INPUT);
    _delay_us(39);

    //check start condition 1
    if (digitalRead(used_pin))
        return DHT11_ERROR;


    _delay_us(80);
    //check start condition 2
    if (!digitalRead(used_pin))
        return DHT11_ERROR;
    _delay_us(80);

    //read the data
    for (j = 0; j < 5; j++) { //read 5 byte
        uint8_t result = 0;
        for (i = 0; i < 8; i++) {//read every bit
            while (!digitalRead(used_pin));
            _delay_us(30);
            if (digitalRead(used_pin))
                result |= (1 << (7 - i));
            while (digitalRead(used_pin));
        }
        bits[j] = result;
    }

    //reset port
    pinMode(used_pin, OUTPUT);
    digitalWrite(used_pin, HIGH);
    _delay_ms(100);

    //check checksum
    if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
        //temperature
        buffer[0] = bits[2];
        //humidity
        buffer[1] = bits[0];
        return true;
    }
    return DHT11_ERROR;
}



