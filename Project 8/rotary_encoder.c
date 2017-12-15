#include "rotary_encoder.h"

void InitializeRotaryEncoderPortPins(void)
{
	SET_ROTARY_ENCODER_A_AS_AN_INPUT;
	SET_ROTARY_ENCODER_B_AS_AN_INPUT;
}

EncoderState RotaryEncoder(EncoderDefinitions *myRotorEncoder, EncoderState *myEncoderState)
{
	EncoderState NextEncoderState = *myEncoderState;


	switch (*myEncoderState){
	case Initialize:
	    if(myRotorEncoder->Reset == High){
	        NextEncoderState = Initialize;
	        myRotorEncoder->RotaryEncoderStateCount = 0;
	    }
	    else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
	        NextEncoderState = AhighBhigh;
	        myRotorEncoder->RotaryEncoderStateCount = 0;
	    }
	    else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
	                NextEncoderState = AhighBlow;
	                myRotorEncoder->RotaryEncoderStateCount = 0;
	    }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
                    NextEncoderState = AlowBhigh;
                    myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AlowBlow;
                    myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        break;

	case AlowBlow:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AlowBlow;//Don't care so keep on same state
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AhighBlow;
                    myRotorEncoder->RotaryEncoderStateCount++;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
                    NextEncoderState = AlowBhigh;
                    myRotorEncoder->RotaryEncoderStateCount--;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AlowBlow;//Don't care so keep on same state
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        break;


	case AlowBhigh:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
            myRotorEncoder->RotaryEncoderStateCount--;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AlowBhigh; //Dont care
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
                    NextEncoderState = AlowBhigh; //Dont care
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AlowBlow;
                    myRotorEncoder->RotaryEncoderStateCount++;
        }
        break;


	case AhighBlow:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;
            myRotorEncoder->RotaryEncoderStateCount++;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AhighBlow; //Dont care
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
                    NextEncoderState = AhighBlow; //Dont care
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AlowBlow;
                    myRotorEncoder->RotaryEncoderStateCount--;
        }
        break;

	case AhighBhigh:
        if(myRotorEncoder->Reset == High){
            NextEncoderState = Initialize;
            myRotorEncoder->RotaryEncoderStateCount = 0;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == High){
            NextEncoderState = AhighBhigh;//Don't care so keep on same state
            myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == High && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AhighBlow;
                    myRotorEncoder->RotaryEncoderStateCount--;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == High){
                    NextEncoderState = AlowBhigh;
                    myRotorEncoder->RotaryEncoderStateCount++;
        }
        else if(myRotorEncoder->Reset == Low && myRotorEncoder->SwitchA == Low && myRotorEncoder->SwitchB == Low){
                    NextEncoderState = AhighBhigh;//Don't care so keep on same state
                    myRotorEncoder->RotaryEncoderStateCount = myRotorEncoder->RotaryEncoderStateCount;
        }
        break;

	default:
		NextEncoderState = Initialize;
		myRotorEncoder->RotaryEncoderStateCount = 0;
	}

	return NextEncoderState;
}
