#include "Timer32.h"

void TimeDelay(uint16_t delay, uint16_t resolution)
{
    /* The elapsed time for the inner loop represents the period of the unit
     * time increment (such as 1 microsecond or 1 millisecond). The outer loop
     * then represents the total elapsed time in unit time increments
     */

    if (delay > 0){

        // Enable the timer, selecting 32-bit size and one-shot mode
        TIMER32_1->CONTROL = (TIMER32_CONTROL_ENABLE + TIMER32_CONTROL_SIZE + TIMER32_CONTROL_ONESHOT);

        // Load the timer with the value for the total elapsed time
        TIMER32_1->LOAD = delay*resolution;

        // Clear the interrupt flag in the status register.
        TIMER32_1->INTCLR = 0;

        // Now, poll the interrupt flag in the status register until its set.
        while ((TIMER32_1->RIS & TIMER32_RIS_RAW_IFG) == 0){};

        // Finally, disable the timer.
        TIMER32_1->CONTROL &= ~(TIMER32_CONTROL_ENABLE);

    }
}
