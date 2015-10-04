//
// Created by Denis Bilyk on 10/2/15.
//


#include "hardware.h"

volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

unsigned long systemTime() {
    unsigned long m;
    uint8_t old_SREG = SREG;
    m = timer0_millis;
    SREG = old_SREG;
    return m;
}


bool timeout(unsigned long m) {
    return systemTime() - m > DEFAULT_TIMEOUT;
}

void init_hardware() {
    uint8_t old_sreg = SREG;

    TIMSK2 |= (1 << TOIE2);
    TCCR2B |= (1 << CS22); // pascale to 64. On 16MHz one tick is 1/250kHz - 40us
    TCNT2 = 0;
    SREG = old_sreg;
}

ISR(TIMER2_OVF_vect) {
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
}
