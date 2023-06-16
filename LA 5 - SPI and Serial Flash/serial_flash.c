#include "serial_flash.h"

void InitializeSerialFlashPortPins()
{
    DISABLE_HOLD_U1;
    SET_HOLD_U1_AS_AN_OUTPUT;
    DISABLE_HOLD_U2;
    SET_HOLD_U2_AS_AN_OUTPUT;
    DISABLE_WRITE_PROTECT;
    SET_WRITE_PROTECT_AS_AN_OUTPUT;

    SET_ENABLE_U1_AS_AN_OUTPUT;
    SET_ENABLE_U2_AS_AN_OUTPUT;
}

void InitializeSerialFlashComponent(SerialFlash *Component, uint8_t *PortRegister, uint8_t BitMask)
{
    Component->PortRegister = PortRegister;
    Component->BitMask = BitMask;
    DISABLE_FLASH_MEMORY;
}

uint16_t ReadFlashMemoryID(SerialFlash *Component)
{
    uint16_t ReturnValue = 0;
    uint8_t ID[2];

    ENABLE_FLASH_MEMORY;
    SPISendByte(READ_ID);
    SPISendByte(0); SPISendByte(0); SPISendByte(0);
    ID[0] = SPIReceiveByte();
    ID[1] = SPIReceiveByte();
    DISABLE_FLASH_MEMORY;

    ReturnValue = ((uint16_t) ID[0] << 8) + (uint16_t) ID[1];

   return ReturnValue;
}

uint8_t ReadFlashMemoryStatusRegister(SerialFlash *Component)
{
    uint8_t RegisterValue = 0;

    return RegisterValue;
}

void WriteFlashMemoryStatusRegister(uint8_t WriteValue,SerialFlash *Component)
{
}

void ReadFlashMemory(uint32_t StartAddress, uint8_t* DataValuesArray,
uint16_t NumberOfDataValues, SerialFlash *Component, uint8_t ReadMode)
{
}

void ByteProgramFlashMemory(uint32_t MemoryAddress, uint8_t WriteValue, SerialFlash *Component)
{
}

void AAIProgramFlashMemory(uint32_t StartAddress, uint8_t* DataValuesArray,
uint16_t NumberOfDataValues, SerialFlash *Component)
{
}

void ChipEraseFlashMemory(SerialFlash *Component)
{
}

void SectorBlockEraseFlashMemory(uint32_t StartAddress,SerialFlash *Component, uint8_t EraseMode)
{
}

void SetBlockProtection(uint8_t ProtectionLevel,SerialFlash *Component)
{
}

uint8_t FlashMemoryBusy(SerialFlash *Component)
{
    uint8_t Busy = 0;

    return Busy;
}
