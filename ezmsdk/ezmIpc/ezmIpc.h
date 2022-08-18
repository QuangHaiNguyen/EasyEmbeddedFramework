/*******************************************************************************
* Title                 :   ezmIpc
* Filename              :   ezmIpc.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   03.02.2022
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
#include "app/app_config.h"

#if (CONFIG_IPC == 1U)
#include "stdint.h"
#include "stdbool.h"
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define IPC_INVALID         NUM_OF_IPC_INSTANCE

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef uint32_t(*ezmIpc_MessageCallback)(void);
typedef uint32_t ezmMailBox;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmIpc_InitModule       (void);
ezmMailBox  ezmIpc_GetInstance      (uint8_t* ipc_buffer, uint16_t buffer_size, ezmIpc_MessageCallback fnCallback);
void*       ezmIpc_InitMessage      (ezmMailBox send_to, uint16_t size_in_byte);
bool        ezmIpc_SendMessage      (ezmMailBox send_to, void *message);
void*       ezmIpc_ReceiveMessage   (ezmMailBox receive_from, uint16_t *message_size);
bool        ezmIpc_ReleaseMessage   (ezmMailBox receive_from, void *message);

#endif /* IPC */
#endif /* _IPC_H */

/* End of file*/
