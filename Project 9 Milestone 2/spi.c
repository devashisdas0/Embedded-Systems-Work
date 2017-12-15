#include "spi.h"

void InitializeSPIPortPins()
{
	// Configure port pins
	SET_SPI_SCK_AS_AN_OUTPUT;

	// Configure port pin to receive output from USCI A0 clock.
	P1SEL2 |= SPI_SCK_BIT; P1SEL |= SPI_SCK_BIT;

	WRITE_LOGIC_0_TO_SLAVE;
	SET_USCIA0_MOSI_AS_AN_OUTPUT;

	// Configure port pin to receive output from USCI A0 MOSI.
	P1SEL2 |= USCIA0_MOSI_BIT; P1SEL |= USCIA0_MOSI_BIT;
}

void InitializeSPI()
{	
	// Software reset enabled. USCI logic held in reset state.
	UCA0CTL1 = UCSWRST;

	// Initialize all USCI registers with UCSWRST = 1 (including UCxCTL1)

	// Select USCI SPI functionality.
	UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;
	UCA0CTL1 |= UCSSEL_2;			// SMCLK as source
	UCA0BR1 = 0; UCA0BR0 |= 0x01;	// divide clock by 1
	UCA0TXBUF = 0; 					// initialize transmit buffer to 0

	InitializeSPIPortPins();

	// Software reset disabled. USCI logic released for operation.
    UCA0CTL1 &= ~UCSWRST;
}

void SPISendByte(unsigned char byte_value)
{
	UCA0TXBUF = byte_value;
	while (UCA0STAT & UCBUSY);
}
