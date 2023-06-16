#include <ReceiveTimerInputCapture.h>

QueReturnErrors (*CaptureTaskRising)(EdgeType edge, uint16_t time);     // user function
QueReturnErrors (*CaptureTaskFalling)(EdgeType edge, uint16_t time);    // user function

//------------ReceiveTimerInputCaptureInitialization------------
// Initialize receive timer in edge time mode to request interrupts on
// the rising and falling edges.  The interrupt service routines acknowledge the interrupt and call
// a user function.
// Input: task0 is a pointer to a user function called for rising edges
//        task1 is a pointer to a user function called for falling edges
// Output: none
// Assumes: low-speed subsystem master clock is 12 MHz
void ReceiveTimerCaptureInitialization(QueReturnErrors (*RisingEdgeTask)(EdgeType edge, uint16_t time),
		QueReturnErrors (*FallingEdgeTask)(EdgeType edge, uint16_t time))
{
	CaptureTaskRising = RisingEdgeTask;            // user function
	CaptureTaskFalling = FallingEdgeTask;            // user function

	CONFIGURE_RECEIVE_RISING_AS_AN_INPUT;
	CONFIGURE_RECEIVE_FALLING_AS_AN_INPUT;

	CONFIGURE_RECEIVE_RISING_SEL1;
	CONFIGURE_RECEIVE_RISING_SEL0;
	CONFIGURE_RECEIVE_FALLING_SEL1;
	CONFIGURE_RECEIVE_FALLING_SEL0;

	//  NVIC_INTERRUPT_ENABLE(RECEIVE_TIMER_ISER,RECEIVE_TIMER_PR_0);
	//  PRIORITY_REGISTER(RECEIVE_TIMER_PR_0,INTERRUPT_PRIORITY(RECEIVE_TIMER_PRIORITY));

	NVIC_INTERRUPT_ENABLE(RECEIVE_TIMER_ISER,RECEIVE_TIMER_PR_N);
	PRIORITY_REGISTER(RECEIVE_TIMER_PR_N,INTERRUPT_PRIORITY(RECEIVE_TIMER_PRIORITY));

	MPE_RECEIVE_TIMER->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);
	MPE_RECEIVE_TIMER->CCTL[RECEIVE_TIMER_CCR_RISING] |= (TIMER_A_CCTLN_CM__RISING | TIMER_A_CCTLN_SCS
			| TIMER_A_CCTLN_CAP | TIMER_A_CCTLN_CCIE);
	MPE_RECEIVE_TIMER->CCTL[RECEIVE_TIMER_CCR_FALLING] |= (TIMER_A_CCTLN_CM__FALLING | TIMER_A_CCTLN_SCS
			| TIMER_A_CCTLN_CAP | TIMER_A_CCTLN_CCIE);
	MPE_RECEIVE_TIMER->EX0 = TIMER_A_EX0_IDEX__1;
	MPE_RECEIVE_TIMER->CTL |= (TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__1 | TIMER_A_CTL_MC__CONTINUOUS);
}

//void RECEIVE_TIMER_IRQ_0(void)
//{
//    CLEAR_CCR0_INTERRUPT_FLAG;
//}

void RECEIVE_TIMER_IRQ_N(void)
{
    if (RECEIVE_RISING_CCR_INTERRUPT_FLAG != 0){
    	CLEAR_RECEIVE_RISING_INTERRUPT_FLAG;
        (*CaptureTaskRising)(Rising, MPE_RECEIVE_TIMER->CCR[RECEIVE_TIMER_CCR_RISING]);      // execute user task
    }
    if (RECEIVE_FALLING_CCR_INTERRUPT_FLAG != 0){
    	CLEAR_RECEIVE_FALLING_INTERRUPT_FLAG;
        (*CaptureTaskFalling)(Falling, MPE_RECEIVE_TIMER->CCR[RECEIVE_TIMER_CCR_FALLING]); // execute user task
    }
}

void ReceiveTimerStop(void)
{
    MPE_RECEIVE_TIMER->CTL = (TIMER_A_CTL_MC__STOP | TIMER_A_CTL_CLR);
//    NVIC_INTERRUPT_DISABLE(RECEIVE_TIMER_ISER,RECEIVE_TIMER_PR_0);
    NVIC_INTERRUPT_DISABLE(RECEIVE_TIMER_ISER,RECEIVE_TIMER_PR_N);
}
