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
#include "utilities/linked_list/linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**@brief List to manage the memory block
 *
 */
struct MemList
{
    struct Node free_list_head;      /**< list to manage the free memory blocks */
    struct Node alloc_list_head;     /**< list to manage the allocated blocks*/
    uint8_t* buff;        /**< pointer to the memory */
    uint16_t buff_size;    /**< size of the buffer */
};

/**@brief Memory block
 *
 */
struct MemBlock
{
    struct Node node;
    void* buff;
    uint16_t buff_size;
};


/**@brief opauue pointer for MemList
 *
 */
typedef struct MemList ezmMemList;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

void        ezmStcMem_Initialization(void);
bool        ezmStcMem_InitMemList   (ezmMemList* mem_list, void* buff, uint16_t buff_size);
void        *ezmStcMem_Malloc       (ezmMemList* mem_list, uint16_t alloc_size);
bool        ezmStcMem_Free          (ezmMemList *mem_list, void *alloc_addr);

uint16_t    ezmStcMem_GetNumOfAllocBlock(ezmMemList* mem_list);
uint16_t    ezmStcMem_GetNumOfFreeBlock(ezmMemList* mem_list);
void        ezmStcMem_HexdumpBuffer (ezmMemList* mem_list);
void        ezmStcMem_PrintFreeList (ezmMemList* mem_list);
void        ezmStcMem_PrintAllocList(ezmMemList* mem_list);


#endif /* _STCMEM_H */

/* End of file*/