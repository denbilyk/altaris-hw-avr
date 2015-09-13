//
// Created by Denis Bilyk on 9/10/15.
//


#include "dht11.h"


#define DHT11_ERROR 255

/*
 * get data from dht11
 */
uint8_t dht11_getdata(uint8_t select) {
    uint8_t bits[5];
    uint8_t i, j = 0;

    memset(bits, 0, sizeof(bits));

    //reset port
    pinMode(DHT11, OUTPUT);
    digitalWrite(DHT11, HIGH);
    _delay_ms(100);

    //send request
    digitalWrite(DHT11, LOW);
    _delay_ms(18);
    digitalWrite(DHT11, HIGH);
    _delay_us(1);
    pinMode(DHT11, INPUT);
    _delay_us(39);

    //check start condition 1
    if (digitalRead(DHT11))
        return DHT11_ERROR;


    _delay_us(80);
    //check start condition 2
    if (!digitalRead(DHT11))
        return DHT11_ERROR;
    _delay_us(80);

    //read the data
    for (j = 0; j < 5; j++) { //read 5 byte
        uint8_t result = 0;
        for (i = 0; i < 8; i++) {//read every bit
            while (!digitalRead(DHT11));
            _delay_us(30);
            if (digitalRead(DHT11))
                result |= (1 << (7 - i));
            while (digitalRead(DHT11));
        }
        bits[j] = result;
    }

    //reset port
    pinMode(DHT11, OUTPUT);
    digitalWrite(DHT11, HIGH);
    _delay_ms(100);

    //check checksum
    if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
        if (select == 0) { //return temperature
            return (bits[2]);
        } else if (select == 1) { //return humidity
            return (bits[0]);
        }
    }

    return DHT11_ERROR;
}

/*
 * get temperature (0..50C)
 */
uint8_t dht11_getTemp() {
    uint8_t ret = dht11_getdata(0);
    if (ret == DHT11_ERROR)
        return DHT11_ERROR;
    else
        return ret;
}

/*
 * get humidity (20..90%)
 */
uint8_t dht11_getHumid() {
    uint8_t ret = dht11_getdata(1);
    if (ret == DHT11_ERROR)
        return DHT11_ERROR;
    else
        return ret;
}