#include "serial_flash.h"

void InitializeSerialFlashPortPins(){//Fully Implemented
    DISABLE_HOLD_U1;
    SET_HOLD_U1_AS_AN_OUTPUT;
    DISABLE_HOLD_U2;
    SET_HOLD_U2_AS_AN_OUTPUT;
    DISABLE_WRITE_PROTECT;
    SET_WRITE_PROTECT_AS_AN_OUTPUT;

    SET_ENABLE_U1_AS_AN_OUTPUT;
    SET_ENABLE_U2_AS_AN_OUTPUT;
}

void InitializeSerialFlashComponent(SerialFlash *Component, uint8_t *PortRegister, uint8_t BitMask){//Fully Implemented
    Component->PortRegister = PortRegister;
    Component->BitMask = BitMask;
    DISABLE_FLASH_MEMORY;
}

uint16_t ReadFlashMemoryID(SerialFlash *Component){//Fully Implemented
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

uint8_t ReadFlashMemoryStatusRegister(SerialFlash *Component){//Fully Implemented
    uint8_t RegisterValue = 0;
    ENABLE_FLASH_MEMORY;
    SPISendByte(RDSR);
    RegisterValue = SPIReceiveByte();
    DISABLE_FLASH_MEMORY;
    return RegisterValue;
}

void WriteFlashMemoryStatusRegister(uint8_t WriteValue,SerialFlash *Component){//Fully Implemented
    ENABLE_FLASH_MEMORY;
    SPISendByte(WREN);
    DISABLE_FLASH_MEMORY;

    ENABLE_FLASH_MEMORY;
    SPISendByte(EWSR);
    DISABLE_FLASH_MEMORY;
    ENABLE_FLASH_MEMORY;
    SPISendByte(WRSR);

    SPISendByte(WriteValue);
    DISABLE_FLASH_MEMORY;

    ENABLE_FLASH_MEMORY;
    SPISendByte(WRDI);
    DISABLE_FLASH_MEMORY;

    while(FlashMemoryBusy(Component));
}

void ReadFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, SerialFlash *Component, unsigned char ReadMode){//Fully Implemented

    ENABLE_FLASH_MEMORY;
    unsigned long localAddress;
    unsigned char addressBytes[3];

    localAddress = StartAddress & 0x00ffffff;
    addressBytes[0] = (unsigned char) localAddress;
    localAddress = localAddress >> 8;
    addressBytes[1] = (unsigned char) localAddress;
    localAddress = localAddress >> 8;
    addressBytes[2] = (unsigned char) localAddress;

    SPISendByte(ReadMode);
    SPISendByte(addressBytes[2]);
    SPISendByte(addressBytes[1]);
    SPISendByte(addressBytes[0]);
    if (ReadMode == HIGH_SPEED_READ){
        SPISendByte(0x00);
    }

    int i;
    for (i = 0; i < NumberOfDataValues; i++){
        *(DataValuesArray+i) = SPIReceiveByte();
    }
    DISABLE_FLASH_MEMORY;
}

void ByteProgramFlashMemory(uint32_t MemoryAddress, uint8_t WriteValue, SerialFlash *Component){
    ENABLE_FLASH_MEMORY;
    SPISendByte(WREN);
    DISABLE_FLASH_MEMORY;

    ENABLE_FLASH_MEMORY;
    SPISendByte(BYTE_PROGRAM);

//    uint8_t firstByte = (MemoryAddress >> 16) & 0xFF;
//    uint8_t secondByte = (MemoryAddress >> 8) & 0xFF;
//    uint8_t thirdByte = MemoryAddress & 0xFF;
//
//    SPISendByte(firstByte);
//    SPISendByte(secondByte);
//    SPISendByte(thirdByte);
//    SPISendByte(WriteValue);

    unsigned long localAddress;
    unsigned char addressBytes[3];

    localAddress = MemoryAddress & 0x00ffffff;
    addressBytes[0] = (unsigned char) localAddress;
    localAddress = localAddress >> 8;
    addressBytes[1] = (unsigned char) localAddress;
    localAddress = localAddress >> 8;
    addressBytes[2] = (unsigned char) localAddress;

    SPISendByte(addressBytes[2]);
    SPISendByte(addressBytes[1]);
    SPISendByte(addressBytes[0]);
    SPISendByte(WriteValue);

    DISABLE_FLASH_MEMORY;

    while(FlashMemoryBusy(Component));
}

void AAIProgramFlashMemory(uint32_t StartAddress, uint8_t* DataValuesArray,
uint16_t NumberOfDataValues, SerialFlash *Component){//Fully Implemented
    ENABLE_FLASH_MEMORY;
    SPISendByte(WREN);
    DISABLE_FLASH_MEMORY;

    ENABLE_FLASH_MEMORY;
    SPISendByte(AAI_PROGRAM);

    uint8_t firstByte = (StartAddress >> 16) & 0xFF;
    uint8_t secondByte = (StartAddress >> 8) & 0xFF;
    uint8_t thirdByte = StartAddress & 0xFF;

    SPISendByte(firstByte);
    SPISendByte(secondByte);
    SPISendByte(thirdByte);

    int i;
    for (i = 0; i < NumberOfDataValues; i++){
        if (i != 0) SPISendByte(AAI_PROGRAM);
        SPISendByte(DataValuesArray[i]);
        DISABLE_FLASH_MEMORY;
        while(FlashMemoryBusy(Component));
        ENABLE_FLASH_MEMORY;
    }

    SPISendByte(WRDI);
    DISABLE_FLASH_MEMORY;
    ENABLE_FLASH_MEMORY;
    SPISendByte(RDSR);
    SPIReceiveByte();

    DISABLE_FLASH_MEMORY;
    while(FlashMemoryBusy(Component));
}

void ChipEraseFlashMemory(SerialFlash *Component){//Fully Implemented
    ENABLE_FLASH_MEMORY;
    SPISendByte(WREN);
    DISABLE_FLASH_MEMORY;

    ENABLE_FLASH_MEMORY;
    SPISendByte(CHIP_ERASE);
    DISABLE_FLASH_MEMORY;

    while(FlashMemoryBusy(Component));
}

void SectorBlockEraseFlashMemory(uint32_t StartAddress,SerialFlash *Component, uint8_t EraseMode){
    ENABLE_FLASH_MEMORY;
    SPISendByte(EraseMode);

    uint8_t firstByte = (StartAddress >> 16) & 0xFF;
    uint8_t secondByte = (StartAddress >> 8) & 0xFF;
    uint8_t thirdByte = StartAddress & 0xFF;

    SPISendByte(firstByte);
    SPISendByte(secondByte);
    SPISendByte(thirdByte);

    DISABLE_FLASH_MEMORY;

    while(FlashMemoryBusy(Component));
}

void SetBlockProtection(uint8_t ProtectionLevel,SerialFlash *Component){//Fully Implemented
    ProtectionLevel = ProtectionLevel << 2;
    WriteFlashMemoryStatusRegister(ProtectionLevel, Component);
}

uint8_t FlashMemoryBusy(SerialFlash *Component){//Fully Implemented
    return ReadFlashMemoryStatusRegister(Component) & 0x01;
}
