#include "pushbutton.h"
#include "LED.h"
#include <stdbool.h>

extern bool currLEDisRGB;


void InitializePushButtonPortPin (void){
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S1;
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S2;
    SELECT_PULL_UP_RESISTORS_S1;
    SELECT_PULL_UP_RESISTORS_S2;
    SET_PUSHBUTTON_S1_TO_AN_INPUT;
    SET_PUSHBUTTON_S2_TO_AN_INPUT;

    RISING_EDGE_INTERRUPTS_S1;
    ENABLE_SENSOR_IN_INTERRUPTS_S1;
    RISING_EDGE_INTERRUPTS_S2;
    ENABLE_SENSOR_IN_INTERRUPTS_S2;


    NVIC->ISER[1] |= (0x00001 << 3);
    NVIC->IP[(TA0_0_IRQn)] = ( (NVIC->IP[(TA0_0_IRQn)] & 0x00) | (2 << 5));

}

void PORT1_IRQHandler(void){
    CLEAR_SENSOR_IN_INTERRUPT_FLAG_S1;
    CLEAR_SENSOR_IN_INTERRUPT_FLAG_S2;
/*
    if (currLEDisRGB){
        currLEDisRGB = false;
        return;
    }
    else{
        currLEDisRGB = true;
    }*/
}
