#include <TransmitTimer.h>

// Global variables used within ISRs
extern TransmitterData Xmit1;

void (*TransmitTimerTask)(TransmitterData*);   // user function

// ***************** TransmitTimerInitialization ****************
// Activate transmit timer interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
// Outputs: none
void TransmitTimerInitialization(void(*task)(TransmitterData *xmit), uint16_t period)
{
	TransmitTimerTask = task;             // user function
    MPE_TRANSMIT_TIMER->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);

    MPE_TRANSMIT_TIMER->CCTL[TRANSMIT_TIMER_CCR] = TIMER_A_CCTLN_CCIE;
    MPE_TRANSMIT_TIMER->CCR[TRANSMIT_TIMER_CCR] = (period - 1);    // compare match value
    MPE_TRANSMIT_TIMER->EX0 = TIMER_A_EX0_IDEX__1;

    // interrupts enabled in the main program after all devices initialized
    PRIORITY_REGISTER(TRANSMIT_TIMER_PR,INTERRUPT_PRIORITY(TRANSMIT_TIMER_PRIORITY));
    NVIC_INTERRUPT_ENABLE(TRANSMIT_TIMER_ISER,TRANSMIT_TIMER_PR);    // enable interrupt 10 in NVIC

    MPE_TRANSMIT_TIMER->CTL = (TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__1 | TIMER_A_CTL_MC__UP);
}


// ------------TransmitTimerStop------------
// Deactivate the interrupt running a user task periodically.
// Input: none
// Output: none
void TransmitTimerStop(void)
{
    MPE_TRANSMIT_TIMER->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);
    NVIC_INTERRUPT_DISABLE(TRANSMIT_TIMER_ISER,TRANSMIT_TIMER_PR);
}

void TRANSMIT_TIMER_IRQ(void)
{
    MPE_TRANSMIT_TIMER->CCTL[TRANSMIT_TIMER_CCR] &= ~TIMER_A_CCTLN_CCIFG;
    (*TransmitTimerTask)(&Xmit1);             // execute user task
}
