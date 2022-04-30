
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

#if (FLASH_SIM == 1U)
#include "ezmDebug/ezmDebug.h"
#include "helper/ezmAssert/ezmAssert.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MOD_NAME    "FLASH_SIM"

#if (MODULE_DEBUG == 1U) && (SYSTEM_ERROR_DEBUG == 1U)
#define FLASHPRINT(format, ...)    PRINTF_MOD(MOD_NAME, format, __VA_ARGS__)
#else
#define FLASHPRINT(format, ...) 
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief metadata to hold the information of a flash memory
 *
 */
struct FlashMetaData
{
    char name[128];             /**< an integer */
    uint16_t num_block;         /**< an integer */
    uint16_t num_sector;        /**< an integer */
    uint16_t num_page;          /**< an integer */
    uint16_t page_size_byte;    /**<*/
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

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
* @see sum
*
*******************************************************************************/
bool FlashSim_Initialization(void)
{
    FLASHPRINT("FlashSim_Initialization()");
    return true;
}

#endif /* FLASH_SIM == 1U */

/* End of file*/

