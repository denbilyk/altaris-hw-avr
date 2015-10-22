//
// Created by Denis Bilyk on 8/22/15.
//

#include "altaris-module-core.h"

extern uint8_t ce_pin;
extern uint8_t csn_pin;
extern uint8_t ds1820_pin;
extern uint8_t dht11_pin;
extern uint8_t door_pin;
extern uint8_t light_pin;
extern uint64_t tx_address;
extern uint64_t rx_address;
extern uint8_t green_led_pin;
extern uint8_t red_led_pin;
extern uint8_t bat_pin;
extern const char *confID;


WDT_POWER wdtPower;
UART uart;
RF24 rf24(ce_pin, csn_pin);

uint16_t transmit_data[6];
uint8_t dht11_data[2];


void setup(void) {
    uart.begin(9600);
    uart.println("Init UART....");
    uart.println("Init WDT....");
    wdtPower.wdtInit();
    uart.println("Init NRF24....");

    rf24.begin();
    rf24.setRetries(15, 15);
    rf24.setAutoAck(true);
    rf24.setCRCLength(RF24_CRC_8);
    rf24.setDataRate(RF24_250KBPS);

    rf24.openWritingPipe(tx_address);
    rf24.openReadingPipe(0, rx_address);

    rf24.printDetails();

    uart.println("Init DS1820...");
    ds1820_init(ds1820_pin);

    /* External interrupt INT0 */
    pinMode(door_pin, INPUT_PULLUP);
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    uart.println(confID);
    _delay_ms(200);

}

void toggleLed(bool isGreen) {
    if (isGreen) {
        pinMode(green_led_pin, OUTPUT);
        digitalWrite(green_led_pin, HIGH);
    } else {
        pinMode(red_led_pin, OUTPUT);
        digitalWrite(red_led_pin, HIGH);
    }
    _delay_ms(1000);
    pinMode(green_led_pin, TRI_STATE);
    pinMode(red_led_pin, TRI_STATE);
}

void loop(void) {
    transmit_data[0] = tx_address & 0xff;

    bool dht11_status = dht11_read_data(dht11_pin, dht11_data);
    uint8_t temp = 0;
    uint8_t humid = 0;
    if (dht11_status) {
        temp = dht11_data[0];
        humid = dht11_data[1];
    }
    uart.print("DHT11 reading status - ");
    uart.println(dht11_status ? "OK" : "ERROR");
    uart.print("Temp: ");
    uart.println(temp);
    uart.print("Humid: ");
    uart.println(humid);

    float ds_temp = ds1820_read_temp(ds1820_pin);
    uint8_t ds_temp_real = (uint8_t) ds_temp;
    uint8_t ds_temp_fract = (uint8_t) ((ds_temp - ds_temp_real) * 1000);

    uart.print("DS1820 > ");
    uart.print(ds_temp_real);
    uart.print(".");
    uart.print(ds_temp_fract);
    uart.println(" C");

    long lightValue = readLight();
    uart.print("Light sensor: ");
    uart.println(lightValue);


    long batVcc = readVcc();

    uint16_t dht_data = (dht11_status ? temp : 255) << 8;
    dht_data |= dht11_status ? humid : 255;

    uint16_t ds1820_data = ds_temp_real << 8;
    ds1820_data |= ds_temp_fract;

    transmit_data[1] = dht_data;
    transmit_data[2] = ds1820_data;
    transmit_data[3] = (uint16_t) lightValue;
    transmit_data[4] = digitalReadAndShift(door_pin);
    transmit_data[5] = (uint16_t) batVcc;

    rf24.stopListening();
    bool res = rf24.write(&transmit_data, sizeof(transmit_data));
    uart.println(res ? "> Tranmission went OK" : "> Message is lost ...");
    toggleLed(res);

    rf24.powerDown();

    uart.println("Go to sleep...");
    wdtPower.sleep_for(24);
}

long readLight() {
    long tmp = 0;
    uint8_t counter = 10;
    do {
        tmp += analogRead(light_pin);
    } while (--counter > 0);

    tmp = tmp / 10;
    return tmp;
}

long readVcc() {

    ADMUX = 0x00;
    ADMUX |= _BV(REFS0) | _BV(REFS1) | _BV(MUX1);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & (1 << ADSC));

    uint8_t low = ADCL;
    uint8_t high = ADCH;

    long batVcc = (high << 8) | low;

    uart.print("Battery : ");
    uart.println(batVcc);
    return batVcc;
}

ISR(INT0_vect) {
    wdtPower.sleep_reset();
}

int main(void) {
    setup();
    sei();
    do {
        loop();
    } while (true);
}







