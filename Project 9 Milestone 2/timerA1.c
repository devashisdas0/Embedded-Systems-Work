#include "timerA1.h"

extern unsigned int CurrentSample;

void ConfigureTimerA1(void)
{
	// Timer0_A3 Control Register
	TA1CTL = (MC_0 | TACLR);

	TA1CTL = (TASSEL_2 + ID_0 + MC_1);
	/* TASSEL1 = 0x0200 Timer A clock source select 0
	 * TASSEL0 = 0x0100 Timer A clock source select 1
	 *  00 = TACLK
	 *  01 = ACLK
	 *  10 = SMCLK
	 *  11 = INCLK
	 * ID1 = 0x0080 Timer A clock input divider 1
	 * ID0 = 0x0040 Timer A clock input divider 0
	 *  00 => Input divider = 1
	 *  01 => Input divider = 2
	 *  10 => Input divider = 4
	 *  11 => Input divider = 8
	 * MC1 = 0x0020 Timer A mode control 1
	 * MC0 = 0x0010 Timer A mode control 0
	 *  00 = Stop
	 *  01 = Up
	 *  10 = Continuous
	 *  11 = Up/Down
	 * TACLR = 0x0004 Timer A counter clear
	 * TAIE = 0x0002 Timer A counter interrupt enable
	 * TAIFG = 0x0001 Timer A counter interrupt flag
	 */

	TA1CCR0 = TACCR0_VALUE;

	// TACCR0 interrupt enabled (see Section 12.2.6 of User's Guide)
	TA1CCTL0 |= CCIE;
}

// Timer1 CCR0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
{
	// Start conversion
	ADC10CTL0 |= ADC10SC;

	// Wait for ADC conversion to complete.
	while (ADC10CTL1 & ADC10BUSY);

	// Read the current sample value from the ADC.
	CurrentSample = ADC10MEM;

	avgFilter();
}

