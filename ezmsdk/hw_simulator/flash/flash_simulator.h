
/*******************************************************************************
* Filename:         flash_simulator.h
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

/** @file   flash_simulator.h
 *  @author Hai Nguyen
 *  @date   29.04.2022 - 23:17:23
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _FLASH_SIMULATOR_H
#define _FLASH_SIMULATOR_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (FLASH_SIM == 1U) && (SUPPORTED_CHIP == WIN)
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define INVALID_HANDLE      0xFF    /**< invalide flash handle */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef uint8_t FlashHandle;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool        FlashSim_Initialization(void);
FlashHandle FlashSim_GetFlash(char const* name);
uint32_t    FlashSim_GetFlashSizeInBytes(FlashHandle handle);
bool        FlashSim_EraseChip(FlashHandle handle);
bool        FlashSim_EraseBlock(FlashHandle handle, uint32_t address);
bool        FlashSim_EraseSector(FlashHandle handle, uint32_t address);
bool        FlashSim_ErasePage(FlashHandle handle, uint32_t address);
uint32_t    FlashSim_WritePage(FlashHandle handle,
                                uint32_t address, 
                                uint8_t * buffer,
                                uint16_t num_byte);
uint32_t    FlashSim_Read(FlashHandle handle,
                                uint32_t address,
                                uint8_t* buffer,
                                uint16_t num_byte);


#endif /* (FLASH_SIM == 1U) && SUPPORTED_CHIP == WIN */
#endif /* _FLASH_SIMULATOR_H */

/* End of file */

