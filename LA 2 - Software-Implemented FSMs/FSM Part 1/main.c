#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "LED.h"
#include "pushbutton.h"
#include "FSM.h"
#include "Defines.h"
#include "Clock.h"

#define READ_PORT_PIN(PORT,MASK,BIT) ((PORT & MASK) >> BIT)
#define PUSHBUTTON_ACTIVE_STATE 0
bool InitializeClockSystem(void);
void InitializePushButtonPortPin (void){
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S1;
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S2;
    SELECT_PULL_UP_RESISTORS_S1;
    SELECT_PULL_UP_RESISTORS_S2;
    SET_PUSHBUTTON_S1_TO_AN_INPUT;
    SET_PUSHBUTTON_S2_TO_AN_INPUT;
}
/**
 * main.c
 */
void main(void)
{
    bool ClockSystemError = false;
    FSMType LED_Pushbutton_FSM;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	ClockSystemError = InitializeClockSystem();
	while (ClockSystemError){
	    ClockSystemError = InitializeClockSystem();
	}

	InitializeLEDPortPins();
	InitializePushButtonPortPin();

	InitializeFSM(&LED_Pushbutton_FSM);

	while (TRUE){
	    LED_Pushbutton_FSM.CurrentInput = READ_PORT_PIN(PUSHBUTTON_S1_IN, PUSHBUTTON_S1_MASK, PUSHBUTTON_S1_BIT);

	    OutputFunction(&LED_Pushbutton_FSM);

	    LED_Pushbutton_FSM.CurrentState = NextStateFunction(&LED_Pushbutton_FSM);
	}
}
