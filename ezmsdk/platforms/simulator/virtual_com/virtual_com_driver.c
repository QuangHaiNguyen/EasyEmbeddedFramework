
/*******************************************************************************
* Filename:         virtual_com_driver.c
* Author:           Hai Nguyen
* Original Date:    07.08.2022
* Last Update:      07.08.2022
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
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 07.08.2022
*
*******************************************************************************/

/** @file   virtual_com_driver.c
 *  @author Hai Nguyen
 *  @date   07.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "virtual_com_driver.h"

#if (CONFIG_VIRTUAL_COM == 1U)

#include <Windows.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilities/hexdump/hexdump.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/* Logging module conficts with some macro from win32 api so we hack it here */
#define DEBUG       1U                      /**< debug activation flag */
#define MOD_NAME    "VIRTUAL_COM_HW"        /**< module name */


#if (DEBUG == 1U)
#define PRNT_DBG(fmt, ...)      printf("["MOD_NAME"] " fmt "\n", ##__VA_ARGS__)
#define HEXDUMP(x,y)            ezmHexdump(x,y)
#else
#define PRNT_DBG(fmt, ...)
#define HEXDUMP(x, y)
#endif


/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct
{
    wchar_t      port_name[32];
    HANDLE       driver_h;
    DCB          config;
    COMMTIMEOUTS timeouts;
}HwVirtualComDrv;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static HwVirtualComDrv driver = {0};
static UartDrvApi drv_api;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool     VirtualCom_Configure(UartConfiguration *config);
static uint16_t VirtualCom_SendBlocking(uint8_t *buff, uint32_t buff_size);
static uint16_t VirtualCom_RecvBlocking(uint8_t *buff, uint32_t buff_size);


/******************************************************************************
* External functions
*******************************************************************************/
bool VirtualCom_Initialization(void)
{
    bool success = true;

    PRNT_DBG("VirtualCom_Initialization()");

    drv_api.ezmUart_Configure = VirtualCom_Configure;
    drv_api.ezmUart_Receive = NULL;
    drv_api.ezmUart_ReceiveBlocking = VirtualCom_RecvBlocking;
    drv_api.ezmUart_Send = NULL;
    drv_api.ezmUart_SendBlocking = VirtualCom_SendBlocking;
    drv_api.ezmUart_RegisterCallback = NULL;
    drv_api.ezmUart_UnregisterCallback = NULL;

    return success;
}


UartDrvApi *VirtualCom_GetInterface(void)
{
    return &drv_api;
}



/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : VirtualCom_Configure
*//**
* @Description:
*
* @param
* @return
*
* @Example Example:
* @code
*
* @endcode
*
*******************************************************************************/
static bool VirtualCom_Configure(UartConfiguration* config)
{
    bool            success = true;
    size_t          byte_converted = 0;
    char            helper[32] = { 0 };
 
    PRNT_DBG("VirtualCom_Configure(%s)", config->port_name);

    if (config != NULL)
    {
        /* Convert from char to wchar_t thingy */
        snprintf(helper, sizeof(helper), "\\\\.\\%s", config->port_name);
        mbstowcs_s(&byte_converted, driver.port_name, 32, helper, 31);



        driver.driver_h = CreateFile((wchar_t*)driver.port_name,    // port name
                                        GENERIC_READ | GENERIC_WRITE,               // Read/Write
                                        0,                                          // No Sharing
                                        NULL,                                       // No Security
                                        OPEN_EXISTING,                              // Open existing port only
                                        0,                                          // Non Overlapped I/O
                                        NULL);

        if (driver.driver_h == INVALID_HANDLE_VALUE)
        {
            success = false;
        }

        if (success)
        {
            driver.config.DCBlength = sizeof(DCB);
            success &= GetCommState(driver.driver_h, &driver.config);
        }

        if (success)
        {
            driver.config.BaudRate = config->baudrate;
            driver.config.ByteSize = config->byte_size;

            if (config->parity == NONE)
            {
                driver.config.Parity = NOPARITY;
            }
            else if (config->parity == EVEN)
            {
                driver.config.Parity = EVENPARITY;
            }
            else if (config->parity == ODD)
            {
                driver.config.Parity = ODDPARITY;
            }
            else if (config->parity == MARK)
            {
                driver.config.Parity = MARKPARITY;
            }
            else if (config->parity == SPACE)
            {
                driver.config.Parity = SPACEPARITY;
            }
            else
            {
                success = false;
            }


            if (config->stop_bit == ONE_BIT)
            {
                driver.config.StopBits = ONESTOPBIT;
            }
            else if (config->stop_bit == ONE_AND_HALF_BIT)
            {
                driver.config.StopBits = ONE5STOPBITS;
            }
            else if (config->stop_bit == TWO_BITS)
            {
                driver.config.StopBits = TWOSTOPBITS;
            }
            else
            {
                success = false;
            }
        }

        if (success)
        {
            success &= SetCommState(driver.driver_h, &driver.config);
        }

        if (success)
        {
            driver.timeouts.ReadIntervalTimeout = 50;
            driver.timeouts.ReadTotalTimeoutConstant = 50;
            driver.timeouts.ReadTotalTimeoutMultiplier = 10;
            driver.timeouts.WriteTotalTimeoutConstant = 50;
            driver.timeouts.WriteTotalTimeoutMultiplier = 10;

            success &= SetCommTimeouts(driver.driver_h, &driver.timeouts);
        }
    }
    else
    {
        success = false;
    }

    return success;
}




/******************************************************************************
* Function : VirtualCom_SendBlocking
*//**
* @Description: 
*
* @param
* @return
*
* @Example Example:
* @code
* 
* @endcode
*
*******************************************************************************/
static uint16_t VirtualCom_SendBlocking(uint8_t *buff, uint32_t buff_size)
{
    bool            success = true;
    uint32_t        num_byte_writen = 0;
    uint32_t        remain_bytes = buff_size;
    uint8_t         *write_ptr = buff;

    PRNT_DBG("VirtualCom_SendBlocking(%lu bytes)", buff_size);

    if (buff != NULL
        && buff_size > 0)
    {
        do
        {
            success &= WriteFile(driver.driver_h,
                                    write_ptr,
                                    buff_size,
                                    &num_byte_writen,
                                    NULL);

            remain_bytes -= num_byte_writen;
            write_ptr += num_byte_writen;

            PRNT_DBG("wrote %lu(%lu) bytes", 
                        buff_size - remain_bytes,
                        buff_size);
        } while (success == true && remain_bytes > 0);

        if (!success)
        {
            PRNT_DBG("write fail, [remain bytes = %lu]", remain_bytes);
        }
    }
    else
    {
        success = false;
    }

    return buff_size - remain_bytes;
}




/******************************************************************************
* Function : VirtualCom_RecvBlocking
*//**
* @Description:
*
* @param
* @return
*
* @Example Example:
* @code
*
* @endcode
*
*******************************************************************************/
static uint16_t VirtualCom_RecvBlocking(uint8_t* buff, uint32_t buff_size)
{
    bool            success = true;
    uint32_t        num_byte_read = 0;
    uint32_t        remain_bytes = buff_size;
    uint8_t         *read_ptr = buff;

    PRNT_DBG("VirtualCom_RecvBlocking(%lu bytes)", buff_size);

    if (buff != NULL
        && buff_size > 0)
    {
        do
        {
            success &= ReadFile(driver.driver_h,
                                    read_ptr,
                                    buff_size,
                                    &num_byte_read,
                                    NULL);

            remain_bytes -= num_byte_read;
            read_ptr += num_byte_read;

            PRNT_DBG("read %lu(%lu) bytes",
                        buff_size - remain_bytes,
                        buff_size);

        } while (success == true && remain_bytes > 0);

        if (!success)
        {
            PRNT_DBG("read fail, [remain bytes = %lu]", remain_bytes);
        }
    }
    else
    {
        success = false;
    }

    return buff_size - remain_bytes;
}

#endif /* CONFIG_VIRTUAL_COM */
/* End of file*/

