#ifndef TRANSMITTIMER_H_
#define TRANSMITTIMER_H_

#include "msp.h"
#include "Defines.h"
#include "transmit.h"

// Function prototypes
void TransmitTimerInitialization(void(*task)(TransmitterData *xmit), uint16_t period);
void TransmitTimerStop(void);

#endif
