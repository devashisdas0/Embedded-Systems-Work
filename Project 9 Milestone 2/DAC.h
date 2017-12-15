#ifndef DAC_DAC_H_
#define DAC_DAC_H_

#include <msp430.h>
#include "spi.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   DAC Chip Select/Load Input
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   GPIO      :  P1.3
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define DAC_CS_BIT				BIT3
#define DAC_CS_PORT				P1OUT
#define DAC_CS_DDR				P1DIR
#define SET_DAC_CS_AS_AN_OUTPUT	DAC_CS_DDR |= DAC_CS_BIT
#define DISABLE_DAC_CS			DAC_CS_PORT |= DAC_CS_BIT
#define ENABLE_DAC_CS			DAC_CS_PORT &= ~DAC_CS_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   DAC Clear (Active Low)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   GPIO      :  P1.5
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define DAC_CLEAR_BIT				BIT5
#define DAC_CLEAR_PORT				P1OUT
#define DAC_CLEAR_DDR				P1DIR
#define SET_DAC_CLEAR_AS_AN_OUTPUT	DAC_CLEAR_DDR |= DAC_CLEAR_BIT
#define DISABLE_DAC_CLEAR			DAC_CLEAR_PORT |= DAC_CLEAR_BIT
#define ENABLE_DAC_CLEAR			DAC_CLEAR_PORT &= ~DAC_CLEAR_BIT

typedef struct {
	int DACAddress;
	int CurrentArrayIndex;
	int ArrayLength;
	unsigned int * ArrayValuesPtr;
} DAC;

// Function Prototypes
void InitializeDACPins(void);
void InitializeDACObject(DAC *Dac, int DACNum, unsigned int * ArrayPtr, int ArrayLength);
int UpdateDACWithArrayValue(DAC *Dac);

// Commands
#define WRITE_TO_INPUT_REGISTER_N 				(0x00 << 4)
#define UPDATE_DAC_REGISTER_N 					(0x01 << 4)
#define WRITE_TO_INPUT_REGISTER_N_UPDATE_ALL_N	(0x02 << 4)
#define WRITE_TO_AND_UPDATE_N	 				(0x03 << 4)
#define	POWER_DOWN_N 							(0x04 << 4)
#define NO_OPERATION							(0x0F << 4)

// Address (N)
#define DAC_A 		0x00
#define DAC_B 		0x01
#define DAC_C 		0x02
#define DAC_D 		0x03
#define ALL_DACS	0x0F

#endif
