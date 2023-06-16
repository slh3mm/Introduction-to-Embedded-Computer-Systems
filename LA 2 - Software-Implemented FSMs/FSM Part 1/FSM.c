# include "FSM.h"

const FSMState NextStateTable[8][2] =
{
//Active                Inactive
 {BothLEDsOffWaiting,   BothLEDsOff},
 {BothLEDsOffWaiting,   RedOffGreenOn},
 {RedOffGreenOnWaiting, RedOffGreenOn},
 {RedOffGreenOnWaiting, RedOnGreenOff},
 {RedOnGreenOffWaiting, RedOnGreenOff},
 {RedOnGreenOffWaiting, BothLEDsOn},
 {BothLEDsOnWaiting,    BothLEDsOn},
 {BothLEDsOnWaiting,    BothLEDsOff}
};

void InitializeFSM(FSMType *FSM)
{
    FSM->CurrentState = BothLEDsOff;
}

FSMState NextStateFunction(FSMType *FSM)
{
    uint8_t curr = (uint8_t) FSM->CurrentState;
    return NextStateTable[curr][FSM->CurrentInput];
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
