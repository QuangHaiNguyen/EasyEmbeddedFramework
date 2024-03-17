/*****************************************************************************
* Filename:         ez_uart.c
* Author:           Hai Nguyen
* Original Date:    15.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_uart.c
 *  @author Hai Nguyen
 *  @date   15.03.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_uart.h"

#if (EZ_UART_ENABLE == 1)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "ez_uart"       /**< module name */
#include "ez_logging.h"

#include <string.h>


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static struct Node hw_driver_list = EZ_LINKEDLIST_INIT_NODE(hw_driver_list);
/**< List of HW driver implementation  */

static struct Node instance_list = EZ_LINKEDLIST_INIT_NODE(instance_list);
/**< Keep tracks of instance register to this driver */

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Public functions
*****************************************************************************/
EZ_DRV_STATUS ezUart_SystemRegisterHwDriver(struct ezUartDriver *hw_uart_driver)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    EZTRACE("ezUart_SystemRegisterHwDriver()");
    if(hw_uart_driver == NULL)
    {
        status = STATUS_ERR_ARG;
        EZERROR("hw_uart_driver == NULL");
    }
    else
    {
        EZ_LINKEDLIST_ADD_TAIL(&hw_driver_list, &hw_uart_driver->ll_node);
        status = STATUS_OK;
    }

    return status;
}


EZ_DRV_STATUS ezUart_SystemUnregisterHwDriver(struct ezUartDriver *hw_uart_driver)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    EZTRACE("ezUart_SystemUnregisterHwDriver()");
    if(hw_uart_driver == NULL)
    {
        status = STATUS_ERR_ARG;
        EZERROR("hw_uart_driver == NULL");
    }
    else
    {
        EZ_LINKEDLIST_UNLINK_NODE(&hw_uart_driver->ll_node);
        status = STATUS_OK;
    }

    return status;
}



EZ_DRV_STATUS ezUart_RegisterInstance(ezUartDrvInstance_t *inst,
                                      const char *driver_name)
{
    EZ_DRV_STATUS status = STATUS_ERR_DRV_NOT_FOUND;
    struct Node* it_node = NULL;
    struct ezUartDriver *uart_drv = NULL;

    EZTRACE("ezUart_RegisterInstance(name = %s)", driver_name);
    if((inst == NULL) || (driver_name == NULL))
    {
        status = STATUS_ERR_ARG;
    }
    else
    {
        EZ_LINKEDLIST_FOR_EACH(it_node, &hw_driver_list)
        {
            uart_drv = EZ_LINKEDLIST_GET_PARENT_OF(it_node, ll_node, struct ezUartDriver);
            if(strcmp(uart_drv->common.name, driver_name) == 0)
            {
                inst->driver = (void*)uart_drv;
                status = STATUS_OK;
                EZDEBUG("Found driver!");
                break;
            }
        }
    }

    return status;
}


EZ_DRV_STATUS ezUart_UnregisterInstance(ezUartDrvInstance_t *inst)
{
    EZTRACE("ezUart_UnregisterInstance()");
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    if(inst == NULL)
    {
        status = STATUS_ERR_ARG;
    }
    else
    {
        inst->driver = NULL;
        EZDEBUG("unregister success");
        status = STATUS_OK;
    }

    return status;
}


EZ_DRV_STATUS ezUart_Initialize(ezUartDrvInstance_t *inst)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;
    struct ezUartDriver *drv = NULL;

    EZTRACE("ezUart_Initialize()");
    if(inst == NULL)
    {
        status = STATUS_ERR_ARG;
    }
    else if (inst->driver == NULL)
    {
        status = STATUS_ERR_DRV_NOT_FOUND;
        EZERROR("Driver not found");
    }
    else
    {
        EZTRACE("Found driver");
        drv = (struct ezUartDriver*)inst->driver;
        if(drv->common.curr_inst == NULL || drv->common.curr_inst == inst)
        {
            drv->common.curr_inst = inst;
            EZTRACE("Driver = %sis available", drv->common.name);
            if(drv->interface.initialize)
            {
                status = drv->interface.initialize(drv->interface.index);
            }
            else
            {
                status = STATUS_ERR_INF_NOT_EXIST;
                EZWARNING("initialize interface is not implemented");
            }
        }
        else
        {
            status = STATUS_BUSY;
            EZWARNING("Driver = %s is busy", drv->common.name);
        }
    }

    return status;
}



/*****************************************************************************
* Local functions
*****************************************************************************/

#endif /* EZ_UART_ENABLE == 1 */
/* End of file*/
