//
// Created by Denis Bilyk on 10/2/15.
//

#ifndef HARDWARE_H
#define HARDWARE_H

#include "stdint.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"


#define clockCyclesToMicroseconds(a) ( (a) / ( F_CPU / 1000000L ) )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define DEFAULT_TIMEOUT 1500


unsigned long systemTime();

bool timeout(unsigned long m);

void init_hardware();


#endif //HARDWARE_H
