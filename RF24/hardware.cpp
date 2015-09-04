/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include "RF24.h"

#include "pins.h"

//#define set_bit(reg, bit) reg |= (1<<bit)
//#define clr_bit(reg, bit) reg &= ~(1<<bit)
//#define check_bit(reg, bit) (reg&(1<<bit))

uint8_t ce;
uint8_t csn;

void nrf24_setupPins(uint8_t CE, uint8_t CSN) {
    pinMode(CE, OUTPUT);
    pinMode(CSN, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    ce = CE;
    csn = CSN;

    //set_bit(DDRA,0); // CE output
    //set_bit(DDRA,1); // CSN output
    //set_bit(DDRA,2); // SCK output
    //set_bit(DDRA,3); // MOSI output
    //clr_bit(DDRA,4); // MISO input
}

/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state) {
    if (state) {
        digitalWrite(ce, HIGH);
        //set_bit(PORTA,0);
    }
    else {
        digitalWrite(ce, LOW);
        //clr_bit(PORTA,0);
    }
}

/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state) {
    if (state) {
        digitalWrite(csn, HIGH);
        //set_bit(PORTA,1);
    }
    else {
        digitalWrite(csn, LOW);
        //clr_bit(PORTA,1);
    }
}

/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state) {
    if (state) {
        digitalWrite(SCK, HIGH);
        //set_bit(PORTA,2);
    }
    else {
        digitalWrite(SCK, LOW);
        //clr_bit(PORTA,2);
    }
}

/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state) {
    if (state) {
        digitalWrite(MOSI, HIGH);
        //set_bit(PORTA, 3);
    }
    else {
        digitalWrite(MOSI, LOW);
        //clr_bit(PORTA, 3);
    }
}

/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead() {
    return digitalRead(MISO);
    //check_bit(PINA, 4);
}
/* ------------------------------------------------------------------------- */



