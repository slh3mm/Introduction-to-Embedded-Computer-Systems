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
    InitializePushButtonPortPin();
    InitializeTimerA0();

    EnableInterrupts();
    while (TRUE) {
        WaitForInterrupt();
   }

}
