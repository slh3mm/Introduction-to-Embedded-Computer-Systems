#include "LED.h"

void InitializeLEDPortPins(void){
    TURN_OFF_GREEN_LED;
    SET_GREEN_LED_AS_AN_OUTPUT;
    TURN_OFF_RED_LED;
    SET_RED_LED_AS_AN_OUTPUT;
}

void SetLEDState(LEDState RedLED, LEDState GreenLED){
    if (RedLED == On){
        TURN_ON_RED_LED;
    }
    else{
        TURN_OFF_RED_LED;
    }
    if (GreenLED == On){
        TURN_ON_GREEN_LED;
    }
    else{
        TURN_OFF_GREEN_LED;
    }

}
LEDState ToggleLEDState(LEDState LED){
    LEDState ReturnValue = LED;
    if (ReturnValue == On){
        ReturnValue = Off;
    }
    else{
        ReturnValue = On;
    }
    return ReturnValue;
}
