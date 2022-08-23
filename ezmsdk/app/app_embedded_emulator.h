
/*******************************************************************************
* Filename:         app_embedded_emulator.h
* Author:           Hai Nguyen
* Original Date:    21.08.2022
* Last Update:      21.08.2022
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
* Written by Hai Nguyen 21.08.2022
*
*******************************************************************************/

/** @file   app_embedded_emulator.h
 *  @author Hai Nguyen
 *  @date   21.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _APP_EMBEDDED_EMULATOR_H
#define _APP_EMBEDDED_EMULATOR_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if(CONFIG_EMBEDDED_EMULATOR == 1U)
#include <stdint.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief the opcode used by the emulator
 *  
 */
typedef enum
{
    OPCODE_ECHO,    /* opcode for eho command*/
    NUM_OF_OPCODE,  /* number of opcode */
}EZ_OPCODE;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Function : ezMbedEmulator_Initialization
*//**
* @Description:
*
* This function initializes embedded emulator application
*
* @param    None
* @return   ezSUCCESS if success
*           ezFAIL if fail
*
* @Example Example:
* @code
* if(ezMbedEmulator_Initialization() == ezSUCCESS)
* {
*     printf("success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezMbedEmulator_Initialization(void);


/******************************************************************************
* Function : ezMbedEmulator_Run
*//**
* @Description:
*
* Run the embedded emulator application. Must be called in the tick fucntion,
* infinite loop or using kernel module
*
* @param    None
* @return   None
*
* @Example Example:
* @code
* while(1)
* {
*     ezMbedEmulator_Run();
* }
* @endcode
*
*******************************************************************************/
void ezMbedEmulator_Run(void);


/******************************************************************************
* Function : ezMbedEmulator_SendData
*//**
* @Description:
*
* This function send the data to the embedded emulator application. Internally,
* the data is push into a queue and waiting to be proccessed
*
* @param    opcode: (IN)opcode of the data to be sent
* @param    *data: (IN)data to be sent
* @param    data_size: (IN)size of the data
* @return   ezSUCCESS:success
*           ezFAIL: internal queue is full
*
* @Example Example:
* @code
* uint8_t opcode = 0x22;
* uint8_t data_to_send[32] = {0xaa};
* if(ezMbedEmulator_SendData(opcode, (void*)data_to_send, 32))
* {
*     printf("success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezMbedEmulator_SendData(uint8_t opcode, void *data, uint32_t data_size);


/******************************************************************************
* Function : ezMbedEmulator_GetQueueElement
*//**
* @Description:
*
* This function creates an element in the queue with a defined size and returns
* the pointer to the buffer for writting data.
*
* @param    *queue_buffer: (OUT) pointer to the buffer of the element
* @param    data_size: (IN)size of the data
* @return   ezSUCCESS:success
*           ezFAIL: internal queue is full
*
* @Example Example:
* @code
* uint8_t *data = NULL;
* if(ezMbedEmulator_GetQueueElement(&(void*)data, 32) == ezSUCCESS)
* {
*     do something
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezMbedEmulator_GetQueueElement(void **queue_buffer, uint32_t data_size);


#endif /* CONFIG_EMBEDDED_EMULATOR */
#endif /* _APP_EMBEDDED_EMULATOR_H */

/* End of file */

