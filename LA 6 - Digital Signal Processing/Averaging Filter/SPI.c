#include "SPI.h"

void InitializeUSCIB0ForSPI()
{
#if (SPI_MODE == SPI_SOFTWARE_MODE)
    SET_USCIB0_MOSI_AS_AN_OUTPUT;
    SET_USCIB0_MISO_AS_AN_INPUT;
    ENABLE_PULL_RESISTORS;
    PULL_DOWN_RESISTORS;
    SET_USCIB0_CLK_AS_AN_OUTPUT;
    USCIB0_CLK_EQUAL_0;

    // Configure port pins for GPIO
    USCIB0_CLK_PORT->SEL1 &= ~USCIB0_CLK_MASK;
    USCIB0_CLK_PORT->SEL0 &= ~USCIB0_CLK_MASK;

    USCIB0_MOSI_PORT->SEL1 &= ~USCIBO_MOSI_MASK;
    USCIB0_MOSI_PORT->SEL0 &= ~USCIBO_MOSI_MASK;

    USCIB0_MISO_PORT->SEL1 &= ~USCIBO_MISO_MASK;
    USCIB0_MISO_PORT->SEL0 &= ~USCIBO_MISO_MASK;

#else
	// Software reset enabled. USCI logic held in reset state.
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST;

	// Initialize all USCI registers with UCSWRST = 1 (including UCxCTL1)

	// Select USCI SPI functionality.
    // bit15      UCCKPH = 1; data shifts in on first edge, out on following edge
    // bit14      UCCKPL = 0; clock is low when inactive
    // bit13      UCMSB = 1; MSB first
    // bit12      UC7BIT = 0; 8-bit data
    // bit11      UCMST = 1; master mode
    // bits10-9   UCMODEx = 0; 3-pin mode
    // bit8       UCSYNC = 1; synchronous mode
    // bits7-6    UCSSELx = 2; eUSCI clock SMCLK
    // bits5-2    reserved
    // bit1       UCSTEM = 0; UCSTE pin enables slave
    // bit0       UCSWRST = 1; reset enabled
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_CKPH + EUSCI_B_CTLW0_MSB + EUSCI_B_CTLW0_MST +
             EUSCI_B_CTLW0_SYNC + EUSCI_B_CTLW0_SSEL__SMCLK);
    EUSCI_B0->BRW = 7;     // fBitClock = fBRCLK/(UCBRx+1)
    EUSCI_B0->TXBUF = 0;    // initialize transmit buffer to 0

    // Configure port pin to receive output from USCI B0 clock.
    USCIB0_CLK_PORT->SEL1 &= ~USCIB0_CLK_MASK;
    USCIB0_CLK_PORT->SEL0 |= USCIB0_CLK_MASK;

	// Configure port pin to receive output from USCI B0 MOSI.
    USCIB0_MOSI_PORT->SEL1 &= ~USCIBO_MOSI_MASK;
    USCIB0_MOSI_PORT->SEL0 |= USCIBO_MOSI_MASK;


	// Configure port pin to receive output from USCI B0 MISO.
    USCIB0_MISO_PORT->SEL1 &= ~USCIBO_MISO_MASK;
    USCIB0_MISO_PORT->SEL0 |= USCIBO_MISO_MASK;
//    ENABLE_PULL_RESISTORS;
//    PULL_DOWN_RESISTORS;

#endif

    // Software reset disabled. USCI logic released for operation.
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
}


void SPISendByte(unsigned char SendValue)
{
#if (SPI_MODE == SPI_HARDWARE_MODE)
    while (EUSCI_B0->STATW & UCBUSY) {};  // 1) Wait for SPI to be idle (let previous frame finish)
    EUSCI_B0->TXBUF = SendValue;
#else
    uint8_t LocalSendValue = SendValue;
    uint8_t k;

    for (k = 0; k < 8; k++){

        // Assign a value to the MOSI based on the value of the MSB.
        if (LocalSendValue & 0x80) {
            USCIB0_MOSI_EQUAL_1;
        }
        else {
            USCIB0_MOSI_EQUAL_0;
        }

        // Left-shift local copy of data to send.
        LocalSendValue <<= 1;

        // Toggle SPI Clock: (HIGH XOR 1) -> LOW, and (LOW XOR 1) -> HIGH
        USCIB0_TOGGLE_CLK; USCIB0_TOGGLE_CLK;
    }
#endif
}

unsigned char SPIReceiveByte()
{
	uint8_t ReceiveValue = 0;

#if (SPI_MODE == SPI_HARDWARE_MODE)

    EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG0; // Clear interrupt flag.

    /* To receive data into the USCI in master mode, data must be written
	 * to TXBUF because receive and transmit operations operate concurrently.
	 */
	EUSCI_B0->TXBUF = 0x00;

	while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)); // wait for full receive buffer
	ReceiveValue = EUSCI_B0->RXBUF;
#else
    uint8_t k;

    for (k = 0; k < 8; k++) {

        // Left-shift the current value of ReceiveValue, and OR
        // the result with the value of MISO.
      ReceiveValue = (ReceiveValue << 1) | READ_BIT_FROM_SLAVE;

        // Toggle SPI Clock: (HIGH XOR 1) -> LOW, and (LOW XOR 1) -> HIGH
      USCIB0_TOGGLE_CLK; USCIB0_TOGGLE_CLK;
    }
#endif

	return ReceiveValue;
}

bool SPIBusy(void)
{
    bool ReturnValue = false;

#if (SPI_MODE == SPI_HARDWARE_MODE)
    if (EUSCI_B0->STATW & UCBUSY) {
        ReturnValue = true;
    }
#endif

    return ReturnValue;
}
