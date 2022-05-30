
/*******************************************************************************
* Filename:         system_error.c
* Author:           Hai Nguyen
* Original Date:    06.04.2022 - 22:48:51
* Last Update:      06.04.2022 - 22:48:51
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

/** @file   system_error.c
 *  @author Hai Nguyen
 *  @date   06.04.2022 - 22:48:51
 *  @brief  This is the source of system error module
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "system_error.h"

#if (SYSTEM_ERROR == 1U)
#include "system_error_table.h"
#include "ezmDebug/ezmDebug.h"
#include "helper/ezmAssert/ezmAssert.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MOD_NAME    "SYS_ERROR"

#if (MODULE_DEBUG == 1U) && (SYSTEM_ERROR_DEBUG == 1U)
#define SYSERRPRINT(format, ...)    PRINTF_MOD(MOD_NAME, format, __VA_ARGS__)
#else
#define SYSERRPRINT(format, ...) 
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function : SystemError_Initialize
*//** 
* @Description: This function initializes the error module
*
* it checks if the error id is unique by using the assert funtion
* 
* @param    None
* @return   None
*
* @Example Example:
* @code
* SystemError_Initialize();
* @endcode
*
*******************************************************************************/
void SystemError_Initialize(void)
{
    for (uint16_t i = 0; i < sizeof(error_table) / sizeof(ErrorObject); i++)
    {
        for (uint16_t j = i + 1; j < sizeof(error_table) / sizeof(ErrorObject); j++)
        {
            ASSERT_MSG(error_table[i].error_id != error_table[j].error_id,
                "id must be unique");

        }
    }
}

/******************************************************************************
* Function : SystemError_SetError
*//**
* @Description: this function triggers the error handling
*
* @param    error_id: (IN)Error id
*
* @return   True: Error is handle
*           False: Error does not exist or is not handled
*
* @Example Example:
* @code
* SystemError_SetError(0x01);
* @endcode
*
*
*******************************************************************************/
bool SystemError_SetError(int error_id)
{
    bool ret = false;

    for (uint16_t i = 0; i < sizeof(error_table) / sizeof(ErrorObject); i++)
    {
        if (error_id == error_table[i].error_id && 
            error_table[i].ErrorHandle != NULL)
        {
            error_table[i].ErrorHandle();
            ret = true;
            break;
        }
    }

    return ret;
}

/******************************************************************************
* Function : SystemError_HandleTestWarningError
*//**
* @Description: This is a demo function for handling the error. Users should
* delete it to save some space
*
* @param    None
* @return   None
*
*******************************************************************************/
void SystemError_HandleTestWarningError(void)
{
    SYSERRPRINT("test warning error triggered");
}

/******************************************************************************
* Function : SystemError_HandleTestAttentionError
*//**
* @Description: This is a demo function for handling the error. Users should
* delete it to save some space
*
* @param    None
* @return   None
*
*******************************************************************************/
void SystemError_HandleTestAttentionError(void)
{
    SYSERRPRINT("test attention error triggered");
}

/******************************************************************************
* Function : SystemError_HandleTestCriticalError
*//**
* @Description: This is a demo function for handling the error. Users should
* delete it to save some space
*
* @param    None
* @return   None
*
*******************************************************************************/
void SystemError_HandleTestCriticalError(void)
{
    SYSERRPRINT("test critical error triggered");
}

/******************************************************************************
* Function : SystemError_HandleTestCastatrophicError
*//**
* @Description: This is a demo function for handling the error. Users should
* delete it to save some space
*
* @param    None
* @return   None
*
*******************************************************************************/
void SystemError_HandleTestCastatrophicError(void)
{
    SYSERRPRINT("test castatrophic error triggered");
}
#endif
/* End of file*/

