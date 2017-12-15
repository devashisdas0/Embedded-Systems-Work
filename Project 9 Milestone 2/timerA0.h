#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>
#include "spi.h"
#include "DAC.h"

#define TACCR0_VALUE (3125-1)	// 12 MHz MCLK divided by Fs=3840 Hz (64 sample points * 60 Hz)
#define SCALING_FACTOR 6 	// Scale 10-bit ADC values for 16-bit DAC.

// Function Prototypes
void ConfigureTimerA0(void);

#endif
