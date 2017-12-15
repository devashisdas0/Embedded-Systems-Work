#include "serial_flash.h"

void EnableWrite(unsigned char ComponentNumber){
    if(ComponentNumber == FLASH_MEMORY_U3)
            ENABLE_FLASH_MEMORY_U3;
    else
            ENABLE_FLASH_MEMORY_U2;
    SPISendByte(0x06);
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;


}

void DisableWrite(unsigned char ComponentNumber){
    if(ComponentNumber == FLASH_MEMORY_U3)
            ENABLE_FLASH_MEMORY_U3;
    else
            ENABLE_FLASH_MEMORY_U2;
    SPISendByte(0x04);
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;


}
void InitializeSerialFlash()
{
    DISABLE_HOLD_U3;
    SET_HOLD_U3_AS_AN_OUTPUT;
    DISABLE_HOLD_U2;
    SET_HOLD_U2_AS_AN_OUTPUT;
    DISABLE_WRITE_PROTECT;
    SET_WRITE_PROTECT_AS_AN_OUTPUT;

    SET_ENABLE_U3_AS_AN_OUTPUT;
    DISABLE_FLASH_MEMORY_U3;
    SET_ENABLE_U2_AS_AN_OUTPUT;
    DISABLE_FLASH_MEMORY_U2;


}

unsigned int ReadFlashMemoryID(unsigned char ComponentNumber)
{
       unsigned int return_value;
       unsigned char ID[2];

       if (ComponentNumber == FLASH_MEMORY_U3)
            ENABLE_FLASH_MEMORY_U3;
       else
            ENABLE_FLASH_MEMORY_U2;

       SPISendByte(READ_ID);     // Send the instruction
       SPISendByte(0); SPISendByte(0); SPISendByte(0); // Send Address
       ID[0] = SPIReceiveByte(); // Read manufacturer ID
       ID[1] = SPIReceiveByte(); // Read device ID

       if (ComponentNumber == FLASH_MEMORY_U3)
            DISABLE_FLASH_MEMORY_U3;
       else
            DISABLE_FLASH_MEMORY_U2;

       // Place Manufacturer ID in the upper byte of the return value,
       // place the device ID in the lower byte.
       return_value = ((unsigned int) ID[0] << 8) + (unsigned int) ID[1];
       return return_value;



}

/*
 * This function reads from the status register of the flash memory component,
 * ComponentNumber.
 *
 * Return Value: The content of the status register.
 */
unsigned char ReadFlashMemoryStatusRegister(unsigned char ComponentNumber)
{
    //Variable to contain return value
    unsigned char returnval;
    //Enable Flash
    if(ComponentNumber == FLASH_MEMORY_U3)
            ENABLE_FLASH_MEMORY_U3;
    else
            ENABLE_FLASH_MEMORY_U2;
    //Send read byte
    SPISendByte(0x05);
    //Return value is received byte
    returnval = SPIReceiveByte();

    //Disable flash
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

    return returnval;

}

/*
 * This function writes the value of WriteValue to the status register of the flash memory
 * component, ComponentNumber.
 */
void WriteFlashMemoryStatusRegister(unsigned char WriteValue,unsigned char ComponentNumber)
{
    //Enable flash active low
    if(ComponentNumber == FLASH_MEMORY_U3)
          ENABLE_FLASH_MEMORY_U3;
    else
          ENABLE_FLASH_MEMORY_U2;
    //Enable the write status register
    SPISendByte(0x50);
    //Disable the flash active high
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;
    //Enable flash for next send
    if(ComponentNumber == FLASH_MEMORY_U3)
          ENABLE_FLASH_MEMORY_U3;
    else
          ENABLE_FLASH_MEMORY_U2;
    //Use the write status register
    SPISendByte(0x01);
    //Send the value you want to write
    SPISendByte(WriteValue);
    //Disable flash active high
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

}
/*
 * This function reads from flash memory component ComponentNumber, beginning at StartAddress,
 * and reads NumberOfDataValues, storing them in DataValuesArray.
 *
 * The input parameter, ReadMode, is used to select between READ mode and HIGH_SPEED_READ mode.
 */
void ReadFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber, unsigned char ReadMode)
{
    volatile unsigned long i = 0;
    //Enable flash active low
    if(ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;
    //Used to select which type of read mode we are in
    SPISendByte(ReadMode);
    //Shift in the address of what we are sending into a data array
    SPISendByte((StartAddress & 0xFFFFFF) >> 16);
    SPISendByte((StartAddress & 0xFFFF) >> 8);
    SPISendByte(StartAddress & 0xFF);

    for(i=0; i<NumberOfDataValues; i++){
        DataValuesArray[i] = SPIReceiveByte();
    }
    //Disable flash memory active high
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;

}

/*
 * This function writes the byte, WriteValue, to the flash memory component, ComponentNumber,
 * at memory location MemoryAddress.
 */
void ByteProgramFlashMemory(unsigned long MemoryAddress, unsigned char WriteValue, unsigned char ComponentNumber)
{
    //Enable write
    EnableWrite(ComponentNumber);
    //Enable flash
    if(ComponentNumber == FLASH_MEMORY_U3)
         ENABLE_FLASH_MEMORY_U3;
    else
         ENABLE_FLASH_MEMORY_U2;
    //Use the byte-program bit (0x02)
    SPISendByte(0x02);
    //Shift int the memory address to write
    SPISendByte((MemoryAddress & 0xFFFFFF) >> 16);
    SPISendByte((MemoryAddress & 0xFFFF) >> 8);
    SPISendByte(MemoryAddress & 0xFF);
    //Write the byte write value
    SPISendByte(WriteValue);
    //disable flash
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;
    //Wait until the flash is not busy
    while(FlashMemoryBusy(ComponentNumber)==1);
    //Disable the write bit so we are back to normal
    DisableWrite(ComponentNumber);



}

/*
 * This function writes to flash memory component ComponentNumber, beginning at StartAddress,
 * and writes NumberOfDataValues, stored in DataValuesArray.
 *
 */
void AAIProgramFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber)
{
    volatile unsigned int i = 0;
    //Enable writing
    EnableWrite(ComponentNumber);
    //Loop used for automation of writing of into flash memory
    for(i=0; i<NumberOfDataValues;i++){
        if (ComponentNumber == FLASH_MEMORY_U3)
            ENABLE_FLASH_MEMORY_U3;
        else
            ENABLE_FLASH_MEMORY_U2;
        //Start the AAI programming
        SPISendByte(0xAF);
        //if we are at the first bit start shifting the address in
        if(i==0){
            SPISendByte((StartAddress & 0xFFFFFF) >> 16);
            SPISendByte((StartAddress & 0xFFFF) >> 8);
            SPISendByte(StartAddress & 0xFF);
        }
        //Send the array which has all the data
        SPISendByte(DataValuesArray[i]);
        //Disable flash
        if (ComponentNumber == FLASH_MEMORY_U3)
            DISABLE_FLASH_MEMORY_U3;
        else
            DISABLE_FLASH_MEMORY_U2;
        //Wait before next commands so the flash is not busy
        while(FlashMemoryBusy(ComponentNumber)==1);
    }
        //Get back to normal state by disabling write
        DisableWrite(ComponentNumber);
        //Enable flash for next command
        if (ComponentNumber == FLASH_MEMORY_U3)
             ENABLE_FLASH_MEMORY_U3;
        else
             ENABLE_FLASH_MEMORY_U2;
        //Poll the status register
        SPISendByte(RDSR);
        //Receive byte for polling
        volatile unsigned char out = SPIReceiveByte();
       //Disable flash and write
        if (ComponentNumber == FLASH_MEMORY_U3)
             DISABLE_FLASH_MEMORY_U3;
        else
             DISABLE_FLASH_MEMORY_U2;
        DisableWrite(ComponentNumber);
}

/*
 * This function will erase flash memory component ComponentNumber.
 */
void ChipEraseFlashMemory(unsigned char ComponentNumber)
{
    //Enable write and flash for commands
    EnableWrite(ComponentNumber);
    if (ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;
    //Semd command to erase chip
    SPISendByte(CHIP_ERASE);
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;
    //Wait till flash is not busy
    while(FlashMemoryBusy(ComponentNumber)==1);
    //Disable write to get back to normal
    DisableWrite(ComponentNumber);


}
/*
 * This function will erase flash memory component ComponentNumber, beginning at address,
 * StartAddress. The function supports both sector and block erase, which are selected
 * using EraseMode, which can have one of two values: SECTOR_ERASE or BLOCK_ERASE.
 *
 * Note that these two values correspond to the Flash commands, so that they can be used
 * as the command sent to the Flash.
 */
void SectorBlockEraseFlashMemory(unsigned long StartAddress, unsigned char ComponentNumber, unsigned char EraseMode)
{
    //Enable write and flash
    EnableWrite(ComponentNumber);
    if (ComponentNumber == FLASH_MEMORY_U3)
        ENABLE_FLASH_MEMORY_U3;
    else
        ENABLE_FLASH_MEMORY_U2;
    //Choose what erase is needed and start erasing from the start address
    SPISendByte(EraseMode);
    SPISendByte((StartAddress & 0xFFFFFF) >> 16);
    SPISendByte((StartAddress & 0xFFFF) >> 8);
    SPISendByte(StartAddress & 0xFF);
    //Disable flash
    if (ComponentNumber == FLASH_MEMORY_U3)
        DISABLE_FLASH_MEMORY_U3;
    else
        DISABLE_FLASH_MEMORY_U2;
    //Wait till not busy to execute command
    while(FlashMemoryBusy(ComponentNumber)==1);
    //Disable write to get back to normal
    DisableWrite(ComponentNumber);

}

/*
 * This function sets the block protection of flash memory component, ComponnetNumber, to
 * the value of ProtectionLevel, which can have 1 of 4 values: NONE, QUARTER_0xC000_0xFFFF,
 * HALF_0x8000_0xFFFF, and FULL.
 */
void SetBlockProtection(unsigned char ProtectionLevel, unsigned char ComponentNumber)
{
    //depending on the level of memory that needs protection make
    //sure we are writing the correct binary
    if(ProtectionLevel == FULL){
        WriteFlashMemoryStatusRegister(0b00001100, ComponentNumber);
    }
    if(ProtectionLevel == HALF_0x8000_0xFFFF){
        WriteFlashMemoryStatusRegister(0b00001000, ComponentNumber);
    }
    if(ProtectionLevel == QUARTER_0xC000_0xFFFF){
        WriteFlashMemoryStatusRegister(0b00000100, ComponentNumber);
    }
    if(ProtectionLevel == NONE){
        WriteFlashMemoryStatusRegister(0b00000000, ComponentNumber);
    }


}

/*
 * This function is used to determine if flash memory component ComponentNumber is busy (that is,
 * whether or not the flash memory has completed the current command.
 *
 * Return Value: The return value is the SR_BUSY bit contained in the status register within the flash
 * memory component.
 */
unsigned char FlashMemoryBusy(unsigned char ComponentNumber)
{
    //Check the status register, if 0 not busy, if 1 busy
    unsigned char busybit = (ReadFlashMemoryStatusRegister(ComponentNumber) & SR_BUSY);
    return busybit;
}
