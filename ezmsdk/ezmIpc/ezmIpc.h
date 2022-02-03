/*******************************************************************************
* Title                 :   IPC module
* Filename              :   ezmIpc.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   09.08.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  09.08.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   ezmIpc.h
 *  @brief  Header template for a module
 */

#ifndef _IPC_H
#define _IPC_H


/*******************************************************************************
* Includes
*******************************************************************************/
#include "../app/app_config.h"

#if (IPC == 1U)
#include "stdint.h"
#include "stdbool.h"
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef uint32_t(*ezmIpc_MessageCallback)(void);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void    ezmIpc_InitModule       (void);
bool    ezmIpc_GetInstance      (uint8_t owner_id, uint8_t* ipc_buffer, uint16_t buffer_size, ezmIpc_MessageCallback fnCallback);
void*   ezmIpc_InitMessage      (uint8_t send_to_id, uint16_t size_in_byte);
bool    ezmIpc_SendMessage      (uint8_t send_to_id, void *message);
void*   ezmIpc_ReceiveMessage   (uint8_t owner_id, uint16_t *message_size);
bool    ezmIpc_ReleaseMessage   (uint8_t owner_id, void *message);

#endif /* IPC */
#endif /* _IPC_H */

/* End of file*/
