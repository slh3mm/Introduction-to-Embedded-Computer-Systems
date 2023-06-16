    #include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "Defines.h"
#include "Clock.h"
#include "CortexM.h"
#include "LED.h"
#include "pushbutton.h"
#include "TimerA0.h"
#include "Timer32.h"

// Function prototypes

// Global variables
uint16_t delay = 250;
uint16_t resolution = ONE_MILLISECOND_DELAY;
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
    //InitializeTimerA0();

    while (TRUE) {
        TOGGLE_GREEN_LED;
        TimeDelay(delay,resolution);

    }

}
