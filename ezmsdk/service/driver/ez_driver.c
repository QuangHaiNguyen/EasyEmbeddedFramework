
/*******************************************************************************
* Filename:         ezDriver.c
* Author:           Hai Nguyen
* Original Date:    20.09.2022
* Last Update:      20.09.2022
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
* Written by Hai Nguyen 20.09.2022
*
*******************************************************************************/

/** @file   ezDriver.c
 *  @author Hai Nguyen
 *  @date   20.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_driver.h"

#if (EZ_GENERIC_DRIVER == 1U)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "ezDriver"      /**< module name */

#include "utilities/logging/ez_logging.h"
#include "dummy_driver.h"
#include <string.h>

#if (CONFIG_HAL_UART == 1U)
#include "ezHal/uart/uart.h"
#endif /* CONFIG_HAL_UART == 1U */

#if (CONFIG_HAL_I2C == 1U)
#include "ezHal/hal_i2c/hal_i2c.h"
#endif /* CONFIG_HAL_I2C == 1U */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define TASK_EXEC_DELAY     0   /**< task is executed after 0 millisecond */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Store the driver configuration, each element of the configuration
 *  will be linked with a ezDriver implmentation from the HAL level.
 */
struct ezDriverConfig
{
    char *driver_name;          /**< Name of the driver */
    char version[3];            /**< Version of the driver */
    struct ezDriver *driver;    /**< Pointer to the actual driver struct */
    void *(*LinkDriverFunction)(void);  /**< Pointer to the link driver function */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
struct ezDriverConfig ConfigurationTable[] = {
    /* name                 version     handle      LinkDriverFunction */
    { "dummy_driver",       {1,0,0},    NULL,       LinkDummyDriver },
    
#if (CONFIG_HAL_UART == 1U)
    { "cli_uart",           {1,0,0},    NULL,       ezHal_Uart_LinkCliDriv },
#endif /*CONFIG_HAL_UART*/

#if (CONFIG_HAL_I2C == 1U)
    { "i2c",                {1,0,0},    NULL,       ezI2c_LinkDriver},
#endif /* CONFIG_HAL_I2C == 1U */

#if (CONFIG_VIRTUAL_COM == 1U)
    { "virtual_com",        {1,0,0},    NULL,       ezHal_VirtualCom_LinkDriv},
#endif /* CONFIG_VIRTUAL_COM == 1U */

    /*End of configuration table - DO NOT CHANGE!!! */
    { NULL, {0,0,0}, NULL, NULL}
};


/******************************************************************************
* Function Definitions
*******************************************************************************/
struct ezDriverConfig *ezDriver_SearchDriverConfig(ezDriveHandle handle);


/******************************************************************************
* External functions
*******************************************************************************/
void ezDriver_Initialize(void)
{
    struct ezDriverConfig *config = ConfigurationTable;

    while (config->driver_name != NULL)
    {
        config->driver = (struct ezDriver *)config->LinkDriverFunction();

        if (config->driver != NULL)
        {
            EZINFO("Link driver success");
            EZINFO("  name = %s", config->driver_name);
            EZINFO("  version = %d.%d.%d",
                config->version[0],
                config->version[1],
                config->version[2]);

            if (config->driver->Initialize)
            {
                if (config->driver->Initialize() == ezSUCCESS)
                {
                    EZINFO("  Initialize driver success");

                    if (config->driver->ll_interface)
                    {
                        EZINFO("  low level driver is available");
                    }

                    if (config->driver->std_interface == NULL)
                    {
                        EZERROR("  No standard interface");
                        config->driver = NULL;
                    }
                }
                else
                {
                    EZERROR("Initialize driver fail");
                }
            }
            else
            {
                EZERROR("No initialize function");
            }
        }
        else
        {
            EZINFO("Cannot link driver");
        }

        config++;
    }
}


ezDriveHandle ezDriver_GetDriver(char *driver_name)
{
    struct ezDriverConfig *config = ConfigurationTable;
    struct ezDriver *ret_driver = NULL;

    while (config->driver_name != NULL)
    {
        if (strncmp(driver_name,
            config->driver_name,
            strlen(config->driver_name)) == 0)
        {
            ret_driver = config->driver;

            EZDEBUG("found %s", driver_name);
            break;
        }

        config++;
    }

    return ret_driver;
}


ezSTATUS ezDriver_Open(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ezDriver_SearchDriverConfig(handle);
    ezSTATUS status = ezFAIL;

    EZDEBUG("ezDriver_Open()");

    if (config != NULL)
    {
        EZDEBUG("driver = %s found. Calling Open()", config->driver_name);

        status = ezKernel_AddTask(config->driver->std_interface->Open,
            TASK_EXEC_DELAY,
            data,
            sizeof(struct ezStdInterfaceData));
    }

    return status;
}


ezSTATUS ezDriver_Write(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ezDriver_SearchDriverConfig(handle);
    ezSTATUS status = ezFAIL;

    if (config != NULL)
    {
        EZDEBUG("driver = %s found. Calling Write()", config->driver_name);

        status = ezKernel_AddTask(config->driver->std_interface->Write,
            TASK_EXEC_DELAY,
            (void *)data,
            sizeof(struct ezStdInterfaceData));
    }

    return status;
}


ezSTATUS ezDriver_Read(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ezDriver_SearchDriverConfig(handle);;
    ezSTATUS status = ezFAIL;

    if (config != NULL)
    {
        EZDEBUG("driver = %s found. Calling Read()", config->driver_name);

        status = ezKernel_AddTask(config->driver->std_interface->Read,
            TASK_EXEC_DELAY,
            data,
            sizeof(struct ezStdInterfaceData));
    }

    return status;
}


ezSTATUS ezDriver_Close(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ezDriver_SearchDriverConfig(handle);
    ezSTATUS status = ezFAIL;

    if (config != NULL)
    {
        EZDEBUG("driver = %s found. Calling Close()", config->driver_name);

        status = ezKernel_AddTask(config->driver->std_interface->Close,
            TASK_EXEC_DELAY,
            data,
            sizeof(struct ezStdInterfaceData));
    }

    return status;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : ezDriver_SearchDriverConfig
*//**
* @Description: Search for the driver in the configuration table
*
* @param    handle: (IN)Driver handle
* @return   Configuration or null
*
*
*******************************************************************************/
struct ezDriverConfig *ezDriver_SearchDriverConfig(ezDriveHandle handle)
{
    struct ezDriverConfig *config = ConfigurationTable;

    while (config->driver_name != NULL)
    {
        if (handle == config->driver)
        {
            break;
        }

        config++;
    }

    return config;
}

#endif /* CONFIG_DRIVERINF == 1U */

/* End of file*/

