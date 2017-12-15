#include "pushbutton.h"

void InitializePushButtonPortPin (void)
{

}

#pragma vector = PORT1_VECTOR
// Button interrupt service routine
__interrupt void Button_routine(void)
{
    
}
