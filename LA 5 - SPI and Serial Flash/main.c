#include "msp.h"
#include "Defines.h"
#include <stdbool.h>
#include "Clock.h"
#include "LED.h"
#include "Timer32.h"
#include "SPI.h"
#include "serial_flash.h"

// Function prototypes

void main(void)
{
    bool ClockSystemError = false;
    uint16_t delay = 250;       // Represents time delay in increments of unit time
    uint16_t resolution = ONE_MILLISECOND_DELAY; // Represents period of unit time

    SerialFlash U2, U1;
    volatile uint16_t ID_U1;
    volatile uint16_t ID_U2;

    // Stop the watchdog timer.
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }

    InitializeLEDPortPins();
    InitializeUSCIB0ForSPI();
    InitializeSerialFlashPortPins();

    // Initialize serial flash components
    InitializeSerialFlashComponent(&U2, (uint8_t *) &(CHIP_ENABLE_U2_OUT), CHIP_ENABLE_U2_MASK);
    InitializeSerialFlashComponent(&U1, (uint8_t *) &(CHIP_ENABLE_U1_OUT), CHIP_ENABLE_U1_MASK);

    while (TRUE) {

        // This code is used to test SPISendByte() function.
//        SPISendByte(READ_ID);

        // This code is used to test SPIReceiveByte() function.
        ID_U1 = ReadFlashMemoryID(&U1);
        ID_U2 = ReadFlashMemoryID(&U2);
        if ((ID_U1 == 0xBF48) && (ID_U2 == 0xBF48)) {
            TOGGLE_RBG_G;
        }
        else {
            TOGGLE_RBG_R;
        }
        TimeDelay(delay,resolution);
    }

}
