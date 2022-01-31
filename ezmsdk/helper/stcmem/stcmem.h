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


#ifndef _STCMEM_H
#define _STCMEM_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "../linked_list/linked_list.h"
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

#define MEMHDR_INVALID_ID     0xFFFF

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**@brief List to manage the memory block
 *
 */
typedef struct
{
    LinkedList  stFreeList;
    LinkedList  stAllocList;
    uint8_t*    pau8Buffer;
    uint16_t    u16BufferSize;
}ezmMemList;

/**@brief Memory block description
 *
 */
typedef Node MemHdr;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
void        ezmStcMem_Initialization    (void);
bool        ezmStcMem_InitMemList       (ezmMemList* pstList, uint8_t* pu8Buffer, uint16_t u16BufferSize);
void*       ezmStcMem_Malloc            (ezmMemList* pstList, uint16_t u16Size);
bool        ezmStcMem_Free              (ezmMemList* pstList, void * pAddress);

void        ezmStcMem_HexdumpBuffer     (ezmMemList* pstList);
void        ezmStcMem_PrintFreeList     (ezmMemList* pstList);
void        ezmStcMem_PrintAllocList    (ezmMemList* pstList);
/******************************************************************************
* Function Prototypes
*******************************************************************************/


#endif /* _STCMEM_H */

/* End of file*/