#include "Timer32.h"
#include "msp.h"
#include <stdint.h>

void TimeDelay(uint16_t delay, uint16_t resolution){
    if (delay > 0){
        //Enable the timer, selecting 32-bit size and one-shot mode
        TIMER32_1->CONTROL |= 0x83;

        //Load the timer with the value for the total elapsed time
        TIMER32_1->LOAD = resolution * delay;

        //Clear the interrupt flag in the status register
        TIMER32_1->INTCLR = 0x03;

        //Now, poll the interrupt flag in the status register until it is set
        while ((TIMER32_1->MIS & 0x01) == 0x00){}

        //Finally, disable the timer
        TIMER32_1->CONTROL &= ~(0x83);

    }
}
