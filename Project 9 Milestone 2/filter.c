#include "filter.h"

extern int y;
int x[NUMBER_OF_SAMPLES];
unsigned int n;
unsigned int CurrentSample;
unsigned int sum;
extern unsigned int m;

void avgFilter()
{
    //Calculate the running total
    sum = CurrentSample + sum - x[n];

    // Update the sample array, x.
	x[n] = CurrentSample;

	// Generate the filter output.
	//Bit shifting is used instead of dividing to act as a fasting operation
	//Bit shifting by 6 is the same as dividing by 64 (2^6)
	y = sum >> 6;

	//Update the array index
	n++;

	//Implementing the circular buffer
	//If we reach the end of the array (number of samples is the last value), the index will go back to the first entry
	//This is effectively "circling" back to the beginning
	if(n == NUMBER_OF_SAMPLES){
	    n = 0;
	}
}

void notchFilter(){
        //Updating the sample array x
        x[n] = CurrentSample;

        //Create the output
        //bit shifting by 1 is the same as dividing by 2
        //m starts at 32, so this works the same as x[n-32]
        //This fix was implemented since x[n-32] would give negative indices
        //The circular buffer allows this to work so there is always 32 values between n and m
        y = ((x[n]+x[m]) >> 1);

        //Updating the array indices of m and n, they are always separated by 32
        n++;
        m++;

        //implementing the circular buffer
        //if either of the indices hit the end of the array (number of samples) they go back to the start
        if(n == NUMBER_OF_SAMPLES){
            n = 0;
        }
        if(m == NUMBER_OF_SAMPLES){
                    m = 0;
        }

}


