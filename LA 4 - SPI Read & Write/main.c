#include "msp.h"
#include "Defines.h"
#include <stdbool.h>
#include "Clock.h"
#include "LED.h"
#include "Timer32.h"
#include "SPI.h"
#include "serial_flash.h"

// Used with the LFSR function
#define DIVISOR_POLYNOMIAL 0xB400
#define INITIAL_LFSR_STATE 0x0001
#define LFSR_TYPE uint16_t

// Function prototypes
LFSR_TYPE LFSR(LFSR_TYPE CurrentState, LFSR_TYPE Divisor);
void CheckSumFlashMemoryTest(SerialFlash *Component, uint8_t ProgrammingMethod,
        uint8_t EraseMode, LFSR_TYPE InitialLFSRState, LFSR_TYPE* LFSRStateAfterRead,
        LFSR_TYPE* LFSRStateAfterWrite);

#define NUMBER_OF_TESTS 6
void main(void){
    bool ClockSystemError = false;
    uint16_t delay = 250;       // Represents time delay in increments of unit time
    uint16_t resolution = ONE_MILLISECOND_DELAY; // Represents period of unit time

    volatile uint8_t TestNumber;
    volatile uint8_t NumberOfTestsPassed;
    SerialFlash U2, U1;
    volatile uint16_t ID_U1;
    volatile uint16_t ID_U2;
    volatile uint8_t StatusRegister_U1;
    volatile uint8_t StatusRegister_U2;
    LFSR_TYPE LFSRStateAfterRead_U1,LFSRStateAfterWrite_U1;
    LFSR_TYPE LFSRStateAfterRead_U2,LFSRStateAfterWrite_U2;
    volatile uint8_t TestHasNotFailed[NUMBER_OF_TESTS];

    // Stop the watchdog timer.
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Configure the clock system. If there is an error,
    // continue trying to initialize.
    ClockSystemError = InitializeClockSystem();
    while (ClockSystemError) {
        ClockSystemError = InitializeClockSystem();
    }

    InitializeLEDPortPins();
    InitializeUSCIB0ForSPI();
    InitializeSerialFlashPortPins();

    // Initialize serial flash components
    InitializeSerialFlashComponent(&U2, (uint8_t *) &(CHIP_ENABLE_U2_OUT), CHIP_ENABLE_U2_MASK);
    InitializeSerialFlashComponent(&U1, (uint8_t *) &(CHIP_ENABLE_U1_OUT), CHIP_ENABLE_U1_MASK);

    // Begin by assuming all tests have not failed.
    for (TestNumber = 0; TestNumber < NUMBER_OF_TESTS; TestNumber++) TestHasNotFailed[TestNumber] = TRUE;
    TestNumber = 0;

    // Begin by reading the ID for each flash memory. The value should be 0xBF48 for both.
    ID_U1 = ReadFlashMemoryID(&U1);
    if (ID_U1 != 0xBF48) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;
    ID_U2 = ReadFlashMemoryID(&U2);
    if (ID_U2 != 0xBF48) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;

    // Next, turn on block protection, and then read status register for each flash memory.
    // The value for each should be 0x0C.
    SetBlockProtection(FULL, &U1);
    SetBlockProtection(FULL, &U2);
    StatusRegister_U1 = ReadFlashMemoryStatusRegister(&U1);
    if (StatusRegister_U1 != 0x0C) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;
    StatusRegister_U2 = ReadFlashMemoryStatusRegister(&U2);
    if (StatusRegister_U2 != 0x0C) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;

    // Next, write pseudo-random data to each flash, and then read it to confirm flash read/write.
    CheckSumFlashMemoryTest(&U1,BYTE_PROGRAM,CHIP_ERASE,INITIAL_LFSR_STATE,
            &LFSRStateAfterRead_U1,&LFSRStateAfterWrite_U1);
    if (LFSRStateAfterRead_U1 != LFSRStateAfterWrite_U1) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;

    // Note that for this test the initial state of the LFSR is the final state of the LFSR from
    // the previous test.
    CheckSumFlashMemoryTest(&U2,AAI_PROGRAM,~CHIP_ERASE,LFSRStateAfterWrite_U1,
                &LFSRStateAfterRead_U2,&LFSRStateAfterWrite_U2);
    if (LFSRStateAfterRead_U2 != LFSRStateAfterWrite_U2) {
        TestHasNotFailed[TestNumber] = FALSE;
    }
    TestNumber++;

    // If all tests were successful, then indicate this by toggling the green LED.
    // Otherwise, toggle the red LED.
    for (NumberOfTestsPassed = 0, TestNumber = 0; TestNumber < NUMBER_OF_TESTS; TestNumber++) {
        NumberOfTestsPassed += TestHasNotFailed[TestNumber];
    }
    __no_operation(); // set a breakpoint here to view results in debugger

    while (TRUE) {
        if (NumberOfTestsPassed == NUMBER_OF_TESTS) {
            TOGGLE_RBG_G;
        }
        else {
            TOGGLE_RBG_R;
        }
        TimeDelay(delay,resolution);
    }

}

LFSR_TYPE LFSR(LFSR_TYPE CurrentState, LFSR_TYPE Divisor)
{
    volatile LFSR_TYPE dividend;
    volatile LFSR_TYPE lsb;

    /* In general, the polynomial exponents increase from left to right, whereas
     * the bit numbering increases from right to left:
     *
     * Bit:  1 1 1 1 1 1
     *       5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
     *
     *       b b b b b b b b b b b b b b b b, b = 0,1
     *
     * Poly: 0 1 2 3 4 5 6 7 8 9 1 1 1 1 1 1
     *                           0 1 2 3 4 5
     *
     * Thus, for the polynomial, 1 + x^11 + x^13 + x^14 + x^16:
     * Bit:  1 1 1 1 1 1
     *       5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
     *
     *       1 0 0 0 0 0 0 0 0 0 0 1 0 1 1 0
     *
     * Poly: 0 1 2 3 4 5 6 7 8 9 1 1 1 1 1 1
     *                           0 1 2 3 4 5
     */
    dividend = CurrentState;

    /* Get the value of the Least Significant Bit (LSB). Then, right-shift the dividend.
     *
     * The LSB represents the coefficient of x^(n-1) in the polynomial, and
     * right-shifting corresponds to multiplying the polynomial by x.  So, these
     * two operations essentially determine if the coefficient for x^n equals 1.
     */
    lsb = dividend & 1;
    dividend >>= 1;

    /* If the LSB == 1, then perform polynomial division using the XOR
     * operation (which corresponds to bitwise addition).
     *
     * The LSB equal to 1 corresponds to the coefficient for x^n equal to 1,
     * which means that the Divisor should be added to perform polynomial division.
     *
     * For example, if the dividend = x^15 (that is, 0x0001), then right-shifting
     * (that is, multiplying by x) produces dividend = x^16.  So, perform polynomial
     * division:
     *                                 1
     *                                -----
     * 1 + x^11 + x^13 + x^14 + x^16 | x^16
     *                              +  x^16 + 1 + x^11 + x^13 + x^14
     *                                 -----------------------------
     *                                 1 + x^11 + x^13 + x^14
     */
    dividend ^= (-lsb) & Divisor;

    return dividend;
}

#define BUFFER_SIZE 2
void CheckSumFlashMemoryTest(SerialFlash *Component, uint8_t ProgrammingMethod,
        uint8_t EraseMode, LFSR_TYPE InitialLFSRState, LFSR_TYPE* LFSRStateAfterRead,
        LFSR_TYPE* LFSRStateAfterWrite)
{
    volatile char ReadWriteBuffer[BUFFER_SIZE];

    uint8_t i;
    uint32_t FlashAddress;
    uint8_t ByteDataValue;

    // Begin by erasing entire flash memory, writing pseudo-random patterns to the
    // entire memory, and then reading back to confirm the write operations.
    SetBlockProtection(NONE, Component);

    // Erase the flash memory, which must be performed before writing to flash.
    if (EraseMode == CHIP_ERASE) {
        ChipEraseFlashMemory(Component);
    }
    else {

        // Erase half of the flash using sector-erase.
        for (FlashAddress = 0; FlashAddress < (FLASH_MEMORY_SIZE>>1); FlashAddress+=FLASH_SECTOR_SIZE) {
            SectorBlockEraseFlashMemory(FlashAddress,Component,SECTOR_ERASE);
        }

        // Erase the remaining half using block-erase.
        for (FlashAddress = (FLASH_MEMORY_SIZE>>1); FlashAddress < FLASH_MEMORY_SIZE; FlashAddress+=FLASH_BLOCK_SIZE) {
            SectorBlockEraseFlashMemory(FlashAddress,Component,BLOCK_ERASE);
        }
    }

    // The programming method is selected by the value of ProgrammingMethod, and can either be
    // Auto-Address Increment (AAI, ProgrammingMethod = AAI_PROGRAM) or byte programming
    // (ProgrammingMethod = BYTE_PROGRAM).
    *LFSRStateAfterWrite = InitialLFSRState;
    if (ProgrammingMethod == BYTE_PROGRAM) {
        for (FlashAddress = 0; FlashAddress < FLASH_MEMORY_SIZE; FlashAddress+=2) {
            ByteDataValue = (uint8_t) *LFSRStateAfterWrite;
            ByteProgramFlashMemory(FlashAddress,ByteDataValue,Component);
            ByteDataValue = (uint8_t) (*LFSRStateAfterWrite >> 8);
            ByteProgramFlashMemory((FlashAddress+1),ByteDataValue,Component);
            *LFSRStateAfterWrite = LFSR(*LFSRStateAfterWrite,DIVISOR_POLYNOMIAL);
        }
    }
    else {
        for (FlashAddress = 0; FlashAddress < FLASH_MEMORY_SIZE; FlashAddress+=2) {
            ReadWriteBuffer[0] = (uint8_t) *LFSRStateAfterWrite;
            ReadWriteBuffer[1] = (uint8_t) (*LFSRStateAfterWrite >> 8);
            AAIProgramFlashMemory(FlashAddress,(uint8_t *) ReadWriteBuffer,2,Component);
            *LFSRStateAfterWrite = LFSR(*LFSRStateAfterWrite,DIVISOR_POLYNOMIAL);
        }
    }

    for (i = 0; i < BUFFER_SIZE; i++) ReadWriteBuffer[i] = (char) 0; // Clear the Read/Write buffer.

    // Initialize *LFSRStateAfterRead (which represents the checksum) to the initial LFSR state.
    *LFSRStateAfterRead = InitialLFSRState;
    for (FlashAddress = 0; FlashAddress < FLASH_MEMORY_SIZE; FlashAddress+=2) {
        ReadFlashMemory(FlashAddress, (uint8_t *) ReadWriteBuffer,2,Component,READ);

        // If the value read from memory (in ReadWriteBuffer) is equal to the current value of *LFSRStateAfterRead,
        // then XORing them together produces zero, which when added to the current value of *LFSRStateAfterRead
        // will result in no change to the current value of *LFSRStateAfterRead.
        *LFSRStateAfterRead = *LFSRStateAfterRead +
                (LFSR_TYPE)(*LFSRStateAfterRead ^ ((ReadWriteBuffer[1] << 8)|ReadWriteBuffer[0]));

        // Now, determine the next LFSR State
        *LFSRStateAfterRead = LFSR(*LFSRStateAfterRead,DIVISOR_POLYNOMIAL);
    }
}
