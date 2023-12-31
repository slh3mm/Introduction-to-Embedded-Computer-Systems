#ifndef USCI_SPI_H
#define USCI_SPI_H

#include "msp.h"
#include "Defines.h"

// USCI B0 Main-In-Secondary-Out (MISO)
#define USCIB0_MISO_BIT					7
#define USCIBO_MISO_MASK                (0x01 << USCIB0_MISO_BIT)
#define USCIB0_MISO_PORT                P1
#define USCIB0_MISO_IN				    USCIB0_MISO_PORT->IN
#define USCIB0_MISO_DIR					USCIB0_MISO_PORT->DIR
#define SET_USCIB0_MISO_AS_AN_INPUT		USCIB0_MISO_DIR &= ~USCIBO_MISO_MASK
#define ENABLE_PULL_RESISTORS           USCIB0_MISO_PORT->REN |= USCIBO_MISO_MASK
#define PULL_UP_RESISTORS               USCIB0_MISO_PORT->OUT |= USCIBO_MISO_MASK
#define PULL_DOWN_RESISTORS             USCIB0_MISO_PORT->OUT &= ~USCIBO_MISO_MASK
#define READ_BIT_FROM_SLAVE             READ_PORT_PIN(USCIB0_MISO_IN,USCIBO_MISO_MASK,USCIB0_MISO_BIT)

// USCI B0 Main-Out-Secondary-In (MOSI)
#define USCIB0_MOSI_BIT					6
#define USCIBO_MOSI_MASK                (0x01 << USCIB0_MOSI_BIT)
#define USCIB0_MOSI_PORT                P1
#define USCIB0_MOSI_OUT				    USCIB0_MOSI_PORT->OUT
#define USCIB0_MOSI_DIR 				USCIB0_MOSI_PORT->DIR
#define SET_USCIB0_MOSI_AS_AN_OUTPUT	USCIB0_MOSI_DIR |= USCIBO_MOSI_MASK
#define USCIB0_MOSI_EQUAL_1             USCIB0_MOSI_OUT |= USCIBO_MOSI_MASK
#define USCIB0_MOSI_EQUAL_0             USCIB0_MOSI_OUT &= ~USCIBO_MOSI_MASK

// Serial Clock
#define USCIB0_CLK_BIT					5
#define USCIB0_CLK_MASK                 (0x01 << USCIB0_CLK_BIT)
#define USCIB0_CLK_PORT                 P1
#define USCIB0_CLK_OUT				    USCIB0_CLK_PORT->OUT
#define USCIB0_CLK_DIR					USCIB0_CLK_PORT->DIR
#define SET_USCIB0_CLK_AS_AN_OUTPUT	    USCIB0_CLK_DIR |= USCIB0_CLK_MASK
#define USCIB0_CLK_EQUAL_0              USCIB0_CLK_OUT &= ~USCIB0_CLK_MASK
#define USCIB0_TOGGLE_CLK               USCIB0_CLK_OUT ^= USCIB0_CLK_MASK

void InitializeUSCIB0ForSPI();
void SPISendByte(uint8_t SendValue);
uint8_t SPIReceiveByte();

#endif
