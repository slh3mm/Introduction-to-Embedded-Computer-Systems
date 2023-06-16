#include "TimerA0.h"
#include "msp.h"
#include "LED.h"
#include <stdbool.h>

extern bool currLEDisRGB;

void InitializeTimerA0(void){
    //Stop timer and clear the registers
    TIMER_A0->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);

    //Assign a value to CCR0 equal to the desired period - 1
    TIMER_A0->CCR[0] = (TIMER_A0_PERIOD - 1);

    //Enable the timer interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;

    //Enable the interrupt in the Interrupt Set Enable Register (ISER)
    NVIC->ISER[0] |= (0x00000001 << TA0_0_IRQn);

    //Assign an interrupt priority register
    NVIC->IP[(TA0_0_IRQn)] = ( (NVIC->IP[(TA0_0_IRQn)] & 0x00) | (2 << 5));

    //Expansion register clock division
    TIMER_A0->EX0 = TIMER_A_EX0_IDEX__8;

    //Input clock = SMCLK, start in up mode, input clock divider
    TIMER_A0->CTL = (TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_ID__8);
}


void TA0_0_IRQHandler(void){
    //Clear interrupt flag
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    //Toggles LEDs
    if (currLEDisRGB){
        TOGGLE_BLUE_LED;
        TURN_OFF_RED_LED;
    }
    else{
        TOGGLE_RED_LED;
        TURN_OFF_BLUE_LED;
    }
}
