#include "msp.h"

#define TRUE 1
#define FALSE 0
#define MASK 0x01

void TimeDelay(uint16_t delay, uint16_t resolution);

void main(void){
    //Volatile places the value in ram, also makes sure that the O2 flag doesn't over optimize the code, so the for loops can run
    volatile uint32_t i;
    uint16_t delay = 250;
    uint16_t resolution = 250;

    //Stop the watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P1->DIR = P1->DIR | MASK;
    P1->OUT = P1->OUT & ~MASK;

    //Infinite Loop
    while(TRUE){
        P1->OUT = P1->OUT ^ MASK;

        TimeDelay(delay, resolution);

    }
}

void TimeDelay(uint16_t delay, uint16_t resolution){
    volatile uint16_t i,j;

    for (i = 0; i < delay; i++){
        for (j = 0; j < resolution; j++){

        }
    }
}
