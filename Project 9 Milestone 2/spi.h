#ifndef USCI_SPI_H
#define USCI_SPI_H

#include <msp430.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-Out-Slave-In (MOSI) 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MOSI_BIT					BIT2
#define USCIA0_MOSI_PORT				P1OUT
#define USCIA0_MOSI_DDR 				P1DIR
#define WRITE_LOGIC_1_TO_SLAVE			USCIA0_MOSI_PORT |= USCIA0_MOSI_BIT
#define WRITE_LOGIC_0_TO_SLAVE			USCIA0_MOSI_PORT &= ~USCIA0_MOSI_BIT
#define SET_USCIA0_MOSI_AS_AN_OUTPUT	USCIA0_MOSI_DDR |= USCIA0_MOSI_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Slave Clock for GPIO Flash Memory Board
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.4
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define SPI_SCK_BIT					BIT4
#define SPI_SCK_PORT				P1OUT // SPI Slave Clock output
#define SPI_SCK_DDR					P1DIR // SPI Slave Clock direction
#define SET_SPI_SCK_AS_AN_OUTPUT	SPI_SCK_DDR |= SPI_SCK_BIT
#define INITIALIZE_SPI_SCK			SPI_SCK_PORT &= ~SPI_SCK_BIT
#define TOGGLE_SPI_SCK				SPI_SCK_PORT ^= SPI_SCK_BIT

void InitializeSPIPortPins();
void InitializeSPI();
void SPISendByte(unsigned char byte_value);

#endif
