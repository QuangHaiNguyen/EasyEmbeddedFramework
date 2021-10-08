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

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void                ezmIpc_Init             (void);
bool                ezmIpc_RegisterModule   (uint8_t u8ModuleId);
ezmMemoryBlock *    ezmIpc_InitMsg          (uint8_t u8MsgSize);
bool                ezmIpc_DeInitMsg        (uint8_t u8ModuleId, ezmMemoryBlock* pstMsg);
bool                ezmIpc_SendMsg          (uint8_t u8FromModuleId, uint8_t u8ToModuleId, ezmMemoryBlock* pstMsg);
ezmMemoryBlock *    ezmIpc_ReceiveMsg       (uint8_t u8ModuleId);

#endif /* IPC */
#endif /* _IPC_H */

/* End of file*/
