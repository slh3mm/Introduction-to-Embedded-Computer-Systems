#ifndef MANCHESTERPHASEENCODINGPACKET_CONFIGURE_H_
#define MANCHESTERPHASEENCODINGPACKET_CONFIGURE_H_

#include "msp.h"

// The following value should be used to establish the system clock frequency,
// the period for each transmitted bit.
#define TACCR0_VALUE			500
#define BIT_TOLERANCE			100

// This define is used to disable the transmit/receive functionality
// which then causes the user input to be echoed to the terminal.
#define TRANSMIT_RECEIVE_ENABLED 1
#define TRANSMIT_RECEIVE_DISABLED 0
#define TRANSMIT_RECEIVE_STATUS TRANSMIT_RECEIVE_ENABLED

// This define is used to configure the device as either a
// receiver or transmitter (which is what is also used for self-test mode).
#define RECEIVE                 0
#define TRANSMIT_OR_SELFTEST    1
#define TRANSMIT_RECEIVE_MODE TRANSMIT_OR_SELFTEST

#define SOT 0x80 // Start Of Transmission (SOT) character
#define EOT 0x04 // End Of Transmission (EOT) character

// Defines for the packet information.
#define TXRX_DATA_TYPE          char
#define TXRX_DATA_TYPE_LENGTH   8
#define TXRX_BUFFER_LENGTH      2
#define INTERWORD_DELAY         10	// This is in units of 0.5*(bit period)
#define SIZE_OF_RCV_QUE         16
#define BIT_TO_BE_TRANSMITTED   0x80 // Used to mask the bit to transmit, either MSB or LSB

// The following defines are for bit width definitions
// The underlying assumptions are
//      The capture timers are updated with a 1 bit period resolution
//      A full bit time is 2*TACCR0_VALUE nominal
#define VALID_HALF_BIT_MIN      (TACCR0_VALUE-BIT_TOLERANCE)
#define VALID_HALF_BIT_MAX      (TACCR0_VALUE+BIT_TOLERANCE)
#define VALID_FULL_BIT_MIN      (2*TACCR0_VALUE-BIT_TOLERANCE)
#define VALID_FULL_BIT_MAX      (2*TACCR0_VALUE+BIT_TOLERANCE)
#define MISSING_EDGE_TIMEOUT    (3*TACCR0_VALUE)

// Transmitter port pin defines
#define TX_DATA_BIT                 5
#define TX_DATA_MASK				(0x01 << TX_DATA_BIT)
#define TX_DATA_PORT                P2
#define TX_DATA_OUT                 TX_DATA_PORT->OUT
#define TX_DATA_DIR					TX_DATA_PORT->DIR
#define SET_TX_DATA_AS_AN_OUTPUT	TX_DATA_DIR |= TX_DATA_MASK
#define SET_TX_DATA_HIGH			TX_DATA_OUT |= TX_DATA_MASK
#define SET_TX_DATA_LOW				TX_DATA_OUT &= ~TX_DATA_MASK
#define MPE_TRANSMIT_TIMER			TIMER_A1
#define TRANSMIT_TIMER_IRQ			TA1_0_IRQHandler
#define TRANSMIT_TIMER_PR           10
#define TRANSMIT_TIMER_PRIORITY     2
#define TRANSMIT_TIMER_ISER			0
#define TRANSMIT_TIMER_CCR			0

// Second transmit pin needed for self-test mode.
#define TX2_DATA_BIT                 0
#define TX2_DATA_MASK                (0x01 << TX2_DATA_BIT)
#define TX2_DATA_PORT                P3
#define TX2_DATA_OUT                 TX2_DATA_PORT->OUT
#define TX2_DATA_DIR                 TX2_DATA_PORT->DIR
#define SET_TX2_DATA_AS_AN_OUTPUT    TX2_DATA_DIR |= TX2_DATA_MASK
#define SET_TX2_DATA_HIGH            TX2_DATA_OUT |= TX2_DATA_MASK
#define SET_TX2_DATA_LOW             TX2_DATA_OUT &= ~TX2_DATA_MASK

// Receiver port pin defines. Note that two port pins are always
// needed for the receiver.
#define RECEIVE_RISING_BIT          4
#define RECEIVE_RISING_MASK			(0x01 << RECEIVE_RISING_BIT)
#define RECEIVE_RISING_PORT         P2
#define RECEIVE_FALLING_BIT			6
#define RECEIVE_FALLING_MASK		(0x01 << RECEIVE_FALLING_BIT)
#define RECEIVE_FALLING_PORT        P2
#define MPE_RECEIVE_TIMER			TIMER_A0
#define RECEIVE_TIMER_IRQ_0			TA0_0_IRQHandler
#define RECEIVE_TIMER_IRQ_N			TA0_N_IRQHandler
#define RECEIVE_TIMER_PR_0			8
#define RECEIVE_TIMER_PR_N			9
#define RECEIVE_TIMER_PRIORITY      2
#define RECEIVE_TIMER_ISER			0
#define RECEIVE_TIMER_CCR_RISING    1
#define RECEIVE_TIMER_CCR_FALLING	3

// Debug for Transmit Clock Phase
#define TRANSMIT_CLK_PHASE_BIT					7
#define TRANSMIT_CLK_PHASE_MASK					(0x01 << TRANSMIT_CLK_PHASE_BIT)
#define TRANSMIT_CLK_PHASE_PORT                 P1
#define TRANSMIT_CLK_PHASE_OUT                  TRANSMIT_CLK_PHASE_PORT->OUT
#define TRANSMIT_CLK_PHASE_DIR					TRANSMIT_CLK_PHASE_PORT->DIR
#define SET_TRANSMIT_CLK_PHASE_PIN_AS_AN_OUTPUT	TRANSMIT_CLK_PHASE_DIR |= TRANSMIT_CLK_PHASE_MASK
#define SET_TRANSMIT_CLK_PHASE_PIN_HIGH			TRANSMIT_CLK_PHASE_OUT |= TRANSMIT_CLK_PHASE_MASK
#define SET_TRANSMIT_CLK_PHASE_PIN_LOW			TRANSMIT_CLK_PHASE_OUT &= ~TRANSMIT_CLK_PHASE_MASK

// This pin was used on the actual radio hardware to select between transmit and receive
#define TR_BIT                      6
#define TR_MASK                     (0x01 << TR_BIT)
#define TR_PORT                     P1
#define TR_OUT                      TR_PORT->OUT
#define TR_DIR                      TR_PORT->DIR
#define SET_TR_AS_AN_OUTPUT         TR_DIR |= TR_MASK
#define ENABLE_RECEIVE              TR_OUT |= TR_MASK
#define ENABLE_TRANSMIT             TR_OUT &= ~TR_MASK

#endif /* MANCHESTERPHASEENCODINGPACKET_CONFIGURE_H_ */
