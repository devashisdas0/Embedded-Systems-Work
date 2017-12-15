#include "timerA.h"
#include "LED.h"
#define BUTTON BIT3
int up;
int Led;



void InitializePushButtonPortPin (void)
{

    P1OUT |= BUTTON; //Enables output
    P1REN  |= BUTTON;//Enables pull up resistor
    P1IES |= BUTTON; // Enables Interrupt Service for active low
    P1IE |= BUTTON; //Enables interrupt


    Led = 1;

}

#pragma vector = PORT1_VECTOR
// Button interrupt service routine
__interrupt void Button_routine(void)
{
    P1IFG &= ~BUTTON;
    Led = 1 - Led;

}


void ConfigureTimerA(void)
{
    //Stop the timer and clear it
    TA0CTL = (MC_0 | TACLR);

    //Choose SMCLK select and input divider and start it in up mode TAIE is the timer A interrupt
    TA0CTL = (TASSEL_2 | ID_3 | MC_1 | TAIE);

    //TA0CCRO acts as the period of the cycle
    TA0CCR0 = 353;
    //TA0CCR1 is the duty cycle (what percentage of the time the signal is on)
    TA0CCR1 = 1; // This will change for the duty cycle

    //Turn on period flag
    TA0CCTL0 |= CCIE;

    //Turn on duty cycle flag
    TA0CCTL1 |= CCIE;

    //Enable all interrupts


}

#pragma vector = TIMER0_A0_VECTOR
// Interrupt service routine for CCIFG0
__interrupt void Timer0_A0_routine(void)
{

	    //if at max intenisty next state should be decrementing state
	    if(TA0CCR1 == 353){
	        up = 0;
	    }
	    //if at min intensity next state should be incrementing state
	    if(TA0CCR1 == 1){
	        up = 1;
	    }
	    //if in incrementing state start counting up
	    if((up == 1) && (Led == 1)){
	        TA0CCR1 +=1;
	    }
	    //otherwise start counting down
	    if((up == 0) && (Led == 1)){
	        TA0CCR1 -=1;
	    }



}

#pragma vector = TIMER0_A1_VECTOR
// Interrupt service routine for CCIFG1 and TAIFG
__interrupt void Timer0_A1_routine(void)
{
	switch (TAIV){
	case TA0IV_NONE:
		break;
	case TA0IV_TACCR1: // CCIFG1 interrupt
	    TURN_OFF_LED1;
	    break;
	case TA0IV_TAIFG: // TAIFG interrupt
	    if(TA0CCR1 > 10){
	        TURN_ON_LED1;
	    }
	    break;
	default: for (;;); // Should not be possible
	}
}
