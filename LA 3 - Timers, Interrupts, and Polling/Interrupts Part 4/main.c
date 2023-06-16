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
bool currLEDisRGB = false;
int currTime = 0;
int numInterrupts = 0;
int prevTime = 0;
int elapsedTime = 0;

void main(void){
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
    //InitializeLEDPortPins();
    InitializePushButtonPortPin();
    InitializeTimerA0();

    EnableInterrupts();
    while (TRUE){
        WaitForInterrupt();
        if (numInterrupts >= 2){
            numInterrupts = 0;
        }
    }

}
