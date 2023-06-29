
/*******************************************************************************
* Filename:         ez_windows_uart.c
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      25.06.2023
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 25.06.2023
*
*******************************************************************************/

/** @file   ez_windows_uart.c
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_windows_uart.h"

#define DEBUG_LVL   LVL_TRACE               /**< logging level */
#define MOD_NAME    "ez_windows_uart"       /**< module name */

#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"

#include <Windows.h>
#include <wchar.h>

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct ezUartInternalDriver
{
    HANDLE       driver_h;
    DCB          config;
    COMMTIMEOUTS timeouts;
    struct ezTargetUartDriver uart_driver;
    ezDriverCallback callback;
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool ezWinUart_Initialize(uint8_t driver_index);
static bool ezWinUart_Deinitialize(uint8_t driver_index);
static uint32_t ezWinUart_WriteBlocking(uint8_t driver_index,
                                        uint8_t *data,
                                        uint32_t data_size);

static uint32_t ezWinUart_ReadBlocking(uint8_t driver_index,
                                       uint8_t *data,
                                       uint32_t data_size);

static struct ezUartInternalDriver internal_drivers[NUM_OF_UART_INTERFACE] =
{
    {
        .uart_driver =
        {
            .index = 0,
            .initialized = false,
            .api =
            {
                .Initialize = ezWinUart_Initialize,
                .Deinitialize = ezWinUart_Deinitialize,
                .Read = NULL,
                .ReadBlocking = ezWinUart_ReadBlocking,
                .Write = NULL,
                .WriteBlocking = ezWinUart_WriteBlocking,
            },
        },
        .callback = NULL,
    },
    {
        .uart_driver =
        {
            .index = 1,
            .initialized = false,
            .api =
            {
                .Initialize = ezWinUart_Initialize,
                .Deinitialize = ezWinUart_Deinitialize,
                .Read = NULL,
                .ReadBlocking = ezWinUart_ReadBlocking,
                .Write = NULL,
                .WriteBlocking = ezWinUart_WriteBlocking,
            },
        },
        .callback = NULL,
    },
};


/******************************************************************************
* External functions
*******************************************************************************/

const struct ezTargetUartDriver *ezWinUart_GetDriver(uint8_t driver_index)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);

    const struct ezTargetUartDriver *driver = NULL;
    EZDEBUG("ezTargeUart_GetDriver(index = %d)", driver_index);

    if (driver_index < NUM_OF_UART_INTERFACE)
    {
        driver = &internal_drivers[driver_index].uart_driver;
    }/* else invalid index */

    return driver;
}


struct ezTargetUartConfiguration *ezWinUart_GetConfiguration(uint8_t driver_index)
{
    struct ezTargetUartConfiguration *config = NULL;

    ASSERT(driver_index < NUM_OF_UART_INTERFACE);

    if (driver_index < NUM_OF_UART_INTERFACE)
    {
        config = &internal_drivers[driver_index].uart_driver.config;

        switch (internal_drivers[driver_index].config.Parity)
        {
        case NOPARITY:      config->parity = PARITY_NONE;   break;
        case EVENPARITY:    config->parity = PARITY_EVEN;   break;
        case ODDPARITY:     config->parity = PARITY_ODD;    break;
        case MARKPARITY:    config->parity = PARITY_MARK;   break;
        case SPACEPARITY:   config->parity = PARITY_SPACE;  break;
        /* Set the config to NULL to indicated error */
        default:            config = NULL;                  break;
        }

        if (config)
        {
            switch (internal_drivers[driver_index].config.StopBits)
            {
            case ONESTOPBIT:    config->stop_bit = ONE_BIT;             break;
            case ONE5STOPBITS:  config->stop_bit = ONE_AND_HALF_BIT;    break;
            case TWO_BITS:      config->stop_bit = TWO_BITS;            break;
            /* Set the config to NULL to indicated error */
            default:            config = NULL;                          break;
            }
        }

        if (config)
        {
            config->port_name = UART1_NAME;
            config->baudrate = internal_drivers[driver_index].config.BaudRate;
            config->byte_size = internal_drivers[driver_index].config.ByteSize;
        }
        else
        {
            config = NULL;
        }/* else invalid argument */
    }
    return config;
}


void ezWinUart_SetCallback(uint8_t driver_index,
                           ezDriverCallback callback)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);
    ASSERT(callback != NULL);

    if (driver_index < NUM_OF_UART_INTERFACE && callback)
    {
        internal_drivers[driver_index].callback = callback;
    }
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static bool ezWinUart_Initialize(uint8_t driver_index)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);
    EZDEBUG("ezWinUart_Initialize(index = %d)", driver_index);

    bool success = true;

    if (driver_index < NUM_OF_UART_INTERFACE)
    {
        internal_drivers[driver_index].driver_h = CreateFile("\\\\.\\" UART_PORT,              // port name
                                                                GENERIC_READ | GENERIC_WRITE,   // Read/Write
                                                                0,                              // No Sharing
                                                                0,                              // No Security
                                                                OPEN_EXISTING,                  // Open existing port only
                                                                0,                              // Non Overlapped I/O
                                                                NULL);

        if (internal_drivers[driver_index].driver_h == INVALID_HANDLE_VALUE)
        {
            EZERROR("  Cannot open port");
            success &= false;
        }

        if (success)
        {
            internal_drivers[driver_index].config.DCBlength = sizeof(DCB);
            success &= GetCommState(internal_drivers[driver_index].driver_h,
                                    &internal_drivers[driver_index].config);
        }

        if (success)
        {
            internal_drivers[driver_index].config.BaudRate = 115200;
            internal_drivers[driver_index].config.ByteSize = 8;
            internal_drivers[driver_index].config.Parity = NOPARITY;
            internal_drivers[driver_index].config.StopBits = 1;

            success &= SetCommState(internal_drivers[driver_index].driver_h,
                                    &internal_drivers[driver_index].config);
        }

        if (success)
        {
            internal_drivers[driver_index].timeouts.ReadIntervalTimeout = 50;
            internal_drivers[driver_index].timeouts.ReadTotalTimeoutConstant = 50;
            internal_drivers[driver_index].timeouts.ReadTotalTimeoutMultiplier = 10;
            internal_drivers[driver_index].timeouts.WriteTotalTimeoutConstant = 50;
            internal_drivers[driver_index].timeouts.WriteTotalTimeoutMultiplier = 10;

            success &= SetCommTimeouts(internal_drivers[driver_index].driver_h,
                                        &internal_drivers[driver_index].timeouts);
        }

        if (success)
        {
            internal_drivers[driver_index].uart_driver.initialized = true;
        }
        else
        {
            EZERROR("  Windows uart driver init error");
        }
    }

    return success;
}


static bool ezWinUart_Deinitialize(uint8_t driver_index)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);
    EZDEBUG("ezWinUart_Deinitialize(index = %d)", driver_index);
    bool success = true;

    if (driver_index >= NUM_OF_UART_INTERFACE)
    {
        success = false;
    }
    else
    {
        success = success && CloseHandle(internal_drivers[driver_index].driver_h);
    }

    if (!success)
    {
        EZERROR("Port cannot be closed!!!");
    }

    return success;
}


static uint32_t ezWinUart_WriteBlocking(uint8_t driver_index,
                                        uint8_t *data,
                                        uint32_t data_size)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);
    ASSERT(data != NULL);
    EZDEBUG("ezWinUart_WriteBlocking(index = %d, data size = %d)", driver_index, data_size);

    uint32_t num_byte_written = 0;
    uint32_t remain_bytes = 0;

    bool success = true;

    if (driver_index < NUM_OF_UART_INTERFACE && data && data_size > 0)
    {
        do
        {
            remain_bytes = data_size - num_byte_written;

            success &= WriteFile(internal_drivers[driver_index].driver_h,
                                 &data[num_byte_written],
                                 remain_bytes,
                                 &num_byte_written,
                                 NULL);

            remain_bytes -= num_byte_written;
        } while (success == true && remain_bytes > 0);

        EZDEBUG("  wrote %lu(%lu) bytes", num_byte_written, data_size);
        EZHEXDUMP(data, num_byte_written);
    }

    return num_byte_written;
}


static uint32_t ezWinUart_ReadBlocking(uint8_t driver_index,
                                       uint8_t *data,
                                       uint32_t data_size)
{
    ASSERT(driver_index < NUM_OF_UART_INTERFACE);
    ASSERT(data != NULL);
    EZDEBUG("ezWinUart_ReadBlocking(index = %d, data size = %d)", driver_index, data_size);

    uint32_t num_byte_read = 0;
    uint32_t remain_bytes = 0;

    bool success = true;

    if (driver_index < NUM_OF_UART_INTERFACE && data && data_size > 0)
    {
        do
        {
            remain_bytes = data_size - num_byte_read;

            success &= ReadFile(internal_drivers[driver_index].driver_h,
                                &data[num_byte_read],
                                remain_bytes,
                                &num_byte_read,
                                NULL);

            remain_bytes -= num_byte_read;
        } while (success == true && remain_bytes > 0);

        EZDEBUG("  read %lu(%lu) bytes", num_byte_read, data_size);
        EZHEXDUMP(data, num_byte_read);
    }

    return num_byte_read;
}
/* End of file*/

