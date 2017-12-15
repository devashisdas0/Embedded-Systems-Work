#include "debounce.h"

#define FALSE 0
#define TRUE 1

extern unsigned int g1msTimer;

void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime)
{
	Switch->CurrentState = DbExpectHigh;
	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortBit = SwitchBit;

	Switch->HoldTime = HoldTime; // units equal milliseconds
	Switch->ReleaseTime = ReleaseTime; // units equal milliseconds
	Switch->EventTime = 0;
}

SwitchStatus GetSwitch(SwitchDefine *Switch)
{

    if((*(Switch->SwitchPort) & (Switch->SwitchPortBit)) == 0){
        return High;
    }
    else
        return Low;

}



SwitchStatus Debouncer(SwitchDefine *Switch)
{
	SwitchStatus CurrentSwitchReading;
	SwitchStatus DebouncedSwitchStatus = Low;
	unsigned char X1;
	unsigned char X0;
	DbState NextState;
	unsigned int elapsedtime = (g1msTimer -Switch->EventTime);
	// First, determine the current inputs, X1 and X0.

	CurrentSwitchReading = GetSwitch(Switch);
	if(CurrentSwitchReading == High){
	    X0 = TRUE;
	}
	else
	    X0 = FALSE;


	if(Switch->CurrentState == DbValidateHigh){
	    if(elapsedtime > Switch->HoldTime)
	        X1 = TRUE;
	    else
	        X1 = FALSE;
	}
	else if(Switch->CurrentState == DbValidateLow){
	    if(elapsedtime > Switch->ReleaseTime)
	        X1 = TRUE;
	    else
	        X1 = FALSE;
	}

	// Next, based on the input values and the current state, determine the next state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:
		    if(X0 == 1)
		        NextState = DbValidateHigh;
		    else
		        NextState = DbExpectHigh;
		break;
		case DbValidateHigh:
		    if(X0 == 1)
		        NextState = DbExpectLow;
		    else if(X0 == 0)
		        NextState = DbExpectHigh;
		    else
		        NextState = DbValidateHigh;
		break;
		case DbExpectLow:
		    if(X0 == 0)
		        NextState = DbValidateLow;
		    else
		        NextState = DbExpectLow;
		break;
		case DbValidateLow:
		    if(X0 == 0)
		        NextState = DbExpectHigh;
		    else if(X0 ==1)
		        NextState = DbExpectLow;
		    else
		        NextState = DbValidateLow;
		break;
		default: NextState = DbExpectHigh;
	}
	
	// Perform the output function based on the inputs and current state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:
		    if(X0 == 1)
		        Switch->EventTime = g1msTimer;
		    DebouncedSwitchStatus = Low;
			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_LOW;
		break;
		case DbValidateHigh:
		    if(X0 == 1 && X1 ==  1)
		        DebouncedSwitchStatus = High;
		    else
		        DebouncedSwitchStatus = Low;

			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_HIGH;
		break;
		case DbExpectLow:
		    if(X0 == 0)
		        Switch->EventTime = g1msTimer;
		    DebouncedSwitchStatus = High;
		    SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_LOW;
		break;
		case DbValidateLow:
		    if(X0 == 0 && X1 ==  1)
		       DebouncedSwitchStatus = High;
		    else
		       DebouncedSwitchStatus = Low;
			SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_HIGH;
		break;
	}
	
	// Finally, update the current state.
	Switch->CurrentState = NextState;

	return DebouncedSwitchStatus;
}

