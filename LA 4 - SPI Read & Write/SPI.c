#include "SPI.h"

void InitializeUSCIB0ForSPI()
{
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
}


void SPISendByte(uint8_t SendValue) //Pass-by-value
{
    uint8_t k;
    //For debugging purposes you could make this voltatile to make it stored in memory and not a register
    uint8_t LocalSendValue = SendValue;

    for (k = 0; k < 8; k++){//8 bits in a byte

        // Assign a value to the MOSI based on the value of the MSB.
        if (LocalSendValue & 0x80){
            USCIB0_MOSI_EQUAL_1;
        }
        else{
            USCIB0_MOSI_EQUAL_0;
        }

        // Left-shift local copy of data to send.
        LocalSendValue <<= 1;

        // Toggle SPI Clock: (HIGH XOR 1) -> LOW, and (LOW XOR 1) -> HIGH
        USCIB0_TOGGLE_CLK;
        USCIB0_TOGGLE_CLK;
    }
}

uint8_t SPIReceiveByte()
{
    uint8_t ReceiveValue = 0;
    uint8_t k;
    uint8_t CurrentBitValue = 0;

    for (k = 0; k < 8; k++) {

        // Left-shift the current value of ReceiveValue, and OR
        // the result with the value of MISO.
        CurrentBitValue = READ_BIT_FROM_SLAVE;
        ReceiveValue = (ReceiveValue << 1) | CurrentBitValue;

        // Toggle SPI Clock: (HIGH XOR 1) -> LOW, and (LOW XOR 1) -> HIGH
        USCIB0_TOGGLE_CLK;
        USCIB0_TOGGLE_CLK;
    }

    return ReceiveValue;
}

