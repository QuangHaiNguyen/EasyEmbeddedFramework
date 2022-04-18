/*******************************************************************************
* Title                 :   my_math component
* Filename              :   my_math.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   my_math.h
 *  @brief  Header file for the ring buffer
 */


#ifndef _SMALLOC_H
#define _SMALLOC_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if(SMALLOC == 1U)
#include "../linked_list/linked_list.h"
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**@brief List to manage the memory block
 *
 */
typedef LinkedList ezmMemList;

/**@brief Memory block description
 *
 */
typedef Node ezmMemoryBlock;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void                ezmSmalloc_Initialize (void);
void *              ezmSmalloc_Malloc (uint16_t u16Size);
void                emzSmalloc_Free (void * address);
void                ezmSmalloc_InitMemList (ezmMemList * pstNewList, uint8_t u8ListOwnerModuleId);
ezmMemoryBlock *    ezmSmalloc_GetMemBlockInList (ezmMemList * pstNewList, uint16_t u16BlockSize);
void                ezmSmalloc_ReturnMemBlock (ezmMemList * pstNewList, ezmMemoryBlock * pstBlock);
ezmMemoryBlock *    ezmSmalloc_GetFreeBlock (uint16_t u16BlockSize);
void                ezmSmalloc_ApendBlockToList (ezmMemoryBlock * pstBlock, ezmMemList * pstNewList);
#endif

#endif /* _SMALLOC_H */

/* End of file*/