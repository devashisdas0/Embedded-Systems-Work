#include "LED_display.h"
#include "cordic.h"
#include "debounce.h"
#include "pushbutton.h"
#include "timerA.h"
#include <math.h>
#include <msp430.h>

#define CORDIC_GAIN 0.607252935103139
#define CORDIC_GAIN_X_EQUALS_Y (float)(sqrt(2) / 2)
#define PI 3.1415926536897932384626
#define NUMBER_OF_SAMPLES 8
#define BUTTON BIT3

//Analog digital converter rating
unsigned int adc[3] = { 0 };

//Setting up variables for axes
unsigned int X_Axis_Sample = 0;
unsigned int Y_Axis_Sample = 0;
unsigned in

//Circular Buffers to continuously receive data for directions
int x = 0;
int x_buffer[NUMBER_OF_SAMPLES] = { 0 };
volatile int cumulativeSumX = 0;

int y = 0;
int y_buffer[NUMBER_OF_SAMPLES] = { 0 };
volatile int cumulativeSumY = 0;

int z = 0;
int z_buffer[NUMBER_OF_SAMPLES] = { 0 };
volatile int cumulativeSumZ = 0;

unsigned int n = 0;

//Variables for Calibration
int CalibrationState = 0;
int zMax = 0;
int zMin = 0;
int yMax = 0;
int yMin = 0;
int xMax = 0;
int xMin = 0;
int offSetX = 0;
int offSetY = 0;
int offSetZ = 0;

//Variables to be used by CORDIC
calculations calcs;
int angle;
int hypotenuse;

//Timer variables
unsigned int g1msTimeout;
unsigned int g1msTimer;
unsigned int mainTimer;
unsigned int sideTimer;
unsigned int updateTimer;
unsigned int resetTimer;

//LED variables used for showing how level the surface
unsigned char mainLED;
unsigned char sideLED;
unsigned int mainDuty;

//Variables needed for pushbutton
volatile SwitchStatus PushButtonStatus;
volatile SwitchStatus LastPushButtonStatus;
SwitchDefine PushButton;

//Function prototypes (self explanatory)
void InitializeADC();
void readADC();
void averageFilter(void);
void calculateAngle();
void ConfigureClockModule();
void CalibrateACC();
void updateDisplay();
void ManageSoftwareTimers(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    ConfigureClockModule();
    InitializeSPI();
    InitializeLEDDisplay();
    InitializeADC();
    ConfigureTimerA();
    InitializePushButtonPortPin();
    InitializeSwitch(&PushButton, (char *) &PUSHBUTTON_PORT_IN,
                     (unsigned char) PUSHBUTTON_BIT, HIGH_THRESHOLD,
                     LOW_THRESHOLD);

    _BIS_SR(GIE);

    CalibrateACC();

    while (1)
    {
        readADC();
        averageFilter();
        calculateAngle();
        ManageSoftwareTimers();
    }
    return 0;
}


//This code was given to us
void InitializeADC()
{
    ADC10CTL1 = INCH_2 + CONSEQ_1; // A2/A1/A0, single sequence
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
    ADC10DTC1 = 0x03; // 3 conversions
    ADC10AE0 |= 0x03; // Disable digital I/O on P1.0 to P1.2
}

void readADC()
{
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY)
        ;
    ADC10SA = (unsigned int) adc;
    ADC10CTL0 |= ENC + ADC10SC;
//Defining sample bits
    X_Axis_Sample = adc[2];
    Y_Axis_Sample = adc[1];
    Z_Axis_Sample = adc[0];
    _enable_interrupts();

    __bis_SR_register(GIE);
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

void averageFilter()
{
//Calculate the runnning total for X
    cumulativeSumX = cumulativeSumX + X_Axis_Sample - x_buffer[n];
//Update the sample array for X
//Generate filter output and set the origin using offset
    y = (cumulativeSumY >> 3) - offSetY;
//Calculate the runnning total for Z
    cumulativeSumZ = cumulativeSumZ + Z_Axis_Sample - z_buffer[n];
//Update the sample array for Z
    z_buffer[n] = Z_Axis_Sample;
//Calculate the runnning total for Z
    z = (cumulativeSumZ >> 3) - offSetZ;
//Increment
    n++;
//Make the buffer Circular
    if (n == NUMBER_OF_SAMPLES)
    {
        n = 0;
    }
}


//Calculating Angle using hypotenuse
void calculateAngle()
{
    if (x >= 0)
    { // first quadrant and fourth quadrant
        calcs.x = MUL * x;
        calcs.y = MUL * y;
        calcs.angle = 0;
        Cordic((calculations *) &calcs, ATAN_HYP);
        angle = ((float) calcs.angle) / MUL;
    }
    else if (x < 0 && y >= 0)
    { // second quadrant
        calcs.x = -1 * MUL * x;
        calcs.y = MUL * y;
        calcs.angle = 0;
        Cordic((calculations *) &calcs, ATAN_HYP);
        angle = 180.0 - ((int) calcs.angle) / MUL;
    }
    else if (x < 0 && y < 0)
    { // third quadrant
        calcs.x = -1 * MUL * x;
        calcs.y = -1 * MUL * y;
        calcs.angle = 0;
        Cordic((calculations *) &calcs, ATAN_HYP);
        angle = ((int) calcs.angle) / MUL - 180.0;
    }
    if (x == y)
    {
        hypotenuse = ((int) calcs.x) * CORDIC_GAIN_X_EQUALS_Y / MUL;
    }
    else
    {
        hypotenuse = ((int) calcs.x) * CORDIC_GAIN / MUL;
    }
}

void CalibrateACC()
{
  //Turn on all LED's
    SetLEDDisplay(N + NE + E + SE + S + SW + W + NW);
  //1 second delay
    _delay_cycles(16000000);
 //Turn off LED's
    SetLEDDisplay(OFF);
//Initiakize CalibrationState
    CalibrationState = 0;
    //While the button hasnt been pressed 6 times (6 calibrations)
    while (CalibrationState < 6)
    {
      //Code for pushing button, when button is pushed increase the calibration state
        ManageSoftwareTimers();
        PushButtonStatus = Debouncer(&PushButton);
        if (LastPushButtonStatus != PushButtonStatus)
        {
            if (PushButtonStatus == High)
            {
                CalibrationState++;
            }
        }
        LastPushButtonStatus = PushButtonStatus;
        //Read in values
        readADC();
        //Average the values
        averageFilter();
        //Based on the number of button clicks the calibration state turns on certain LED's
        switch (CalibrationState)
        {
        //North State
        case 0:
            mainLED = N;
            sideLED = OFF;
            mainDuty = 60;
            xMax = x;
            break;
        //South
        case 1:
            mainLED = S;
            sideLED = OFF;
            mainDuty = 60;
            xMin = x;
            break;
        //West
        case 2:
            mainLED = W;
            sideLED = OFF;
            mainDuty = 60;
            yMax = y;
            break;
        //East
        case 3:
            mainLED = E;
            sideLED = OFF;
            mainDuty = 60;
            yMin = y;
            break;
        //+Z
        case 4:
            mainLED = N + S + E + W;
            sideLED = OFF;
            mainDuty = 60;
            zMax = z;
            break;
        //-Z
        case 5:
            mainLED = NE + SE + NW + SW;
            sideLED = OFF;
            mainDuty = 60;
            zMin = z;
            break;
        }
    }
    //Offset used for calibrations which makes the origin 0
    offSetX = (xMax + xMin) >> 1;
    offSetY = (yMax + yMin) >> 1;
    offSetZ = (zMax + zMin) >> 1;
}

//This function updates the LED display based on the angle and the hypotenuse
void updateDisplay()
{
  //Makes sure the machine is calibrated
    if (CalibrationState > 5)
    {
      //If the board is level turn all the lights on and at the highest relative brightness
        if (hypotenuse < 10)
        {
            mainLED = N + NE + E + SE + S + SW + W + NW;
            sideLED = OFF;
            mainDuty = 50;

        }
        //Based on the angle of the board certain LED's will light up
        //The LED matching the tilt of the board will be the brightest
        //The LED's adjacent to the brightest LED will approximately be half the brightness
        else
        {
//North LED Lights up
            if (-30 < angle && angle < 30)
            {
                mainLED = N;
                mainDuty = hypotenuse;
                sideLED = NE + NW;
            }
//NE LED lights up
            else if (-60 < angle && angle < -30)
            {
                mainLED = NE;
                sideLED = N + E;
                mainDuty = hypotenuse;

            }
//E LED lights up
            else if (angle > -120 && angle <= -60)
            {

                mainLED = E;
                sideLED = NE + SE;
                mainDuty = hypotenuse;
            }
//SE LED lights up
            else if ((angle > -150 && angle <= -120))
            {

                mainLED = SE;
                sideLED = E + S;
                mainDuty = hypotenuse;
            }
//S LED lights up
            else if ((angle > -180 && angle <= -150)
                    || (angle <= 180 && angle > 150))
            {

                mainLED = S;
                sideLED = SW + SE;
                mainDuty = hypotenuse;
            }
//SW LED lights up
            else if (angle > 120 && angle <= 150)
            {

                mainLED = SW;
                sideLED = S + W;
                mainDuty = hypotenuse;
            }
//W LED lights up
            else if (angle > 60 && angle <= 120)
            {
                mainLED = W;
                sideLED = NW + SW;
                mainDuty = hypotenuse;
            }
//NW LED lights up
            else
            {
                mainLED = NW;
                sideLED = N + W;
                mainDuty = hypotenuse;
            }
        }
    }
}

void ConfigureClockModule()
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations
    //Using 16MHz Clock
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}

void ManageSoftwareTimers(void)
{
    if (g1msTimeout)
    {
        g1msTimeout--;
        g1msTimer++;
        updateTimer++;
        mainTimer++;
        sideTimer++;
        resetTimer++;
    }
    if (sideTimer >= mainDuty / 6)
    {
        SetLEDDisplay(mainLED);
        sideTimer = 0;
    }
    if (mainTimer >= mainDuty / 12)
    {
        SetLEDDisplay(OFF);
        mainTimer = 0;
    }
    if (resetTimer >= 10)
    {
        SetLEDDisplay(mainLED + sideLED);
        resetTimer = 0;

    }
    if (updateTimer == 100)
    {
        updateDisplay();
        updateTimer = 0;
    }
}
