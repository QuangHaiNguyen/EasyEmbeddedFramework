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
#include "smalloc_conf.h"
#include "stdint.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/**
 *
 * @brief   Contain the HAL API to interact with the underlying wireless module. 
 *          User must point these function pointers to the actual implementation
**/

typedef struct LinkedList ezmMemList;
typedef struct Node ezmMemoryBlock;
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmSmalloc_Initialize (void);
void *      ezmSmalloc_Malloc (uint16_t u16Size);
void        emzSmalloc_Free (void * address);
void                ezmSmalloc_InitMemList (ezmMemList * pstNewList, uint8_t u8ListOwnerModuleId);
ezmMemoryBlock *    ezmSmalloc_GetMemBlockInList (ezmMemList * pstNewList, uint16_t u16BlockSize);
void                ezmSmalloc_ReturnMemBlock (ezmMemList * pstNewList, ezmMemoryBlock * pstBlock);
ezmMemoryBlock *    ezmSmalloc_GetFreeBlock (uint16_t u16BlockSize);
void                ezmSmalloc_ApendBlockToList (ezmMemoryBlock * pstBlock, ezmMemList * pstNewList);

#if (SMALLOC_ANALYSIS == 1U)
void ezmSmalloc_PrintFreeListHead();
void ezmSmalloc_PrintFreeList();
void ezmSmalloc_PrintMetadata(Block_t * str_ptrMetaData);
void ezmSmalloc_Hexdump(int * address, unsigned int size);
#endif /* SMALLOC_ANALYSIS */

#endif /* _SMALLOC_H */

/* End of file*/