/*******************************************************************************
* Title                 :   Static memory component
* Filename              :   stcmem.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   01.02.2022
* Version               :   1.0.0
*
*******************************************************************************/

/** @file   stcmem.h
 *  @brief  Header file for the static memory module
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
    LinkedList  free_list;      /**< list to manage the free memory blocks */
    LinkedList  alloc_list;     /**< list to manage the allocated blocks*/
    uint8_t     *buffer;        /**< pointer to the memory */
    uint16_t    buffer_size;    /**< size of the buffer */
}ezmMemList;

/**@brief Header to manage a memory block
 *
 */
typedef Node MemHdr;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmStcMem_Initialization(void);
bool        ezmStcMem_InitMemList   (ezmMemList* mem_list, uint8_t* buffer, uint16_t buffer_size);
void        *ezmStcMem_Malloc       (ezmMemList* mem_list, uint16_t alloc_size);
bool        ezmStcMem_Free          (ezmMemList* mem_list, void* alloc_addr);
void        ezmStcMem_HexdumpBuffer (ezmMemList* mem_list);
void        ezmStcMem_PrintFreeList (ezmMemList* mem_list);
void        ezmStcMem_PrintAllocList(ezmMemList* mem_list);

#endif /* _STCMEM_H */

/* End of file*/