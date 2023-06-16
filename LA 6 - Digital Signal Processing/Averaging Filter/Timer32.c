#include "Timer32.h"

void TimeDelay(uint16_t delay, uint16_t resolution)
{
    /* The elapsed time for the inner loop represents the period of the unit
     * time increment (such as 1 microsecond or 1 millisecond). The outer loop
     * then represents the total elapsed time in unit time increments
     */

    if (delay > 0){

        TIMER32_1->CONTROL = (TIMER32_CONTROL_ENABLE + TIMER32_CONTROL_SIZE + TIMER32_CONTROL_ONESHOT);
        TIMER32_1->LOAD = delay*resolution;                     // delay*resolution = total elapsed time
        TIMER32_1->INTCLR = 0;                                  // any write clears interrupt flag
        while ((TIMER32_1->RIS & TIMER32_RIS_RAW_IFG) == 0){};  // count down to zero
        TIMER32_1->CONTROL &= ~(TIMER32_CONTROL_ENABLE);        // disable the timer

    }
}
