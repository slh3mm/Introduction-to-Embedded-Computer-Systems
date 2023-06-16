#include "TimerA0.h"
#include "msp.h"
#include "LED.h"
#include <stdbool.h>

extern int numInterrupts;
extern int currTime;
extern int prevTime;
extern int elapsedTime;

void InitializeTimerA0(void){

    //Set the P2.4 Control Bits
    PORT2_IN;
    PORT2_INPUT;
    PORT2_SEL0;
    PORT2_SEL1;

    //Stop timer and clear the registers
    TIMER_A0->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);

    //Enable the timer interrupt
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE | TIMER_A_CCTLN_CM_3  | TIMER_A_CCTLN_SCS  | TIMER_A_CCTLN_CAP;

    //Enable the interrupt in the Interrupt Set Enable Register (ISER)
    NVIC->ISER[0] |= (0x00000001 << TA0_N_IRQn);

    //Assign an interrupt priority register
    //NVIC->IP[(TA0_N_IRQn)] = ( (NVIC->IP[(TA0_0_IRQn)] & 0x00) | (2 << 5));
                           //the 2 here is what the priority is, we will always shift it
                           //by 5 to make sure the bit is in the right index
    NVIC->IP[(TA0_N_IRQn)] = 2 << 5;

    //Expansion register clock division
    TIMER_A0->EX0 = TIMER_A_EX0_IDEX__3;

    //Input clock = SMCLK, start in up mode, input clock divider
    TIMER_A0->CTL = (TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS | TIMER_A_CTL_ID__4);
}

void TA0_N_IRQHandler(void){
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    numInterrupts += 1;
    currTime = TIMER_A0->CCR[1];
    elapsedTime = currTime - prevTime;
    prevTime = currTime;
}

