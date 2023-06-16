#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "msp.h"
//change port and bit for verification
#define PUSHBUTTON_S1_BIT                       1
#define PUSHBUTTON_S1_MASK                      (0x01 << PUSHBUTTON_S1_BIT)
#define PUSHBUTTON_S1_PORT                      P1
#define PUSHBUTTON_S1_IN                        PUSHBUTTON_S1_PORT->IN
#define SET_PUSHBUTTON_S1_TO_AN_INPUT           PUSHBUTTON_S1_PORT->DIR &= ~PUSHBUTTON_S1_MASK
#define ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S1   PUSHBUTTON_S1_PORT->REN |= PUSHBUTTON_S1_MASK
#define SELECT_PULL_UP_RESISTORS_S1             PUSHBUTTON_S1_PORT->OUT |= PUSHBUTTON_S1_MASK

#define PUSHBUTTON_S2_BIT                       1
#define PUSHBUTTON_S2_MASK                      (0x01 << PUSHBUTTON_S2_BIT)
#define PUSHBUTTON_S2_PORT                      P4
#define PUSHBUTTON_S2_IN                        PUSHBUTTON_S2_PORT->IN
#define SET_PUSHBUTTON_S2_TO_AN_INPUT           PUSHBUTTON_S2_PORT->DIR &= ~PUSHBUTTON_S2_MASK
#define ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S2   PUSHBUTTON_S2_PORT->REN |= PUSHBUTTON_S2_MASK
#define SELECT_PULL_UP_RESISTORS_S2             PUSHBUTTON_S2_PORT->OUT |= PUSHBUTTON_S2_MASK

#endif