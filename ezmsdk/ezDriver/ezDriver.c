
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
#include "ezDriver.h"

#if (CONFIG_DRIVERINF == 1U)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "ezDriver"      /**< module name */

#include "utilities/logging/logging.h"
#include "ezKernel/ezKernel.h"
#include "dummy_driver.h"
#include <string.h>


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define TASK_EXEC_DELAY     0   /**< task is executed after 0 millisecond */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief -
 *
 */
struct ezDriverConfig
{
    char *driver_name;          /**< */
    char version[3];            /**< */
    struct ezDriver *driver;    /**< */
    void *(*LinkDriverFunction)(void);  /**< */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
struct ezDriverConfig ConfigurationTable[] = {
    /* name              version  handle      LinkDriverFunction */
    { "dummy_driver",    {1,0,0}, NULL,       LinkDummyDriver },

    /*End of configuration table */
    { NULL, {0,0,0}, NULL, NULL}
};


/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* External functions
*******************************************************************************/
void ezDriver_Initialize(void)
{
    struct ezDriverConfig *config = ConfigurationTable;

    while (config->LinkDriverFunction != NULL)
    {
        config->driver = (struct ezDriver *)config->LinkDriverFunction();

        if (config->driver != NULL)
        {
            INFO("Link driver success");
            INFO("  name = %s", config->driver_name);
            INFO("  version = %d.%d.%d",
                config->version[0],
                config->version[1],
                config->version[2]);

            if (config->driver->Initialize() == ezSUCCESS)
            {
                INFO("  Initialize driver success");

                if (config->driver->ll_interface)
                {
                    INFO("  low level driver is available");

                }
                if (config->driver->std_interface == NULL)
                {
                    ERROR("  No standard interface");
                    config->driver = NULL;
                }
            }
            else
            {
                ERROR("Initialize driver fail");
            }
        }
        else
        {
            INFO("Cannot link driver");
        }

        config++;
    }
}


ezDriveHandle ezDriver_GetDriver(char *driver_name)
{
    struct ezDriverConfig *config = ConfigurationTable;
    struct ezDriver *ret_driver = NULL;

    while (config->LinkDriverFunction != NULL)
    {
        if (strncmp(driver_name,
            config->driver_name,
            strlen(config->driver_name)) == 0)
        {
            ret_driver = config->driver;

            DEBUG("found %s", driver_name);
            break;
        }

        config++;
    }

    return ret_driver;
}


ezSTATUS ezDriver_Open(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ConfigurationTable;
    ezSTATUS status = ezFAIL;

    DEBUG("ezDriver_Open()");

    while (config->LinkDriverFunction != NULL)
    {
        if (handle == config->driver)
        {
            DEBUG("driver = %s found. Calling Open()", config->driver_name);

            status = ezKernel_AddTask(config->driver->std_interface->Open,
                TASK_EXEC_DELAY,
                data,
                sizeof(struct ezStdInterfaceData));
            break;
        }

        config++;
    }

    return status;
}


ezSTATUS ezDriver_Write(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ConfigurationTable;
    ezSTATUS status = ezFAIL;

    while (config->LinkDriverFunction != NULL)
    {
        if (handle == config->driver)
        {
            DEBUG("driver = %s found. Calling Write()", config->driver_name);

            status = ezKernel_AddTask(config->driver->std_interface->Write,
                TASK_EXEC_DELAY,
                (void *)data,
                sizeof(struct ezStdInterfaceData));
            break;
        }

        config++;
    }

    return status;
}


ezSTATUS ezDriver_Read(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ConfigurationTable;
    ezSTATUS status = ezFAIL;

    while (config->LinkDriverFunction != NULL)
    {
        if (handle == config->driver)
        {
            DEBUG("driver = %s found. Calling Read()", config->driver_name);

            status = ezKernel_AddTask(config->driver->std_interface->Read,
                TASK_EXEC_DELAY,
                data,
                sizeof(struct ezStdInterfaceData));
            break;
        }

        config++;
    }

    return status;
}


ezSTATUS ezDriver_Close(ezDriveHandle handle, struct ezStdInterfaceData *data)
{
    struct ezDriverConfig *config = ConfigurationTable;
    ezSTATUS status = ezFAIL;

    while (config->LinkDriverFunction != NULL)
    {
        if (handle == config->driver)
        {
            DEBUG("driver = %s found. Calling Close()", config->driver_name);

            status = ezKernel_AddTask(config->driver->std_interface->Close,
                TASK_EXEC_DELAY,
                data,
                sizeof(struct ezStdInterfaceData));
            break;
        }

        config++;
    }

    return status;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* CONFIG_DRIVERINF == 1U */

/* End of file*/

