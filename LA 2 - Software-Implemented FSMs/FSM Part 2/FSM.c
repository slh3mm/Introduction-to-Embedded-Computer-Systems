# include "FSM.h"
#include "LED.h"

#define BOTH_LEDS_OFF               &FSMTable[0]
#define BOTH_LEDS_OFF_WAITING       &FSMTable[1]
#define RED_OFF_GREEN_ON            &FSMTable[2]
#define RED_OFF_GREEN_ON_WAITING    &FSMTable[3]
#define RED_ON_GREEN_OFF            &FSMTable[4]
#define RED_ON_GREEN_OFF_WAITING    &FSMTable[5]
#define BOTH_LEDS_ON                &FSMTable[6]
#define BOTH_LEDS_ON_WAITING        &FSMTable[7]

const FSMTableType FSMTable[8] = {
//                       Next State
// Current State         Active                     Inactive
 {BothLEDsOff,          {BOTH_LEDS_OFF_WAITING,     BOTH_LEDS_OFF}},
 {BothLEDsOffWaiting,   {BOTH_LEDS_OFF_WAITING,     RED_OFF_GREEN_ON}},
 {RedOffGreenOn,        {RED_OFF_GREEN_ON_WAITING,  RED_OFF_GREEN_ON}},
 {RedOffGreenOnWaiting, {RED_OFF_GREEN_ON_WAITING,  RED_ON_GREEN_OFF}},
 {RedOnGreenOff,        {RED_ON_GREEN_OFF_WAITING,  RED_ON_GREEN_OFF}},
 {RedOnGreenOffWaiting, {RED_ON_GREEN_OFF_WAITING,  BOTH_LEDS_ON}},
 {BothLEDsOn,           {BOTH_LEDS_ON_WAITING,      BOTH_LEDS_ON}},
 {BothLEDsOnWaiting,    {BOTH_LEDS_ON_WAITING,      BOTH_LEDS_OFF}}
};

void InitializeFSM(FSMType *FSM){
    FSM->CurrentState = BothLEDsOff;
}

FSMState NextStateFunction(FSMType *FSM){
    return FSMTable[(uint8_t) FSM->CurrentState].NextState[FSM->CurrentInput]->CurrentState;
}

void OutputFunction(FSMType *FSM)
{
    switch (FSM->CurrentState) {
    case BothLEDsOff:
        TURN_OFF_RED_LED;
        TURN_OFF_GREEN_LED;
        break;
    case BothLEDsOffWaiting:
        break;
    case RedOffGreenOn:
        TURN_OFF_RED_LED;
        TURN_ON_GREEN_LED;
        break;
    case RedOffGreenOnWaiting:
        break;
    case RedOnGreenOff:
        TURN_ON_RED_LED;
        TURN_OFF_GREEN_LED;
        break;
    case RedOnGreenOffWaiting:
        break;
    case BothLEDsOn:
        TURN_ON_RED_LED;
        TURN_ON_GREEN_LED;
        break;
    case BothLEDsOnWaiting:
    }
}


