#ifndef FSM_H_
#define FSM_H_

#include "Defines.h"
#include "LED.h"

// Type Definitions
typedef enum {
    BothLEDsOff, BothLEDsOffWaiting, RedOffGreenOn, RedOffGreenOnWaiting,
    RedOnGreenOff, RedOnGreenOffWaiting, BothLEDsOn, BothLEDsOnWaiting
} FSMState;

typedef struct {
    FSMState CurrentState;      // Current state of the FSM
    uint8_t CurrentInput;  // Current input of the FSM
} FSMType;

// Function Prototypes
void InitializeFSM(FSMType *FSM);
FSMState NextStateFunction(FSMType *FSM);
void OutputFunction(FSMType *FSM);

#endif /* FSM_H_ */
