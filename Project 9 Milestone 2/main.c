#include <msp430.h>

#include "LED.h"
#include "ADC.h"
#include "spi.h"
#include "DAC.h"
#include "debug.h"
#include "timerA0.h"
#include "timerA1.h"
#include "filter.h"

// Global Variables
int y; // Filter output
int x[NUMBER_OF_SAMPLES]; // Array to hold input samples
unsigned int n; // Index that indicates location to place current sample input
unsigned int CurrentSample;
unsigned int m = 32;

// Filter output scaled to account for resolution discrepancy between ADC and DAC because the system is linear
int y_scaled;

// Local prototypes
void ConfigureClockModule();
void InitializeSampleBuffer(void);

int main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

	InitializeLEDPortPins();
	ConfigureTimerA0();
	InitializeSPI();
	InitializeDACPins();
	ConfigureTimerA1();
	ConfigureADC();

	// Initialize the array used to store the ADC samples, along with the variables
	// used in filter().
	InitializeSampleBuffer();

	SET_DEBUG_LOW;
	SET_DEBUG_TO_AN_OUTPUT;

	_enable_interrupts();

	// Infinite loop
	//while (1) {
	//}
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
	DCOCTL  = CALDCO_12MHZ;
	BCSCTL1 = CALBC1_12MHZ;
}

void InitializeSampleBuffer(void)
{
	unsigned char i;

	for (i = 0; i < NUMBER_OF_SAMPLES; i++) x[i] = 0;

	CurrentSample = 0;
	n = 0;

	y = 0;
	y_scaled = 0;
}

