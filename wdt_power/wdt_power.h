//
// Created by Denis Bilyk on 8/29/15.
//

#ifndef _WDT_POWER_MGNT_H
#define _WDT_POWER_MGNT_H

#include "util/delay.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "avr/sleep.h"
#include "uart.h"

class WDT_POWER {

private:

    void enterSleep();

public:
    void wdtInit();

    void sleep_for(UART *uart, uint8_t sec);
};


#endif //_WDT_POWER_MGNT_H
