#include <msp430.h>
#include "LED.h"
#define TRUE 1

// Function prototypes
void ConfigureClockModule(void);
void TimeDelay(unsigned int delay, unsigned int resolution);


void main(void)
{
    // Stop the watchdog timer, and configure the clock module.
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    InitializeLEDPortPins();

    // Infinite loop
    while (TRUE)
    {
      TOGGLE_LED1;
      TOGGLE_LED2;
      TimeDelay(250,999);
    }
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
    DCOCTL  = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}

void TimeDelay(unsigned int delay, unsigned int resolution){ //Use timer A1

  volatile int i;
  //Stop the timer and clear the register
  TA1CTL = (MC_0 | TACLR);
  //Update the value of compare-capture register
  TA1CCR0 = resolution;
  //Use the smclk (TASSEL_2) select and divide by 1 (ID_0) and start it in up mode (MC_1)
  TA1CTL = (TASSEL_2 | ID_0 | MC_1);


  for(i = 0; i < delay; i++){// Wait for 250ms
      while(!(TA1CTL & TAIFG));
      //Use bit wise complement so when you 'and' the control register
      //the flag it will stay 0 until the condition will become a 1
      //TA1CTL is an 8bit register with bit 0 is the bit of interest currently at 0
      //TAIFG is a bit in the register 00000001 so when we 'and' them we should get 0 until TA1CTL bit 0 is a 1

      //Reset TAIFG
      TA1CTL &= ~(TAIFG);


  }
//  //Stop the timer and clear the register
  TA1CTL = (MC_0 | TACLR);

}

