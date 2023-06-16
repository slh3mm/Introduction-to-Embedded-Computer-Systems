#include "msp.h"

#define TRUE 1
#define FALSE 0
#define MASK 0x01


void main(void){
    //Volatile places the value in ram, also makes sure that the O2 flag doesn't over optimize the code, so the for loops can run
    volatile uint32_t i;

    //Stop the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	P1->DIR = P1->DIR | MASK;

	//Infinite Loop
	while(TRUE){
	    int iterations = 68000;
	    //Generate logic 1
	    P1->OUT = P1->OUT | MASK;

	    //Wait for ~1/4 sec
	    for (i = 0; i < iterations; i++){

	    }

	    //Generate logic 0
	    P1->OUT = P1->OUT & ~MASK;

	    //Wait for ~1/4 sec
	    for (i = 0; i < iterations; i++){

	    }

	}
}
