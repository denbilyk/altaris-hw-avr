//
// Created by Denis Bilyk on 8/29/15.
//

#include "wdt_power.h"

#define SERIAL_DELAY _delay_ms(300)

short sleep_cycles = 0;
volatile short sleep_cycles_remain = sleep_cycles;

#define PRE_8S (1 << WDP0 | 1 << WDP3)
#define PRE_4S (1 << WDP3)
#define PRE_2S (1 << WDP0 | 1 << WDP1 | 1 << WDP2)
#define PRE_1S (1 << WDP1 | 1 << WDP2)

void WDT_POWER::wdtInit() {
    /* Clear the reset flag. */
    MCUSR &= ~(1 << WDRF);

    /* In order to change WDE or the prescaler, we need to
     * set WDCE (This will allow updates for 4 clock cycles).
     */
    WDTCSR |= (1 << WDCE) | (1 << WDE);

    /* set new watchdog timeout prescaler value */
    WDTCSR = PRE_8S; /* 8.0 seconds */

    /* Enable the WD interrupt (note no reset). */
    WDTCSR |= _BV(WDIE);

    _delay_ms(100);
}

void WDT_POWER::sleep_for(uint8_t sec) {
    sleep_cycles = (short) (sec / 8);
    sleep_cycles_remain = sleep_cycles;

    while (sleep_cycles_remain) {
        uart_puts("Sleep cycle - ");
        uart_putc(sleep_cycles_remain);
        SERIAL_DELAY;
        enterSleep();
    }
    sleep_cycles_remain = sleep_cycles;
    uart_puts("Sleep complete. Wake up!");
    SERIAL_DELAY;
}

void WDT_POWER::sleep_reset() {
    sleep_cycles_remain = 0;
}

void WDT_POWER::enterSleep() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
}

ISR(WDT_vect) {
    --sleep_cycles_remain;
}
