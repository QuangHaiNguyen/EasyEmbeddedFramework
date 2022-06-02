
/*******************************************************************************
* Filename:         flash_simulator.c
* Author:           Hai Nguyen
* Original Date:    29.04.2022 - 23:17:23
* Last Update:      29.04.2022 - 23:17:23
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright (C) 2022 by Easy Embedded
*  
*******************************************************************************/

/** @file   flash_simulator.c
 *  @author Hai Nguyen
 *  @date   29.04.2022 - 23:17:23
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "flash_simulator.h"

#if (FLASH_SIM == 1U) && (SUPPORTED_CHIP == WIN)
#include "ezmDebug/ezmDebug.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/ezmAssert/ezmAssert.h"
#include <string.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#pragma warning(disable : 4996)

#define MOD_NAME            "FLASH_SIM"
#define MOD_NAME_VERBOSE    "FLASH_SIM_VERBOSE"
#define VERBOSE     0U

#if (MODULE_DEBUG == 1U) && (FLASH_SIM_DEBUG == 1U)
#define FLASHPRINT(format, ...)    PRINTF_MOD(MOD_NAME, format, __VA_ARGS__)
#else
#define FLASHPRINT(format, ...) 
#endif

#if (MODULE_DEBUG == 1U) && (FLASH_SIM_DEBUG == 1U) && (VERBOSE == 1U)
#define VERBOSEPRINT(format, ...)   PRINTF_MOD(MOD_NAME_VERBOSE, format, __VA_ARGS__)
#define HEXDUMP(x,y)                ezmHexdump(x,y)
#else
#define VERBOSEPRINT(format, ...)
#define HEXDUMP(x,y)
#endif

#define GET_PAGE_SIZE(handle)   (flash_list[handle].page_size_byte)
#define GET_NUM_PAGE(handle)    (flash_list[handle].num_page)
#define GET_NUM_SECTOR(handle)  (flash_list[handle].num_sector)
#define GET_NUM_BLOCK(handle)   (flash_list[handle].num_block)
#define GET_START_ADR(handle)   (flash_list[handle].start_address)

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief metadata to hold the information of a flash memory
 *
 */
struct FlashMetaData
{
    char name[128];             /**< Name of the flash */
    uint16_t num_block;         /**< Number of blocks in the flash memory */
    uint16_t num_sector;        /**< Number of sectors in a block */
    uint16_t num_page;          /**< Number of pages in a sector */
    uint16_t page_size_byte;    /**< Numer of byte in a page */
    unsigned int start_address; /**< Start address of the flash */
};

/** @brief store the list of simulated flash, users must manually edit or add
 * new flash to the list
 * 
 * @see 0002-flash-sim-initialization.md
 */
static struct FlashMetaData flash_list[NUM_OF_FLASH] = {
    {"default_flash", 4U, 4U, 4U, 256U, 0x00000000},
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static char     full_path[128] = { 0 };
static uint8_t  cache[1024] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
static char*    FlashSim_GetFullPath    (FlashHandle handle);
static bool     FlashSim_IsFlashExisting(FlashHandle handle);
static bool     FlashSim_CreateNewBinFile(FlashHandle handle);
static bool     FlashSim_IsAddressValid (FlashHandle handle, uint32_t address);
static uint32_t FlashSim_GetPageSize    (FlashHandle handle);
static uint32_t FlashSim_GetSectorSize  (FlashHandle handle);
static uint32_t FlashSim_GetBlockSize   (FlashHandle handle);
static uint32_t FlashSim_GetFlashSize   (FlashHandle handle);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : FlashSim_Initialization
*//** 
* @Description: This funcion initializes a flash memory simulator
*
* The function checks if the file name flash_name.bin has been existing. If not,
* the function create a bin file with:
* size = num_block * num_sector * num_page * num_byte
* and fills that page with 0xFF to simulate the memory of the flash. If the file
* is existing, the function does sanity check of the file
* 
* @param    None
* @return   true:   success
*           false:  fail
*
* @Example Example:
* @code
* if( FlashSim_Initialization() == true)
* {
*      printf("init success\n");
* }
* @endcode
*
*******************************************************************************/
bool FlashSim_Initialization(void)
{
    FLASHPRINT("FlashSim_Initialization()");

    for (uint8_t i = 0; i < NUM_OF_FLASH; i++)
    {
        FLASHPRINT("Initialize [flash name = %s]", flash_list[i].name);
        if (FlashSim_IsFlashExisting(i) == false)
        {
            FLASHPRINT("flash does not exist");
            if (FlashSim_CreateNewBinFile(i) == false)
            {
                PRINT_ERROR("cannot create new flash!!!");
            }
        }
        else
        {
            FLASHPRINT("flash exists");
        }
    }

    return true;
}

/******************************************************************************
* Function : FlashSim_GetFlash
*//**
* @Description: This funcion returns the flash handle from the flash name
*
* The function checks if the flash name is presented in the list and returns
* the corresponding handle
*
* @param    name:   (IN)pointer to the flash name
* @return   flash handle
*
* @Example Example:
* @code
* if( FlashSim_GetFlash("test_flash") != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
* }
* @endcode
*
*******************************************************************************/
FlashHandle FlashSim_GetFlash(char const* name)
{
    FlashHandle handle = INVALID_HANDLE;

    for (uint8_t i = 0; i < NUM_OF_FLASH; i++)
    {
        if (strncmp(name, flash_list[i].name, sizeof(flash_list[i].name)) == 0U)
        {
            handle = i;
            break;
        }
    }

    return handle;
}

/******************************************************************************
* Function : FlashSim_GetFlashSizeInBytes
*//**
* @Description: This funcion returns the size of the flash in bytes
*
* @param    handle      (IN)handle of the flash
* @return   size of the flash in bytes or 0 if failing
*
*******************************************************************************/
uint32_t FlashSim_GetFlashSizeInBytes(FlashHandle handle)
{
    uint32_t ret = 0;
    if (handle < NUM_OF_FLASH)
    {
        ret = flash_list[handle].page_size_byte * flash_list[handle].num_page *\
                flash_list[handle].num_sector * flash_list[handle].num_block;
    }

    return ret;
}

/******************************************************************************
* Function : FlashSim_Read
*//**
* @Description: This funcion return the number of bytes read from address
*
* @param    handle      (IN)handle of the flash
* @param    address     (IN)address where data is read
* @param    buffer      (OUT)pointer to output buffer
* @param    num_byte    (IN)number of byte to be read
* @return   number of byte to be read
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* uint8_t buffer[64];
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_Read(handle, 0x00000000, buffer, 64);
* }
* @endcode
*******************************************************************************/
uint32_t FlashSim_Read(FlashHandle handle, uint32_t address,
                            uint8_t* buffer,
                            uint16_t num_byte)
{
    FILE* file = NULL;
    char* path = NULL;
    bool success = true;
    uint32_t ret_byte = num_byte;

    /* Do not need to check the handle, FlashSim_IsAddressValid will do it */
    success = success && FlashSim_IsAddressValid(handle, address);

    if (success)
    {
        if (ret_byte > FlashSim_GetFlashSize(handle))
        {
            ret_byte = FlashSim_GetFlashSize(handle);
        }

        path = FlashSim_GetFullPath(handle);
        file = fopen(path, "rb");
    }

    if (file != NULL && success == true && buffer != NULL && ret_byte > 0)
    {
        if (fseek(file, address - GET_START_ADR(handle), SEEK_SET) == 0)
        {
            ret_byte = fread(buffer, sizeof(uint8_t), ret_byte, file);
        }

        fclose(file);

        VERBOSEPRINT("Read [size = %d] at [address = 0x%08x]", ret_byte, address);
        HEXDUMP(buffer, (uint16_t)ret_byte);
    }

    return ret_byte;
}

/******************************************************************************
* Function : FlashSim_WritePage
*//**
* @Description: This funcion writes a page from the buffer to the flash
*
* @param    handle      (IN)handle of the flash
* @param    address     (IN)address where data is read
* @param    buffer      (OUT)pointer to input buffer
* @param    num_byte    (IN)number of byte to be written
* @return   number of byte to be read
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* uint8_t buffer[64] = {0xAA};
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_WritePage(handle, 0x00000000, buffer, 64);
* }
* @endcode
*******************************************************************************/
uint32_t FlashSim_WritePage(FlashHandle handle,
                            uint32_t address,
                            uint8_t* buffer,
                            uint16_t num_byte)
{
    FILE* file = NULL;
    char* path = NULL;
    bool success = true;
    uint32_t ret_byte = num_byte;

    /* Do not need to check the handle, FlashSim_IsAddressValid will do it */
    if (FlashSim_IsAddressValid(handle, address) == false ||
        address % GET_PAGE_SIZE(handle) != 0)
    {
        success = false;
        ret_byte = 0U;
    }

    if (success)
    {
        if (ret_byte > FlashSim_GetPageSize(handle))
        {
            ret_byte = FlashSim_GetPageSize(handle);
        }

        path = FlashSim_GetFullPath(handle);
        file = fopen(path, "rb+");
    }

    if (file != NULL && success == true)
    {
        if (fseek(file, address - GET_START_ADR(handle), SEEK_SET) == 0)
        {
            ret_byte = fread(cache, sizeof(uint8_t), ret_byte, file);
            for (uint16_t i = 0; i < ret_byte; i++)
            {
                cache[i] &= buffer[i];
            }
        }
        else
        {
            success = false;
        }

        if (success == true && 
            fseek(file, address - GET_START_ADR(handle), SEEK_SET) == 0)
        {
            ret_byte = fwrite(cache, sizeof(uint8_t), ret_byte, file);
        }
        else
        {
            success = false;
            ret_byte = 0;
        }

        fclose(file);

        VERBOSEPRINT("Write [size = %d] at [address = 0x%04x]",
            ret_byte, address);

        HEXDUMP(cache, (uint16_t)ret_byte);
    }
    return ret_byte;
}

/******************************************************************************
* Function : FlashSim_ErasePage
*//**
* @Description: This funcion erases a page
*
* This functions check if the address of the page is correct before erasing
* 
* @param    handle      (IN)handle of the flash
* @param    address     (IN)address of the erased page
* @return   true:   erase success
*           false:  erase fail
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_ErasePage(handle, 0x00000000);
* }
* @endcode
*******************************************************************************/
bool FlashSim_ErasePage(FlashHandle handle, uint32_t address)
{
    FILE* file = NULL;
    bool success = true;
    char* path = NULL;

    if (false == FlashSim_IsAddressValid(handle, address) ||
        0 != address % FlashSim_GetPageSize(handle))
    {
        success = false;
    }

    if (success)
    {
        path = FlashSim_GetFullPath(handle);
        file = fopen(path, "rb+");
    }

    if (success == true && file != NULL)
    {
        if (0 == fseek(file, address - GET_START_ADR(handle), SEEK_SET))
        {
            memset(cache, 0xff, GET_PAGE_SIZE(handle));
            (void)fwrite(cache, sizeof(uint8_t), GET_PAGE_SIZE(handle), file);
        }
        else
        {
            success = false;
        }
        (void) fclose(file);
    }

    return success;
}

/******************************************************************************
* Function : FlashSim_EraseSector
*//**
* @Description: This funcion erases a sector
*
* This functions check if the address of the sector is correct before erasing
*
* @param    handle      (IN)handle of the flash
* @param    address     (IN)address of the erased sector
* @return   true:   erase success
*           false:  erase fail
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_ErasePage(handle, 0x00000000);
* }
* @endcode
*******************************************************************************/
bool FlashSim_EraseSector(FlashHandle handle, uint32_t address)
{
    bool success = true;

    if (address % FlashSim_GetSectorSize(handle) != 0U)
    {
        success = false;
    }

    if(success)
    {
        for (uint32_t i = address;
                i < address + FlashSim_GetSectorSize(handle);
                i = i + GET_PAGE_SIZE(handle))
        {
            if (FlashSim_ErasePage(handle, i) == false)
            {
                success = false;
                break;
            }
        }
    }
    return success;
}

/******************************************************************************
* Function : FlashSim_EraseBlock
*//**
* @Description: This funcion erases a block
*
* This functions check if the address of the block is correct before erasing
*
* @param    handle      (IN)handle of the flash
* @param    address     (IN)address of the erased block
* @return   true:   erase success
*           false:  erase fail
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_EraseBlock(handle, 0x00000000);
* }
* @endcode
*******************************************************************************/
bool FlashSim_EraseBlock(FlashHandle handle, uint32_t address)
{
    bool success = true;

    if (address % FlashSim_GetBlockSize(handle) == 0U)
    {
        for (uint16_t i = 0; i < GET_NUM_SECTOR(handle); i++)
        {
            if (FlashSim_EraseSector(handle, address))
            {
                address = address + FlashSim_GetSectorSize(handle);
            }
            else
            {
                success = false;
                break;
            }
        }
    }
    else
    {
        success = false;
    }

    return success;
}

/******************************************************************************
* Function : FlashSim_EraseChip
*//**
* @Description: This funcion erases complete flash
*
* @param    handle      (IN)handle of the flash
* @return   true:   erase success
*           false:  erase fail
*
* @Example Example:
* @code
* FlashHandle handle = FlashSim_GetFlash("test_flash");
* if( handle != INVALID_HANDLE)
* {
*      printf("get valid handle\n");
*      (void*)FlashSim_EraseChip(handle);
* }
* @endcode
*******************************************************************************/
bool FlashSim_EraseChip(FlashHandle handle)
{
    bool success = true;
    uint32_t address = GET_START_ADR(handle);

    for (uint16_t i = 0; i < GET_NUM_BLOCK(handle); i++)
    {
        if (FlashSim_EraseBlock(handle, address))
        {
            address = address + FlashSim_GetBlockSize(handle);
        }
        else
        {
            success = false;
            break;
        }
    }

    return success;
}
/*******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : FlashSim_GetFullPath
*//**
* @Description: This funcion return the path of the binary file
*
* The function combines the name of the flash with fe FLASH_LOCATION to create
* the path to the binary file
*
* @param    handle  (IN)handle of the flash
* @return   path to binary file
*
*******************************************************************************/
static char* FlashSim_GetFullPath(FlashHandle handle)
{
    char* ret = NULL;
    struct FlashMetaData* flash = NULL;

    if (handle < NUM_OF_FLASH)
    {
        memset(full_path, '\0', sizeof(full_path));
        flash = &flash_list[handle];
        strcat(full_path, FLASH_LOCATION);
        strcat(full_path, flash->name);
        strcat(full_path, ".bin");
        ret = full_path;
    }

    return ret;
}

/******************************************************************************
* Function : FlashSim_IsFlashExisting
*//**
* @Description: This funcion checks if a flash memory exists
*
* The function tries to open the binary file simulating flash memory with read
* operation. If opening is success, it means file exists, else, files does not
* exist
*
* @param    handle  (IN)handle of the flash
* @return   true:   flash exists
*           false:  flash does not exist
*
*******************************************************************************/
static bool FlashSim_IsFlashExisting(FlashHandle handle)
{
    FILE *file;
    char* path = NULL;
    bool existing = false;

    if (handle < NUM_OF_FLASH)
    {
        path = FlashSim_GetFullPath(handle);
        file = fopen(path, "rb");
        if (file != NULL)
        {
            fclose(file);
            existing = true;
            VERBOSEPRINT("bin file exists [path = %s]", path);
        }
    }
    return existing;
}

/******************************************************************************
* Function : FlashSim_CreateNewBinFile
*//**
* @Description: This funcion creats a file in binary format to simulates the
*               memory
*
* The function gets the name of the file from the flash list and fills 0xFF to
* simulate the reset stage of a new flash memory
* 
* @param    handle  (IN)handle of the flash
* @return   true:   success
*           false:  fail (cannot open file and write data)
*
*******************************************************************************/
static bool FlashSim_CreateNewBinFile(FlashHandle handle)
{
    FILE* file;
    bool success = false;
    char* path = NULL;

    if (handle < NUM_OF_FLASH)
    {
        path = FlashSim_GetFullPath(handle);
        file = fopen(path, "wb");
        if (file != NULL)
        {
            for (uint32_t i = 0; i < FlashSim_GetFlashSize(handle); i++)
            {
                (void)fputc(0xFF, file);
            }
            (void)fclose(file);
            success = true;
            VERBOSEPRINT("new bin file is created [path = %s]", path);
        }
    }

    return success;
}

/******************************************************************************
* Function : FlashSim_IsAddressValid
*//**
* @Description: This funcion check if an address is in the address space of the
*               flash
*
* @param    handle  (IN)handle of the flash
* @param    address (IN)address to be check
* @return   true:   valid
*           false:  not valid
*
*******************************************************************************/
static bool FlashSim_IsAddressValid(FlashHandle handle, uint32_t address)
{
    bool valid = false;
    struct FlashMetaData* flash = NULL;
    if (handle < NUM_OF_FLASH)
    {
        flash = &flash_list[handle];
        
        if (address >= flash->start_address &&
            address < flash->start_address + FlashSim_GetFlashSize(handle))
        {
            valid = true;
        }
    }
    return valid;
}

/******************************************************************************
* Function : FlashSim_GetPageSize
*//**
* @Description: This funcion return number of bytes in a page
*
* @param    handle  (IN)handle of the flash
* @return   0:                      file handle is invalid, cannot get size
*           others positive value:  number of byte
*
*******************************************************************************/
static uint32_t FlashSim_GetPageSize(FlashHandle handle)
{
    uint32_t size = 0;

    if (handle < NUM_OF_FLASH)
    {
        size = flash_list[handle].page_size_byte;
    }

    return size;
}

/******************************************************************************
* Function : FlashSim_GetSectorSize
*//**
* @Description: This funcion return number of bytes in a sector
*
* @param    handle  (IN)handle of the flash
* @return   0:                      file handle is invalid, cannot get size
*           others positive value:  number of byte
*
*******************************************************************************/
static uint32_t FlashSim_GetSectorSize(FlashHandle handle)
{
    uint32_t size = 0;

    if (handle < NUM_OF_FLASH)
    {
        size = GET_NUM_PAGE(handle) * GET_PAGE_SIZE(handle);
    }

    return size;
}

/******************************************************************************
* Function : FlashSim_GetBlockSize
*//**
* @Description: This funcion return number of bytes in a sector
*
* @param    handle  (IN)handle of the flash
* @return   0:                      file handle is invalid, cannot get size
*           others positive value:  number of byte
*
*******************************************************************************/
static uint32_t FlashSim_GetBlockSize(FlashHandle handle)
{
    uint32_t size = 0;

    if (handle < NUM_OF_FLASH)
    {
        size = GET_NUM_SECTOR(handle) * FlashSim_GetSectorSize(handle);
    }

    return size;
}

/******************************************************************************
* Function : FlashSim_GetFlashSize
*//**
* @Description: This funcion return number of bytes in a sector
*
* @param    handle  (IN)handle of the flash
* @return   0:                      file handle is invalid, cannot get size
*           others positive value:  number of byte
*
*******************************************************************************/
static uint32_t FlashSim_GetFlashSize(FlashHandle handle)
{
    uint32_t size = 0;

    if (handle < NUM_OF_FLASH)
    {
        size = GET_NUM_BLOCK(handle) * FlashSim_GetBlockSize(handle);
    }

    return size;
}
#endif /* (FLASH_SIM == 1U) && SUPPORTED_CHIP == WIN */

/* End of file*/

