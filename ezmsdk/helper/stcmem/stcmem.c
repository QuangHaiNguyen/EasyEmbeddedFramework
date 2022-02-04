/*******************************************************************************
* Title                 :   static memory module 
* Filename              :   stcmem.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   01.02.2022
* Version               :   1.0.0
*******************************************************************************/


/** @file  stcmem.c
 *  @brief This is the source file for static memory allocation. It is basically,
 *          works as the normal malloc, but the memory is allocated from a static
 *          memory buffer
 */

/******************************************************************************
 Includes
******************************************************************************/
#include "../../app/app_config.h"

#if (STCMEM == 1U)

#include "../linked_list/linked_list.h"
#include "../hexdump/hexdump.h"
#include "stcmem.h"
#include "stdbool.h"
#include <string.h>

/******************************************************************************
 Module Preprocessor Macros
*******************************************************************************/
#define VERBOSE         1U

#define MOD_NAME        "STATIC_MEM"
#if (MODULE_DEBUG == 1U) && (STCMEM_DEBUG == 1U)
    #define STCMEMPRINT(a)            PRINT_DEBUG(MOD_NAME,a)
    #define STCMEMPRINT1(a,b)         PRINT_DEBUG1(MOD_NAME,a,b)
    #define STCMEMPRINT2(a,b,c)       PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define STCMEMPRINT3(a,b,c,d)     PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define STCMEMPRINT4(a,b,c,d,e)   PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
    #define STCMEMHEXDUMP(a,b)        ezmHexdump(a,b)
#else 
    #define STCMEMPRINT(a)
    #define STCMEMPRINT1(a,b)
    #define STCMEMPRINT2(a,b,c)
    #define STCMEMPRINT3(a,b,c,d)
    #define STCMEMPRINT4(a,b,c,d,e)
    #define STCMEMHEXDUMP(a,b)
#endif


/******************************************************************************
 Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
 Module Variable Definitions
*******************************************************************************/
static MemHdr   header_pool[NUM_OF_MEMHDR] = { 0 };

/******************************************************************************
 Function Definitions
*******************************************************************************/
MemHdr          *ezmStcMem_ReserveMemoryBlock   (LinkedList* free_list, uint16_t block_size_byte);
bool            ezmStcMem_MoveHeader            (MemHdr* header, LinkedList* from_list, LinkedList* to_list);
static void     ezmStcMem_ResetHeader           (uint16_t header_index);
static MemHdr   *ezmStcMem_GetFreeHeader        (void);
static void     ezmStcMem_ReturnHeaderToFreeList(LinkedList *free_list, MemHdr *free_header);
static void     ezmSmalloc_Merge                (LinkedList *free_list);

/**************************** Public function ********************************/

/******************************************************************************
* Function : ezmStcMem_Initialization
*//** 
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezmStcMem_Initialization(void)
{
    STCMEMPRINT("ezmStcMem_Initialization()");

    /* Reset the list of memory header*/
    for (uint16_t i = 0; i < NUM_OF_MEMHDR; i++)
    {
        ezmStcMem_ResetHeader(i);
    }
}


/******************************************************************************
* Function : ezmStcMem_InitMemList
*//**
* \b Description:
*
* This function initializes memory handle to manage the memory buffer. After
* the initialization, the memory buffer can not be used directly, but thru the
* ezmStcMem API
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:      handle to manage memory buffer
* @param    *buffer:        buffer to be managed
* @param    buffer_size:    size of the buffer
* @return   True is initialization is success
*
*******************************************************************************/
bool ezmStcMem_InitMemList(ezmMemList *mem_list, uint8_t *buffer, uint16_t buffer_size)
{
    bool    is_success = true;
    MemHdr  *free_mem_header = NULL;

    STCMEMPRINT("ezmStcMem_InitMemList()");

    if (mem_list == NULL || buffer == NULL || buffer_size == 0)
    {
        is_success = false;
    }

    if (is_success)
    {
        mem_list->buffer = buffer;
        mem_list->buffer_size = buffer_size;

        mem_list->alloc_list.pstHead = NULL;
        mem_list->alloc_list.pstTail = NULL;
        mem_list->alloc_list.u16Size = 0U;

        free_mem_header = ezmStcMem_GetFreeHeader();
        
        if (NULL != free_mem_header)
        {
            mem_list->free_list.pstHead = NULL;
            mem_list->free_list.pstTail = NULL;
            mem_list->free_list.u16Size = 0;

            free_mem_header->pBuffer = buffer;
            free_mem_header->u16BufferSize = buffer_size;
            LinkedList_InsertToHead(&mem_list->free_list, free_mem_header);
        }
        else
        {
            is_success = false;
        }
    }
    return is_success;
}

/******************************************************************************
* Function : ezmStcMem_Malloc
*//**
* \b Description:
*
* This function allocate the number of bytes in the initialized memory
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:  handle to manage memory buffer
* @param    alloc_size: number of byte to be allocated
* @return   address of the allocated memory
*
*******************************************************************************/
void *ezmStcMem_Malloc(ezmMemList *mem_list, uint16_t alloc_size)
{
    void    *alloc_addr = NULL;
    bool    is_success = true;
    MemHdr  *reserve_header = NULL;

    STCMEMPRINT("ezmStcMem_Malloc()");

    if (NULL == mem_list || 0U == alloc_size)
    {
        is_success = false;
    }

    if (is_success)
    {
        reserve_header = ezmStcMem_ReserveMemoryBlock(&mem_list->free_list, alloc_size);
    }

    if (NULL != reserve_header)
    {
        is_success = is_success & ezmStcMem_MoveHeader(reserve_header, &mem_list->free_list, &mem_list->alloc_list);

        if (is_success)
        {
            alloc_addr = reserve_header->pBuffer;
        }
    }

    ezmStcMem_PrintFreeList(mem_list);
    ezmStcMem_PrintAllocList(mem_list);

    return alloc_addr;
}

/******************************************************************************
* Function : ezmStcMem_Free
*//**
* \b Description:
*
* This function frees the allocated memory in the memory buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:  handle to manage memory buffer
* @param    *alloc_addr: the address of the allocated memory
*
* @return   True if free is success
*
*******************************************************************************/
bool ezmStcMem_Free(ezmMemList *mem_list, void *alloc_addr)
{
    bool        is_success = true;
    LinkedList  *alloc_list = &mem_list->alloc_list;
    LinkedList  *free_list = &mem_list->free_list;
    MemHdr      *next_header = alloc_list->pstHead;

    STCMEMPRINT1("ezmStcMem_Free() - [address = %p]", alloc_addr);

    if (mem_list == NULL || alloc_addr == NULL)
    {
        is_success = false;
    }

    if (is_success)
    {
        while(next_header != NULL)
        {
            if (next_header->pBuffer == (uint8_t*)alloc_addr)
            {
                /* Do something */
                is_success = is_success & LinkedList_RemoveNode(alloc_list, next_header);
                ezmStcMem_ReturnHeaderToFreeList(free_list, next_header);
                ezmSmalloc_Merge(free_list);
                STCMEMPRINT("Free OK");
                break;
            }
            else
            {
                next_header = next_header->pstNextNode;
            }
        }
    }

    ezmStcMem_PrintFreeList(mem_list);
    ezmStcMem_PrintAllocList(mem_list);

    return is_success;
}

/******************************************************************************
* Function : ezmStcMem_HexdumpBuffer
*//**
* \b Description:
*
* This function prints the content of the memory buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:  handle to manage memory buffer
*
* @return   None
*
*******************************************************************************/
void ezmStcMem_HexdumpBuffer(ezmMemList *mem_list)
{
#if (VERBOSE == 1U)
    if (mem_list)
    {
        STCMEMHEXDUMP(mem_list->buffer, mem_list->buffer_size);
    }
#endif /* VERBOSE */
}

/******************************************************************************
* Function : ezmStcMem_PrintFreeList
*//**
* \b Description:
*
* This function prints the memory headers of the free list
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:  handle to manage memory buffer
*
* @return   None
*
*******************************************************************************/
void ezmStcMem_PrintFreeList(ezmMemList *mem_list)
{
#if (VERBOSE == 1U)
    MemHdr *next_header = mem_list->free_list.pstHead;

    STCMEMPRINT("*****************************************");
    while (next_header)
    {
        STCMEMPRINT1("[addr = %p]", next_header);
        STCMEMPRINT1("[next = %p]", next_header->pstNextNode);
        STCMEMPRINT1("[prev = %p]", next_header->pstPrevNode);
        STCMEMPRINT1("[buff = %p]", next_header->pBuffer);
        STCMEMPRINT1("[size = %d]", next_header->u16BufferSize);
        STCMEMPRINT1("[index = %d]", next_header->u16NodeIndex);
        STCMEMPRINT("<======>");

        next_header = next_header->pstNextNode;
    }
    STCMEMPRINT("*****************************************\n");
#endif
}

/******************************************************************************
* Function : ezmStcMem_PrintAllocList
*//**
* \b Description:
*
* This function prints the memory headers of the allocated list
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *mem_list:  handle to manage memory buffer
*
* @return   None
*
*******************************************************************************/
void ezmStcMem_PrintAllocList(ezmMemList * mem_list)
{
#if (VERBOSE == 1U)
    MemHdr *next_header = mem_list->alloc_list.pstHead;

    STCMEMPRINT("*****************************************");
    while (next_header)
    {
        STCMEMPRINT1("[addr = %p]", next_header);
        STCMEMPRINT1("[next = %p]", next_header->pstNextNode);
        STCMEMPRINT1("[prev = %p]", next_header->pstPrevNode);
        STCMEMPRINT1("[buff = %p]", next_header->pBuffer);
        STCMEMPRINT1("[size = %d]", next_header->u16BufferSize);
        STCMEMPRINT1("[index = %d]", next_header->u16NodeIndex);
        STCMEMPRINT("<======>");

        next_header = next_header->pstNextNode;
    }
    STCMEMPRINT("*****************************************\n");
#endif
}

MemHdr* ezmStcMem_ReserveMemoryBlock(LinkedList* free_list, uint16_t block_size_byte)
{
    bool    is_success = true;
    MemHdr* next_header = NULL;
    MemHdr* free_header = NULL;

    STCMEMPRINT("ezmStcMem_Malloc()");

    if (NULL == free_list || 0U == block_size_byte)
    {
        is_success = false;
    }

    if (is_success)
    {
        free_header = ezmStcMem_GetFreeHeader();

        if (free_header == NULL)
        {
            is_success = false;
        }
    }

    if (is_success)
    {
        next_header = free_list->pstHead;
        while (next_header->u16BufferSize < block_size_byte && next_header->pstNextNode != NULL)
        {
            next_header = next_header->pstNextNode;
        }

        if (next_header->u16BufferSize >= block_size_byte)
        {
            free_header->u16BufferSize = next_header->u16BufferSize - block_size_byte;

            /* wrap around point */
            if (free_header->u16BufferSize == 0)
            {
                ezmStcMem_ResetHeader(free_header->u16NodeIndex);
                STCMEMPRINT("buffer reach the end");
            }
            else
            {
                free_header->pBuffer = (uint8_t*)(next_header->pBuffer + block_size_byte);
                LinkedList_InsertToTail(free_list, free_header);
            }

            next_header->u16BufferSize = block_size_byte;
        }
    }
    return next_header;
}

bool  ezmStcMem_MoveHeader(MemHdr* header, LinkedList* from_list, LinkedList* to_list)
{
    bool is_success = true;

    if (NULL == header || NULL == from_list || NULL == to_list)
    {
        is_success = false;
    }

    if (is_success)
    {
        is_success = LinkedList_RemoveNode(from_list, header);
        LinkedList_InsertToTail(to_list, header);
    }

    return is_success;
}
/**************************** Private function *******************************/

/******************************************************************************
* Function : ezmStcMem_ResetHeader
*//**
* \b Description:
*
* This function resets the memory header to the un-init state
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    header_index:  handle to manage memory buffer
*
* @return   None
*
*******************************************************************************/
static void ezmStcMem_ResetHeader(uint16_t header_index)
{
    if (header_index < NUM_OF_MEMHDR)
    {
        header_pool[header_index].pBuffer = NULL;
        header_pool[header_index].pstNextNode = NULL;
        header_pool[header_index].pstPrevNode = NULL;
        header_pool[header_index].u16BufferSize = 0U;
        header_pool[header_index].u16NodeIndex = MEMHDR_INVALID_ID;
    }
}

/******************************************************************************
* Function : ezmStcMem_GetFreeHeader
*//**
* \b Description:
*
* This function return a free header from the memory pool
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
*
* @return   pointer to the free header or NULL if no header is free
*
*******************************************************************************/
static MemHdr *ezmStcMem_GetFreeHeader(void)
{
    STCMEMPRINT("ezmStcMem_GetFreeHeader()");
    MemHdr *free_header = NULL;

    for (uint16_t i = 0; i < NUM_OF_MEMHDR; i++)
    {
        if (header_pool[i].u16NodeIndex == MEMHDR_INVALID_ID)
        {
            /* store its own index for eaiser look up*/
            header_pool[i].u16NodeIndex = i;

            free_header = &header_pool[i];

            STCMEMPRINT1("Found free instance [inst = %d]", i);
            break;
        }
    }

    return free_header;
}

/******************************************************************************
* Function : ezmStcMem_ReturnHeaderToFreeList
*//**
* \b Description:
*
* This function returns the free memory header (and its buffer) to the free list
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *free_list      free list the the header will be returned
* @param    *free_header    the header to be moved
*
* @return   None
*
*******************************************************************************/
static void ezmStcMem_ReturnHeaderToFreeList(LinkedList *free_list, MemHdr *free_header)
{
    memset(free_header->pBuffer, 0, free_header->u16BufferSize);
    MemHdr *next_header = free_list->pstHead;

    if (NULL == next_header)
    {
        /* Free list is empty so we just add it the free header to the list and finish*/
        LinkedList_InsertToHead(free_list, free_header);
    }
    else
    {
        /* tranverse the list to add the header */
        while (NULL != next_header)
        {
            if ((uint8_t*)next_header->pBuffer < (uint8_t*)free_header->pBuffer)
            {
                LinkedList_InsertNewNodeAfterANode(free_list, next_header, free_header);
                break;
            }
            else if ((uint8_t*)next_header->pBuffer > (uint8_t*)free_header->pBuffer)
            {
                /* yeah swap, must be modifed later*/
                LinkedList_InsertNewNodeAfterANode(free_list, next_header, free_header);
                LinkedList_RemoveNode(free_list, next_header);
                LinkedList_InsertNewNodeAfterANode(free_list, free_header, next_header);
                break;
            }
            else
            {
                next_header = next_header->pstNextNode;
            }
        }
    }
}

/******************************************************************************
* Function : ezmSmalloc_Merge
*//**
* \b Description:
*
* This function tries to merge the adjacent free blocks into a bigger block
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *free_list      free list the the header will be returned
*
* @return   None
*
*******************************************************************************/
static void ezmSmalloc_Merge(LinkedList *free_list)
{
    MemHdr *head = free_list->pstHead;
    MemHdr *next = head->pstNextNode;

    while (NULL != next && ((uint8_t*)head->pBuffer + head->u16BufferSize) == next->pBuffer)
    {
        STCMEMPRINT("Next adjacent block is free");
        head->u16BufferSize += next->u16BufferSize;
        LinkedList_RemoveNode(free_list, next);
        ezmStcMem_ResetHeader(next->u16NodeIndex);
        next = head->pstNextNode; /* Advance to next node */
    }
}
#endif /* STCMEM */
/* End of file */