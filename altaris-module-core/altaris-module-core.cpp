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
extern const char *confID;


WDT_POWER wdtPower;
UART uart;
RF24 rf24(ce_pin, csn_pin);

uint8_t transmit_data[7];
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

void loop(void) {
    uart.println("Available....");
    _delay_ms(100);
    transmit_data[0] = tx_address & 0xff;

    uart.println("Read DHT11...");

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


    uart.println("Read DS1820...");
    float ds_temp = ds1820_read_temp(ds1820_pin);
    uint8_t ds_temp_real = (uint8_t) ds_temp;
    uint8_t ds_temp_fract = (uint8_t) ((ds_temp - ds_temp_real) * 1000);

    uart.print("DS1820 > ");
    uart.print(ds_temp_real);
    uart.print(".");
    uart.print(ds_temp_fract);
    uart.println(" C");

    long result = analogRead(light_pin);
    uart.print("Light sensor: ");
    uart.println((uint8_t) result);


    transmit_data[1] = dht11_status ? temp : 255;
    transmit_data[2] = dht11_status ? humid : 255;
    transmit_data[3] = ds_temp_real;
    transmit_data[4] = ds_temp_fract;
    transmit_data[5] = (uint8_t) result;
    transmit_data[6] = digitalReadAndShift(door_pin);

    rf24.stopListening();
    bool res = rf24.write(&transmit_data, sizeof(transmit_data));
    uart.println(res ? "> Tranmission went OK" : "> Message is lost ...");

    _delay_ms(300);
    /* Or you might want to power down after TX */
    rf24.powerDown();

    // long adc = readVcc();
    // uart.print("ADC res : ");
    // uart.println(adc);

    /* Wait a little ... */
    //_delay_ms(2000);
    uart.println("Go to sleep...");
    wdtPower.sleep_for(24);
    _delay_ms(500);
}

long readVcc() {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    _delay_ms(75); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC) | _BV(ADEN); // Start conversion
    while (ADCSRA & (1 << ADSC)); // measuring

    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    long result = (high << 8) | low;

    result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result; // Vcc in millivolts
}

ISR(INT0_vect) {
    uart.println("INT0 interrupt...");
    wdtPower.sleep_reset();
}

int main(void) {
    setup();
    sei();
    do {
        loop();
    } while (true);
}







