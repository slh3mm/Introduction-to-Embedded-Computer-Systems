#include "msp.h"
#include "LED.h "

#define TRUE 1
#define FALSE 0
#define MASK 0x01

void TimeDelay(uint16_t delay, uint16_t resolution);

void main(void){
    //Volatile places the value in ram, also makes sure that the O2 flag doesn't over optimize the code, so the for loops can run
    uint16_t delay = 250;
    uint16_t resolution = 250;
    LED RedLED, GreenLED;

    //Stop the watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    InitializeLEDPortPins();
    GreenLED.LEDPort = (uint8_t *) &(P2->OUT);
    GreenLED.LEDBit = (uint8_t) (0x01 << 1);
    GreenLED.CurrentState = Off;
    RedLED.LEDPort = (uint8_t *) &(P1->OUT);
    RedLED.LEDBit = (uint8_t) (0x01 << 0);
    RedLED.CurrentState = Off;


    //Infinite Loop
    while(TRUE){
        ToggleLEDState(&RedLED);
        ToggleLEDState(&GreenLED);
        TimeDelay(delay,resolution);
    }
}

void TimeDelay(uint16_t delay, uint16_t resolution){
    volatile uint16_t i,j;

    for (i = 0; i < delay; i++){
        for (j = 0; j < resolution; j++){

        }
    }
}
