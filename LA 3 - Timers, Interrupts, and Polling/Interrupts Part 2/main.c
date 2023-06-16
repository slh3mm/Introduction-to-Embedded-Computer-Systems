#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Defines.h"
#include "Clock.h"
#include "CortexM.h"
#include "LED.h"
#include "pushbutton.h"
#include "TimerA0.h"

// Function prototypes

// Global variables
bool currLEDisRGB = true;
void main(void)
{
    bool ClockSystemError = false;

    // Stop the watchdog timer.
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    DisableInterrupts();

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }

    // Initialize the hardware
    InitializeLEDPortPins();
    //InitializePushButtonPortPin();
    InitializeTimerA0();

    while (TRUE) {
        //Wait for Timer flag
        while ((TIMER_A0->CCTL[0] & 0x01) == 0x00){}
        //Clear flag
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

        //Toggle LEDs
        if (currLEDisRGB){
            TOGGLE_BLUE_LED;
            TURN_OFF_RED_RGB_LED;
            currLEDisRGB = false;
        }
        else{
            TOGGLE_RED_RGB_LED;
            TURN_OFF_BLUE_LED;
            currLEDisRGB = true;
        }
    }

}
