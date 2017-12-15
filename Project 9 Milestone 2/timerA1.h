#ifndef TIMERA_H
#define TIMERA_H

#include <msp430.h>
# include "filter.h"

#define TACCR0_VALUE (3125-1)	// 12 MHz MCLK divided by Fs=3840 Hz (64 sample points * 60 Hz)

void ConfigureTimerA1(void);

#endif
