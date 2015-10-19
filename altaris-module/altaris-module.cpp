//
// Created by Denis Bilyk on 8/22/15.
//

#include "altaris-module.h"


#define CE D9
#define CSN D10
#define DS1820 D8
#define DHT11 A0
#define DOOR D2
#define LIGHT A1


WDT_POWER wdtPower;
UART uart;


uint8_t transmit_data[7];
uint8_t dht11_data[2];

uint8_t tx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
uint8_t rx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};


void setup(void) {
    uart.begin(9600);

    uart.println("Init UART....");
    uart.println("Init WDT....");
    wdtPower.wdtInit();
    uart.println("Init NRF24....");

    nrf24_init(CE, CSN);
    /* Channel #2 , payload length: 4 */
    nrf24_config(2, 7);
    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address_0(rx_address);

    uart.println("Init DS1820...");
    ds1820_init(DS1820);


    /* External interrupt INT0 */
    pinMode(DOOR, INPUT_PULLUP);
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);


    _delay_ms(200);

}

void loop(void) {
    uart.println("Available....");
    _delay_ms(100);
    transmit_data[0] = tx_address[4];

    uart.println("Read DHT11...");

    bool dht11_status = dht11_read_data(DHT11, dht11_data);
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
    float ds_temp = ds1820_read_temp(DS1820);
    uint8_t ds_temp_real = (uint8_t) ds_temp;
    uint8_t ds_temp_fract = (uint8_t) ((ds_temp - ds_temp_real) * 1000);

    uart.print("DS1820 > ");
    uart.print(ds_temp_real);
    uart.print(".");
    uart.print(ds_temp_fract);
    uart.println(" C");

    long result = analogRead(LIGHT);
    uart.print("Light sensor: ");
    uart.println((uint8_t) result);


    transmit_data[1] = dht11_status ? temp : 255;
    transmit_data[2] = dht11_status ? humid : 255;
    transmit_data[3] = ds_temp_real;
    transmit_data[4] = ds_temp_fract;
    transmit_data[5] = (uint8_t) result;
    transmit_data[6] = digitalReadAndShift(DOOR);

    nrf(transmit_data);

    _delay_ms(300);
    /* Or you might want to power down after TX */
    nrf24_powerDown();

    // long adc = readVcc();
    // uart.print("ADC res : ");
    // uart.println(adc);

    /* Wait a little ... */
    //_delay_ms(2000);
    uart.println("Go to sleep...");
    wdtPower.sleep_for(24);
    _delay_ms(500);
}


void nrf(uint8_t *data_array) {
    uint8_t temp;
    /* Automatically goes to TX mode */
    nrf24_send(data_array);

    /* Wait for transmission to end */
    while (nrf24_isSending());

    /* Make analysis on last tranmission attempt */
    temp = nrf24_lastMessageStatus();

    if (temp == NRF24_TRANSMISSON_OK) {
        uart.println("> Tranmission went OK\r\n");
    }
    else if (temp == NRF24_MESSAGE_LOST) {
        uart.println("> Message is lost ...\r\n");
    }

    /* Retranmission count indicates the tranmission quality */
    temp = nrf24_retransmissionCount();
    uart.print("> Retranmission count: ");
    uart.println(temp);
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







