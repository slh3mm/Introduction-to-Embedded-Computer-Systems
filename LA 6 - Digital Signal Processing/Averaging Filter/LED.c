#include "LED.h"

void InitializeLEDPortPins(void)
{
    // Initialize LED values and port pin directions.
    TURN_OFF_RED_LED;
    SET_RED_LED_AS_AN_OUTPUT;
    TURN_OFF_RBG_R;
    SET_RBG_R_AS_AN_OUTPUT;
    TURN_OFF_RBG_G;
    SET_RBG_G_AS_AN_OUTPUT;
    TURN_OFF_RBG_B;
    SET_RBG_B_AS_AN_OUTPUT;
}

