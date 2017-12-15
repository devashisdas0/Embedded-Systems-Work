#ifndef USCI_SPI_H
#define USCI_SPI_H

#include <msp430.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-In-Slave-Out (MISO)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MISO_BIT					BIT1
#define USCIA0_MISO_PORT				P1IN
#define USCIA0_MISO_DDR					P1DIR
#define READ_BIT_FROM_SLAVE				((unsigned char) (USCIA0_MISO_PORT & USCIA0_MISO_BIT) >> 1)
#define SET_USCIA0_MISO_AS_AN_INPUT		USCIA0_MISO_DDR &= ~USCIA0_MISO_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-Out-Slave-In (MOSI)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MOSI_BIT					BIT7
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
#define SPI_SCK_BIT					BIT5
#define SPI_SCK_PORT				P1OUT // SPI Slave Clock output
#define SPI_SCK_DDR					P1DIR // SPI Slave Clock direction
#define SET_SPI_SCK_AS_AN_OUTPUT	SPI_SCK_DDR |= SPI_SCK_BIT
#define INITIALIZE_SPI_SCK			SPI_SCK_PORT &= ~SPI_SCK_BIT
#define TOGGLE_SPI_SCK				SPI_SCK_PORT ^= SPI_SCK_BIT

/*
 * This function initializes all hardware and port pins to support SPI.
 */
void InitializeSPI();

/*
 * This function sends the byte, SendValue, using SPI.
 */
void SPISendByte(unsigned char SendValue);

/*
 * This function receives a byte using SPI.
 *
 * Return Value: The byte that is received over SPI.
 */
unsigned char SPIReceiveByte();

#endif
