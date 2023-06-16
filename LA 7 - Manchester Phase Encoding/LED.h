#ifndef LED_H
#define LED_H

#include "msp.h"

//  Red LED
#define RED_LED_BIT                 0
#define RED_LED_MASK                (0x01 << RED_LED_BIT)
#define RED_LED_PORT                P1
#define RED_LED_OUT                 RED_LED_PORT->OUT
#define RED_LED_DIR                 RED_LED_PORT->DIR
#define SET_RED_LED_AS_AN_OUTPUT    RED_LED_DIR |= RED_LED_MASK
#define TURN_ON_RED_LED             RED_LED_OUT |= RED_LED_MASK
#define TURN_OFF_RED_LED            RED_LED_OUT &= ~RED_LED_MASK
#define TOGGLE_RED_LED              RED_LED_OUT ^= RED_LED_MASK

//  RGB Red LED
#define RBG_R_BIT               0
#define RBG_R_MASK              (0x01 << RBG_R_BIT)
#define RBG_R_PORT              P2
#define RBG_R_OUT               RBG_R_PORT->OUT
#define RBG_R_DIR               RBG_R_PORT->DIR
#define SET_RBG_R_AS_AN_OUTPUT  RBG_R_DIR |= RBG_R_MASK
#define TURN_ON_RBG_R           RBG_R_OUT |= RBG_R_MASK
#define TURN_OFF_RBG_R          RBG_R_OUT &= ~RBG_R_MASK
#define TOGGLE_RBG_R            RBG_R_OUT ^= RBG_R_MASK

//  RGB Green LED
#define RBG_G_BIT               1
#define RBG_G_MASK              (0x01 << RBG_G_BIT)
#define RBG_G_PORT              P2
#define RBG_G_OUT               RBG_G_PORT->OUT
#define RBG_G_DIR               RBG_G_PORT->DIR
#define SET_RBG_G_AS_AN_OUTPUT  RBG_G_DIR |= RBG_G_MASK
#define TURN_ON_RBG_G           RBG_G_OUT |= RBG_G_MASK
#define TURN_OFF_RBG_G          RBG_G_OUT &= ~RBG_G_MASK
#define TOGGLE_RBG_G            RBG_G_OUT ^= RBG_G_MASK

//  RGB Blue LED
#define RBG_B_BIT               2
#define RBG_B_MASK              (0x01 << RBG_B_BIT)
#define RBG_B_PORT              P2
#define RBG_B_OUT               RBG_B_PORT->OUT
#define RBG_B_DIR               RBG_B_PORT->DIR
#define SET_RBG_B_AS_AN_OUTPUT  RBG_B_DIR |= RBG_B_MASK
#define TURN_ON_RBG_B           RBG_B_OUT |= RBG_B_MASK
#define TURN_OFF_RBG_B          RBG_B_OUT &= ~RBG_B_MASK
#define TOGGLE_RBG_B            RBG_B_OUT ^= RBG_B_MASK

typedef struct LED_Structure {
    uint8_t *LEDPort;
    uint8_t LEDBit;
} LED;

#define NUMBER_OF_LEDS 3

// Prototypes
void InitializeLEDPortPins(void);

#endif
