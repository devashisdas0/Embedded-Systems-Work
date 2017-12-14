//
// This program presents a simple User Interface (UI) for controlling the LEDs.
//
#include <msp430.h>
#include <stdio.h>

#include "LED.h"
#include "uart.h"

#define BUFFER_SIZE 80
#define TRUE 1
#define FALSE 0
#define UART 0

// Global variable
char CharacterHasNotBeenReceived;
char ReceivedCharacter = (char) NULL;;

// Function prototypes
void ConfigureClockModule(void);
void PrintString(char *, unsigned char);
void PrintCharacter(char, unsigned char);

void main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

    // Initialize LEDs
    InitializeLEDPortPins();

	// Initialize USCI A0 to use UART.
	InitializeUART();

	_enable_interrupts();

    // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
	// followed by the prompt string.

    
	// Continue with the UI indefinitely.
	while (TRUE) {
        
		PrintString("\r\nEnter a command - (1) Turn on LED (2) Turn off LED (3) Blink LED (x) Exit: ",UART);

		// Wait for a character to be entered.
		CharacterHasNotBeenReceived = TRUE;
		while (CharacterHasNotBeenReceived);

		// Echo character
		PrintString("\r\n>>> ",UART);
		PrintCharacter(ReceivedCharacter,UART);
		if(ReceivedCharacter == '1'){
		    PrintString("\r\n (1) Red LED (2) Green LED",UART);
		    CharacterHasNotBeenReceived = TRUE;
		    while (CharacterHasNotBeenReceived);
		    PrintString("\r\n>>> ",UART);
		    PrintCharacter(ReceivedCharacter,UART);
		    if(ReceivedCharacter == '1'){
		        PrintString("\n Red LED is now on ",UART);
		        TURN_ON_LED1;
		    }
		    else if(ReceivedCharacter == '2'){
		        PrintString("\n Green LED is now on",UART);
		        TURN_ON_LED2;
		    }
		    else
		        break;
		}
		else if(ReceivedCharacter == '2'){
		    PrintString("\n Turning off LED's ",UART);
		    TURN_OFF_LED1;
		    TURN_OFF_LED2;

		}
		else if(ReceivedCharacter == '3'){
		    PrintString("\r\n Blink which LED? (1) Red LED (2) Green LED",UART);
		    CharacterHasNotBeenReceived = TRUE;
		    while (CharacterHasNotBeenReceived);
		    PrintString("\r\n>>> ",UART);
		    PrintCharacter(ReceivedCharacter,UART);
		    if(ReceivedCharacter == '1'){
		        volatile int numloops;
		        PrintString("\n Red LED is blinking now",UART);
		        for(numloops =0; numloops < 5; numloops++){
		        TURN_ON_LED1;
		        volatile int a,c;
		        volatile int b,d;
		        for(b = 0; b <250; b++)
		            for(a = 0; a <250; a++)
		            {}
		        TURN_OFF_LED1;
		        for(c = 0; c <250; c++)
		           for(d = 0; d <250; d++)
		           {}
		        }
		    }

		    else if(ReceivedCharacter == '2'){
		         volatile int loops;
		         PrintString("\n Green LED is blinking now",UART);
		         for(loops = 0; loops < 5; loops++){
		         TURN_ON_LED2;
		         volatile int w,x,y,z;

		         for(x = 0; x <250; x++)
		             for(y = 0; y <250; y++)
		                 {}
		         TURN_OFF_LED2;
		         for(x = 0; x <250; x++)
		             for(y = 0; y <250; y++)
		                 {}
		         }

		    }
		    else
		        break;
		}
		else if(ReceivedCharacter == 'x'){
		    PrintString("\r\n Exiting... ",UART);
		    ShutdownUART();

		}
		else{
		    break;
		}
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) to use factory calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void PrintString(char * StringToPrint, unsigned char PrintDeviceType)
{
	unsigned short i;                     // Used for indexing
	char StringBuffer[BUFFER_SIZE];    // Character buffer to hold each string to be printed

	for (i = 0; i < BUFFER_SIZE; i++) StringBuffer[i] = (char) NULL;
	sprintf(StringBuffer, StringToPrint);
	switch (PrintDeviceType) {
	case UART:
		WriteStringUART(StringBuffer);
		break;
	default: break;
	}
}

void PrintCharacter(char CharacterToPrint, unsigned char PrintDeviceType)
{
	switch (PrintDeviceType) {
	case UART:
		WriteCharacterUART(CharacterToPrint);
		break;
	default: break;
	}
}
