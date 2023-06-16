#ifndef SPI_SERIAL_FLASH_H_
#define SPI_SERIAL_FLASH_H_

#include "msp.h"
#include "SPI.h"

typedef struct SerialFlash_Structure {
    uint8_t *PortRegister;
    uint8_t BitMask;
} SerialFlash;

#define ENABLE_FLASH_MEMORY *(Component->PortRegister) &= ~Component->BitMask // Active low
#define DISABLE_FLASH_MEMORY *(Component->PortRegister) |= Component->BitMask

// Chip Enable for U1
#define CHIP_ENABLE_U1_BIT				3
#define CHIP_ENABLE_U1_MASK             (0x01 << CHIP_ENABLE_U1_BIT)
#define CHIP_ENABLE_U1_PORT             P4
#define CHIP_ENABLE_U1_OUT              CHIP_ENABLE_U1_PORT->OUT
#define CHIP_ENABLE_U1_DIR				CHIP_ENABLE_U1_PORT->DIR
#define SET_ENABLE_U1_AS_AN_OUTPUT		CHIP_ENABLE_U1_DIR |= CHIP_ENABLE_U1_MASK

// Chip Enable for U2
#define CHIP_ENABLE_U2_BIT              6
#define CHIP_ENABLE_U2_MASK             (0x01 << CHIP_ENABLE_U2_BIT)
#define CHIP_ENABLE_U2_PORT             P4
#define CHIP_ENABLE_U2_OUT              CHIP_ENABLE_U2_PORT->OUT
#define CHIP_ENABLE_U2_DIR              CHIP_ENABLE_U2_PORT->DIR
#define SET_ENABLE_U2_AS_AN_OUTPUT      CHIP_ENABLE_U2_DIR |= CHIP_ENABLE_U2_MASK

#define WRITE_PROTECT_BIT               5
#define WRITE_PROTECT_MASK              (0x01 << WRITE_PROTECT_BIT)
#define WRITE_PROTECT_PORT              P6
#define WRITE_PROTECT_OUT               WRITE_PROTECT_PORT->OUT
#define WRITE_PROTECT_DIR               WRITE_PROTECT_PORT->DIR
#define SET_WRITE_PROTECT_AS_AN_OUTPUT  WRITE_PROTECT_DIR |= WRITE_PROTECT_MASK
#define ENABLE_WRITE_PROTECT            WRITE_PROTECT_OUT &= ~WRITE_PROTECT_MASK // active low
#define DISABLE_WRITE_PROTECT           WRITE_PROTECT_OUT |= WRITE_PROTECT_MASK

#define HOLD_U1_BIT						4
#define HOLD_U1_MASK                    (0x01 << HOLD_U1_BIT)
#define HOLD_U1_PORT					P6
#define HOLD_U1_OUT                     HOLD_U1_PORT->OUT
#define HOLD_U1_DIR						HOLD_U1_PORT->DIR
#define SET_HOLD_U1_AS_AN_OUTPUT		HOLD_U1_DIR |= HOLD_U1_MASK
#define ENABLE_HOLD_U1					HOLD_U1_OUT &= ~HOLD_U1_MASK // active low
#define DISABLE_HOLD_U1					HOLD_U1_OUT |= HOLD_U1_MASK

#define HOLD_U2_BIT						6
#define HOLD_U2_MASK                    (0x01 << HOLD_U2_BIT)
#define HOLD_U2_PORT					P3
#define HOLD_U2_OUT                     HOLD_U2_PORT->OUT
#define HOLD_U2_DIR                     HOLD_U2_PORT->DIR
#define SET_HOLD_U2_AS_AN_OUTPUT		HOLD_U2_DIR |= HOLD_U2_MASK
#define ENABLE_HOLD_U2					HOLD_U2_OUT &= ~HOLD_U2_MASK // active low
#define DISABLE_HOLD_U2					HOLD_U2_OUT |= HOLD_U2_MASK

#define FLASH_MEMORY_SIZE 0x10000	// 512 Kbits = 64K bytes
#define FLASH_SECTOR_SIZE 0x1000	// 4 Kbytes
#define FLASH_BLOCK_SIZE 0x8000		// 32 Kbytes
#define TSCE 100 					// chip-erase time limit (milliseconds)
#define TBP 20						// byte-program time (microseconds)

// Device Operation Instructions (see Table 6 in data sheet)
#define READ                0x03
#define HIGH_SPEED_READ     0x0b
#define SECTOR_ERASE        0x20
#define BLOCK_ERASE         0x52
#define CHIP_ERASE          0x60
#define BYTE_PROGRAM        0x02
#define AAI_PROGRAM         0xAF    // Auto Address Increment Program
#define RDSR                0x05    // Read Status Register
#define EWSR                0x50    // Enable-Write-Status-Register
#define WRSR                0x01    // Write-Status-Register
#define WREN                0x06    // Write-Enable
#define WRDI                0x04    // Write-Disable
#define READ_ID             0x90

//Bits of the SPI status  register
#define SR_BUSY                BIT0        // active high, is system busy writing
#define SR_WRITE_ENABLED       BIT1        // active high, is writing allowed
#define SR_BP0                 BIT2        // Block protection first bit
#define SR_BP1                 BIT3        // Block protection second bit
#define SR_AAI                 BIT6        // active high - use AAI programming mode
#define SR_BPL                 BIT7        // controls whether or not BP0 and BP1 bits are read/write (low) or not (high)

// Block protection levels
#define NONE 					0
#define QUARTER_0xC000_0xFFFF 	1
#define HALF_0x8000_0xFFFF 		2
#define FULL 					3

// Function Protoypes

/*
 * This function initializes the port pins associated with the serial flash components.
 */
void InitializeSerialFlashPortPins();

/*
 * This function will initialize a serial flash component to an associated port register
 * and bit mask (that represents the chip enable for the device).
 */
void InitializeSerialFlashComponent(SerialFlash *Component, uint8_t *PortRegister, uint8_t BitMask);

/*
 * This function reads the ID of the flash memory component, ComponentNumber.
 *
 * Return Value: The ID read from the flash memory component.
 */
uint16_t ReadFlashMemoryID(SerialFlash *Component);

/*
 * This function reads from the status register of the flash memory component,
 * ComponentNumber.
 *
 * Return Value: The content of the status register.
 */
uint8_t ReadFlashMemoryStatusRegister(SerialFlash *Component);

/*
 * This function writes the value of WriteValue to the status register of the flash memory
 * component, ComponentNumber.
 */
void WriteFlashMemoryStatusRegister(uint8_t WriteValue,SerialFlash *Component);

/*
 * This function reads from flash memory component ComponentNumber, beginning at StartAddress,
 * and reads NumberOfDataValues, storing them in DataValuesArray.
 *
 * The input parameter, ReadMode, is used to select between READ mode and HIGH_SPEED_READ mode.
 */
void ReadFlashMemory(uint32_t StartAddress, uint8_t* DataValuesArray,
 uint16_t NumberOfDataValues, SerialFlash *Component, uint8_t ReadMode);

/*
 * This function sets the block protection of flash memory component, ComponnetNumber, to
 * the value of ProtectionLevel, which can have 1 of 4 values: NONE, QUARTER_0xC000_0xFFFF,
 * HALF_0x8000_0xFFFF, and FULL.
 */
void SetBlockProtection(uint8_t ProtectionLevel, SerialFlash *Component);

/*
 * This function writes the byte, WriteValue, to the flash memory component, ComponentNumber,
 * at memory location MemoryAddress.
 */
void ByteProgramFlashMemory(uint32_t MemoryAddress, uint8_t WriteValue, SerialFlash *Component);

/*
 * This function writes to flash memory component ComponentNumber, beginning at StartAddress,
 * and writes NumberOfDataValues, stored in DataValuesArray.
 *
 * The input parameter, ReadMode, is used to select between READ mode and HIGH_SPEED_READ mode.
 */
void AAIProgramFlashMemory(uint32_t StartAddress, uint8_t* DataValuesArray,
 uint16_t NumberOfDataValues, SerialFlash *Component);

/*
 * This function will erase flash memory component ComponentNumber.
 */
void ChipEraseFlashMemory(SerialFlash *Component);


/*
 * This function will erase either a sector or block of flash memory in component ComponentNumber, starting
 * at address StartAddress.
 * 
 * Note EraseMode = SECTOR_ERASE performs a sector erase, and EraseMode = BLOCK_ERASE performs a block erase.
 */
void SectorBlockEraseFlashMemory(uint32_t StartAddress, SerialFlash *Component, uint8_t EraseMode);

/*
 * This function is used to determine if flash memory component ComponentNumber is busy (that is,
 * whether or not the flash memory has completed the current command.
 *
 * Return Value: The return value is the SR_BUSY bit contained in the status register within the flash
 * memory component.
 */
uint8_t FlashMemoryBusy(SerialFlash *Component);

#endif
