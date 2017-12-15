#include "timerA.h"
#include "LED.h"
#define TA0CCR0_VALUE (62500-1)

void ConfigureTimerA(void)
{
    //Stop the timer and clear it
    TA0CTL = (MC_0 | TACLR);

    //Choose SMCLK select and input divider and start it in up mode
    TA0CTL = (TASSEL_2 | ID_2 | MC_1);

    //CCR0 = period * freq * divider (1/8)
    TA0CCR0 = TA0CCR0_VALUE;

    //Enable interrupt
    TA0CCTL0 |= CCIE;
}

#pragma vector = TIMER0_A0_VECTOR
// Timer a interrupt service routine
__interrupt void TimerA0_routine(void)
{
    TOGGLE_LED1;
//ISR Code to toggle RED Led
}
