/*******************************************************************************
* Title                 :   serial module 
* Filename              :   serial.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*******************************************************************************/

/*************** SOURCE REVISION LOG *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Initial Release.
*
*******************************************************************************/
/** @file  serial.c
 *  @brief This is the source file for the serial component. It contain a wrapper
 *  for the serial communication protocol
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "serial.h"


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
HAL_Status eSerialStatus = HAL_ERR; /* status of the serial module*/
bool bIsInit = false;

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function : Serial_Init
*//** 
* \b Description:
*
* This function initializes the serial module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    size: (IN)size of the internal buffer
* @return   SERIAL_ERR - not enough memory
*           SERIAL_OK - init complete
*
* \b Example Example:
* @code
* Serial_Init(10)
* @endcode
*
* @see Serial_Init
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
HAL_Status Serial_Init(void)
{
#if defined (SIMULATION)
    #warning runnig simulation
    bIsInit = true;
    eSerialStatus = HAL_OK;

#elif defined (EMBEDDED)
    /* Init hardware*/

#else
    #warning no platform is define 
#endif

#if defined (USING_CALLBACK)
    /* Register callback*/
#endif

    return eSerialStatus;
}

bool Serial_IsInit(void)
{
    return bIsInit;
}

/******************************************************************************
* Function : Serial_Receive
*//** 
* \b Description:
*
* This function starts the reception of the data in the hardware peripheral
*
* PRE-CONDITION: Serial_Init(params) is successfull
*
* POST-CONDITION: None
* 
* @param    size: (IN)number of byte being received
* @return   SERIAL_NO_MEM - don't have enough memory to receive the meintioned size
*           SERIAL_OK - init complete
*
* \b Example Example:
* @code
* Serial_Receive(10)
* @endcode
*
* @see Serial_Init
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
HAL_Status Serial_Receive(HAL_UART *uart, uint8_t size)
{
    if(size > BUFF_SIZE)
    {
        eSerialStatus = HAL_NO_MEM;
        PRINT_ERROR("%s\n","not enough memory");
    }
    else
    {

        /*Start to write data to buffer*/
#if defined (SIMULATION)
        eSerialStatus = HAL_RX_BUSY;
        uint8_t byte;
        for(uint8_t i = 0; i < size; i++)
        {
            uart->ptr_u8Buffer[i] = (uint8_t)getchar();
            PRINT_DEBUG("receive: %c\n", uart->ptr_u8Buffer[i]);
        }
        eSerialStatus = HAL_OK;
#elif defined(EMBEDDED) /*embedded system*/
        /*embedded dependent libraries*/
        /*UART Library*/
#else
    #error unknow platform
#endif
    }
    return eSerialStatus;
}


/******************************************************************************
* Function : Serial_Transmit
*//** 
* \b Description:
*
* This function start the data transmission of the hardware peripheral
*
* PRE-CONDITION: Serial_Init(params) is successfull
*
* POST-CONDITION: None
* 
* @param    data: (IN)pointer to data to be transmitted
* @param    size: (IN)number of byte being transmitted
* @return   SERIAL_NO_MEM - don't have enough memory to store the transmitting data
*           SERIAL_OK - init complete
*
* \b Example Example:
* @code
* uint8_t data[3] = {0x01, 0x02, 0x03};
* Serial_Transmit(data, 3)
* @endcode
*
* @see Serial_Init
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
HAL_Status Serial_Transmit(HAL_UART *uart, uint8_t size)
{

    if(size > BUFF_SIZE)
    {
        eSerialStatus = HAL_NO_MEM;
        PRINT_ERROR("%s\n","not enough memory");
    }
    else
    {
        eSerialStatus = HAL_TX_BUSY;
#if defined (SIMULATION)
        for(uint8_t i = 0; i < size; i++)
        {
            PRINT_INFO("%c\n", (char)uart->ptr_u8Buffer[i]);
        }
#elif defined(EMBEDDED) /*embedded system*/
    /*embedded dependent libraries*/
    /*UART Library*/

#else
    #error unknow platform
    
#endif
        eSerialStatus = HAL_OK;
    }
    return eSerialStatus;
}

/******************************************************************************
* Function : Serial_GetStatus
*//** 
* \b Description:
*
* This function returns the status of the serial module
*
* PRE-CONDITION: Serial_Init(params) is successfull
*
* POST-CONDITION: None
* 
* @param    none
* @return   SerialStatus
*
* \b Example Example:
* @code
* SerialStatus status;
* status = Serial_GetStatus();
* @endcode
*
* @see Serial_Init
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
HAL_Status Serial_GetStatus(void)
{
    return eSerialStatus;
}

/******************************************************************************
* Function : Serial_DeInit
*//** 
* \b Description:
*
* This function deinit the serial module and free the buffer
*
* PRE-CONDITION: Serial_Init(params) is successfull
*
* POST-CONDITION: None
* 
* @param    none
* @return   SERIAL_OK - OK
*
* \b Example Example:
* @code
* Serial_DeInit();
* @endcode
*
* @see Serial_Init
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
HAL_Status Serial_DeInit(void)
{
    
#if defined(_WIN32) || defined(__linux__)
    #warning simulation, nothing to do
    eSerialStatus = HAL_OK;
#elif defined(EMBEDDED) /*embedded system*/
    /*embedded dependent libraries*/
    /*UART Library*/
    eSerialStatus = HAL_OK;
#else
    #error unknow platform
#endif
    return eSerialStatus;
}