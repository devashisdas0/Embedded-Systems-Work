#include "DAC.h"

/*
 * InitializeDACPins
 *
 * Set up the direction registers and initial states for the output pins used to interface with the DAC
 *
 * No arguments
 *
 * Return type: Void
 */
void InitializeDACPins(void)
{
	DISABLE_DAC_CS;
	SET_DAC_CS_AS_AN_OUTPUT;

	DISABLE_DAC_CLEAR;
	SET_DAC_CLEAR_AS_AN_OUTPUT;

}

/*
 * InitializeDACObject
 *
 * Define the initial values for the DAC struct, as demonstrated in main.c.
 *
 * Arguments:
 *  - DAC *Dac: 				Pointer to DAC struct in use
 *  - int DACNum:				Identifier for DAC output A through D
 *  - unsigned int * ArrayPtr	Pointer to the first element of sine table (array) associated with DAC
 *  - int ArrayLength			Variable for length of sine table associated with DAC
 *
 *  Return type: void
 */
void InitializeDACObject(DAC *Dac, int DACNum, unsigned int * ArrayPtr, int ArrayLength)
{
	Dac->DACAddress = DACNum;
	Dac->CurrentArrayIndex = 0;
	Dac->ArrayLength = ArrayLength;
	Dac->ArrayValuesPtr = ArrayPtr;
}

/*
 * UpdateDACWithArrayValue
 *
 * Function returns an integer that represents the value in the sinusoid array
 * located at Dac->CurrentArrayIndex.  Also, once the value has been obtained,
 * Dac->CurrentArrayIndex is incremented modulo Dac->ArrayLength; that is, if
 * Dac->CurrentArrayIndex is greater than Dac->ArrayLength after its incremented,
 * then reset Dac->CurrentArrayIndex to zero.
 *
 * Arguments:
 * - DAC *Dac				Pointer to DAC struct in use
 *
 * Return type: int
 */
int UpdateDACWithArrayValue(DAC *Dac)
{

	int ArrayValue = 0;

	ArrayValue = (*(Dac->ArrayValuesPtr + Dac->CurrentArrayIndex));
	Dac->CurrentArrayIndex++;
	if ((Dac->CurrentArrayIndex) >= (Dac->ArrayLength)) {Dac->CurrentArrayIndex = 0;}

	return ArrayValue;
}

