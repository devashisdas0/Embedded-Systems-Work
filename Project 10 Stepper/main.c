//Stepper Motor

#include <msp430.h>

#define MS1 BIT4    //2.4
#define MS2 BIT7    //1.7
#define ENABLE BIT6 //1.6
#define STEP BIT2   //2.2
#define DIR BIT3    //2.3
#define RESET BIT5  //2.5
#define SLEEP BIT1  //2.1
#define LED1 BIT0



void ConfigureClockModule(void);

int main(void)
{
        WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
        ConfigureClockModule();

       //Set MS1 and MS2 as output and low
        P2DIR |= MS1;
        P2OUT &= ~MS1;
        P1DIR |= MS2;
        P1OUT &= ~MS2;

        //Reset
        P2DIR |= RESET;
        P2OUT &= ~RESET;

        //Enable high changes to low later
        P1DIR |= ENABLE;
        P1OUT |= ENABLE;


        //Sleep high
        P2DIR |= SLEEP;
        P2OUT |= SLEEP;

        //Direction low
        P2DIR |= DIR;
        P2OUT &= ~DIR;

        //Step as output
        P2OUT |= RESET;
        P2DIR |= STEP;
        P2OUT |= STEP;

        //Set enable to low
        P1OUT &= ~ENABLE;

        //Enable PWM step
        P2DIR |= STEP;
        P2SEL |= STEP;

        //Set PWM Frequency
        TA1CCR0 = 5000-1;
        TA1CCTL1 = OUTMOD_7;
        TA1CCR1 = 2500;
        TA1CTL = TASSEL_2 + MC_1;

        //Enable low


        for(;;){
        }


        return 0;
}

void ConfigureClockModule(void)
{
        // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
        DCOCTL = CALDCO_1MHZ;
        BCSCTL1 = CALBC1_1MHZ;
}
