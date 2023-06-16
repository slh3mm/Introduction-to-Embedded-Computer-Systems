#include "LED.h"

void InitializeLEDPortPins(void){
    TURN_OFF_GREEN_LED;
    SET_GREEN_LED_AS_AN_OUTPUT;
    TURN_OFF_RED_LED;
    SET_RED_LED_AS_AN_OUTPUT;
}

void ToggleLEDState(LED* CurrentLED){
    if (CurrentLED->CurrentState == On){
        *(CurrentLED->LEDPort) &= ~(CurrentLED->LEDBit);
        CurrentLED->CurrentState = Off;
    }
    else{
        *(CurrentLED->LEDPort) |= CurrentLED->LEDBit;
        CurrentLED->CurrentState = On;
    }
}
