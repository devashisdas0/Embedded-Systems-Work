
//Brsh Motor
#include <msp430.h>

void ConfigureClockModule(void);
void HandlePIDControl(void);

int encCount;
int currCount;
int readyToPID;

int sumError;
int prevError;
int prevCount;

//Amount of encoder ticks expected over the PID measurement range for 60rpm
const int targetSpeed = 14;

//PID Constants
const float Kp = 7;
const float Ki = 0.001;
const float Kd = 5;


int main(void)
{
        WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
        ConfigureClockModule();

        //Set Debug LEDs
        P1DIR |= BIT6;
        P1OUT &= ~BIT6;
        P1DIR |= BIT0;
        P1OUT &= ~BIT0;

        //Set Standby high
        P1DIR |= BIT1;
        P1OUT |= BIT1;

        //Set Ain1 high
        P1DIR |= BIT3;
        P1OUT |= BIT3;

        //Set Ain2 Low
        P1DIR |= BIT4;
        P1OUT &= ~BIT4;

        //Enable PWM
        P1DIR |= BIT2;
        P1SEL |= BIT2;

        //Set PWM Frequency to 16kHz
        TA0CCR0 = 1000 - 1;
        TA0CCTL1 = OUTMOD_7;
        TA0CCR1 = 73;
        TA0CTL = TASSEL_2 + MC_1;

        //Enable Timer Interrupts for 16Hz
        TA1CCR0 |= 62500- 1;
        TA1CCTL0 |= CCIE;
        TA1CTL |= (TASSEL_2   |   MC_1 | ID_3);

        //Set Encoder Interrupt
        P2IES |= BIT3;
        P2IFG &= ~BIT3;
        P2IE |= BIT3;

        __enable_interrupt();


        for (;; )
                //If the PID period has expired, update control
                if (readyToPID == 1)
                {
                        HandlePIDControl();
                }


        return 0;
}

void HandlePIDControl(void)
{

        //PID Calculations
        int actualSpeed = currCount - prevCount;
        prevCount = currCount;

        int error = targetSpeed - actualSpeed;
        sumError += error;
        float pwm = Kp * error + Ki * sumError + Kd * (error - prevError);
        prevError = error;

        //Cap PWM effort
        if (pwm > 1000)
                pwm = 1000;
        if (pwm < 0)
                pwm = 0;

        //Update control effort
        //TA0CCR1 = (int)pwm;

        //Reset PID period
        readyToPID = 0;
}

#pragma vector = PORT2_VECTOR
// Encoder interrupt service routine
__interrupt void Port2_interrupt(void)
{
        encCount++;
        P1OUT ^= BIT0;
        P2IFG = 0;
}

//Update PID Input
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
{
        P1OUT ^= BIT6;
        currCount = encCount;
        readyToPID = 1;
}

void ConfigureClockModule(void)
{
        // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
        DCOCTL = CALDCO_8MHZ;
        BCSCTL1 = CALBC1_8MHZ;
}
