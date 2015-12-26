//
// Created by Denis Bilyk on 8/22/15.
//

#include "altaris-module-core.h"

extern uint8_t ce_pin;
extern uint8_t csn_pin;
extern uint8_t ds1820_pin;
extern uint8_t ds1820_ctl_pin;
extern uint8_t dht11_pin;
extern uint8_t dht11_ctl_pin;
extern uint8_t door_pin;
extern uint8_t light_pin;
extern uint8_t light_ctl_pin;
extern uint64_t tx_address;
extern uint64_t rx_address;
extern const char *confID;


WDT_POWER wdtPower;
UART uart;
RF24 rf24(ce_pin, csn_pin);

uint16_t transmit_data[6];
uint8_t dht11_data[2];


void setup(void) {
    init_ctl_pins();
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

    /* ADC */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);

    uart.println(confID);
    _delay_ms(200);

}

uint16_t readVcc() {
    const long int_ref_const = 1060L;  // Adust this value to your boards specific internal BG voltage x1000
    // REFS1 REFS0          --> 0 1, AVcc internal ref.
    // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
    _delay_ms(100);
    // Start a conversion
    ADCSRA |= (1 << ADSC);
    // Wait for it to complete
    while (ADCSRA & (1 << ADSC));
    // Scale the value
    uint16_t adc = ADC;
    long results = (int_ref_const * 1024L / adc + 5L) / 10L;
    return (uint16_t) results;
}

void loop(void) {
    transmit_data[0] = tx_address & 0xff;

    startDevice(dht11_ctl_pin, 500);
    bool dht11_status = dht11_read_data(dht11_pin, dht11_data);
    stopDevice(dht11_ctl_pin);
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

    startDevice(ds1820_ctl_pin, 500);
    int16_t ds_temp_raw = ds1820_read_temp(ds1820_pin);
    stopDevice(ds1820_ctl_pin);
    float ds_temp = rawToCelsius(ds_temp_raw);

    uint8_t ds_temp_real = (uint8_t) ds_temp;
    uint8_t ds_temp_fract = (uint8_t) ((ds_temp - ds_temp_real) * 1000);

    uart.print("DS1820 > ");
    uart.print(ds_temp_real);
    uart.print(".");
    uart.print(ds_temp_fract);
    uart.println(" C");

    startDevice(light_ctl_pin, 200);
    long lightValue = readLight();
    stopDevice(light_ctl_pin);
    uart.print("Light sensor: ");
    uart.println(lightValue);


    uint16_t batVcc = readVcc();
    uart.print("Battery : ");
    uart.println(batVcc);

    uint16_t dht_data = (dht11_status ? temp : 255) << 8;
    dht_data |= dht11_status ? humid : 255;

    uint16_t ds1820_data = ds_temp_real << 8;
    ds1820_data |= ds_temp_fract;

    transmit_data[1] = dht_data;
    transmit_data[2] = ds1820_data;
    transmit_data[3] = (uint16_t) lightValue;
    transmit_data[4] = digitalReadAndShift(door_pin);
    transmit_data[5] = batVcc;

    rf24.stopListening();
    bool res = false; //rf24.write(&transmit_data, sizeof(transmit_data));
    uart.println(res ? "> Tranmission went OK" : "> Message is lost ...");

    rf24.powerDown();

    uart.println("Go to sleep...");
    wdtPower.sleep_for(304);  //5 min
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

void startDevice(uint8_t pin, int delay) {
    digitalWrite(pin, HIGH);
    while (delay--) {
        _delay_ms(1);
    }
}

void stopDevice(uint8_t pin) {
    digitalWrite(pin, LOW);
}

ISR(INT0_vect) {
    wdtPower.sleep_reset();
}

int main(void) {
    init_hardware();
    setup();
    sei();
    do {
        loop();
    } while (true);
}


void init_ctl_pins() {
    pinMode(light_ctl_pin, OUTPUT);
    digitalWrite(light_ctl_pin, LOW);
    pinMode(dht11_ctl_pin, OUTPUT);
    digitalWrite(dht11_ctl_pin, LOW);
    pinMode(ds1820_ctl_pin, OUTPUT);
    digitalWrite(ds1820_ctl_pin, LOW);
}
