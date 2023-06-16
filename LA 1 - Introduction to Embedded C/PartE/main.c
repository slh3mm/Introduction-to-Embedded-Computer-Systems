#include "msp.h"
#include "LED.h "

#define TRUE 1
#define FALSE 0
#define MASK 0x01
#define NUMBER_OF_LEDS 2

void TimeDelay(uint16_t delay, uint16_t resolution);

void main(void){
    //Volatile places the value in ram, also makes sure that the O2 flag doesn't over optimize the code, so the for loops can run
    uint16_t delay = 250;
    uint16_t resolution = 250;
    LED LEDArray[NUMBER_OF_LEDS];
    uint32_t i;
    void (*LEDStateFunction)(LED*) = &ToggleLEDState;

    //Stop the watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    InitializeLEDPortPins();
    LEDArray[0].LEDPort = (uint8_t *) &(P2->OUT);
    LEDArray[0].LEDBit = (uint8_t) (0x01 << 1);
    LEDArray[1].LEDPort = (uint8_t *) &(P1->OUT);
    LEDArray[1].LEDBit = (uint8_t) (0x01 << 0);
    LEDArray[0].CurrentState = On;
    LEDArray[1].CurrentState = On;

    //Infinite Loop
    while(TRUE){
        for (i = 0; i < NUMBER_OF_LEDS; i++){
            (*LEDStateFunction)(&(LEDArray[i]));
        }
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
