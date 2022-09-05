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
#include "app/app_config.h"

#if (CONFIG_STCMEM == 1U)
#include "ezmDebug/ezmDebug.h"
#include "utilities/linked_list/linked_list.h"
#include "utilities/hexdump/hexdump.h"
#include "stcmem.h"
#include "stdbool.h"
#include <string.h>

/******************************************************************************
 Module Preprocessor Macros
*******************************************************************************/
#define VERBOSE         0U

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

#ifndef CONFIG_NUM_OF_MEM_BLOCK
#define CONFIG_NUM_OF_MEM_BLOCK 128
#endif /*CONFIG_NUM_OF_MEM_BLOCK*/

#define INIT_BLOCK(block, buff_ptr, size) {ezmLL_InitNode(&block->node);block->buff = buff_ptr;block->buff_size = size; }
#define GET_LIST(x) ((struct MemList*)x)
#define GET_BLOCK(node_ptr) (EZMLL_GET_PARENT_OF(node_ptr, node, struct MemBlock))

/******************************************************************************
 Module Typedefs
*******************************************************************************/

/******************************************************************************
 Module Variable Definitions
*******************************************************************************/
static struct MemBlock block_pool[CONFIG_NUM_OF_MEM_BLOCK] = { 0U };

/******************************************************************************
 Function Definitions
*******************************************************************************/
static void     ezmStcMem_ReturnHeaderToFreeList(struct Node *free_list_head, struct Node *free_node);
static void     ezmSmalloc_Merge                (struct Node *free_list_head);

struct Node*      ezmStcMem_ReserveMemoryBlock(struct Node* free_list_head, uint16_t block_size_byte);
bool              ezmStcMem_MoveBlock(struct Node* move_node, struct Node* from_list_head, struct Node* to_list_head);
struct MemBlock*  GetFreeBlock(void);
void              ReleaseBlock(struct MemBlock* block);
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
    /* Do nothing here ???*/
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
bool ezmStcMem_InitMemList(ezmMemList* mem_list, void* buff, uint16_t buff_size)
{
    bool    is_success = true;
    struct MemBlock *free_block = NULL;

    STCMEMPRINT("ezmStcMem_InitMemList()");
    STCMEMPRINT1("size = %d", sizeof(struct MemList));
    if (mem_list == NULL || buff == NULL || buff_size == 0)
    {
        is_success = false;
    }

    if (is_success)
    {
        GET_LIST(mem_list)->buff = buff;
        GET_LIST(mem_list)->buff_size = buff_size;
        ezmLL_InitNode(&GET_LIST(mem_list)->alloc_list_head);
        ezmLL_InitNode(&GET_LIST(mem_list)->free_list_head);

        free_block = GetFreeBlock();
        
        if (NULL != free_block)
        {
            INIT_BLOCK(free_block, buff, buff_size);

            is_success = is_success && EZMLL_ADD_HEAD(&GET_LIST(mem_list)->free_list_head, &free_block->node);
        }
        else
        {
            is_success = false;
        }
    }

    ezmStcMem_PrintAllocList(mem_list);
    ezmStcMem_PrintFreeList(mem_list);
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
    struct Node  *reserved_node = NULL;

    STCMEMPRINT("ezmStcMem_Malloc()");

    if (NULL == mem_list || 0U == alloc_size)
    {
        is_success = false;
    }

    if (is_success)
    {
        reserved_node = ezmStcMem_ReserveMemoryBlock(&GET_LIST(mem_list)->free_list_head, alloc_size);
    }

    if (NULL != reserved_node)
    {
        is_success = is_success && ezmStcMem_MoveBlock(reserved_node, &GET_LIST(mem_list)->free_list_head, &GET_LIST(mem_list)->alloc_list_head);

        if (is_success)
        {
            alloc_addr = GET_BLOCK(reserved_node)->buff;
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
    bool        is_success = false;
    struct Node* it_node = NULL;

    STCMEMPRINT1("ezmStcMem_Free() - [address = %p]", alloc_addr);

    if (mem_list != NULL && alloc_addr != NULL)
    {
        EZMLL_FOR_EACH(it_node, &GET_LIST(mem_list)->alloc_list_head)
        {
            if (GET_BLOCK(it_node)->buff == (uint8_t*)alloc_addr)
            {
                EZMLL_UNLINK_NODE(it_node);
                ezmStcMem_ReturnHeaderToFreeList(&GET_LIST(mem_list)->free_list_head, it_node);
                ezmSmalloc_Merge(&GET_LIST(mem_list)->free_list_head);
                is_success = true;
                STCMEMPRINT("Free OK");
                break;
            }
        }
    }

    ezmStcMem_PrintFreeList(mem_list);
    ezmStcMem_PrintAllocList(mem_list);

    return is_success;
}


/******************************************************************************
* Function : ezmStcMem_IsMemListReady
*//**
* \b Description: Return the status if the mem list is ready 
*
* This function frees the allocated memory in the memory buffer
*
* @param    *mem_list:  handle to manage memory buffer
*
* @return   True if ready, else false
*
*******************************************************************************/
bool ezmStcMem_IsMemListReady(ezmMemList *mem_list)
{
    bool is_ready = false;

    if (mem_list->buff != NULL && mem_list->buff_size > 0)
    {
        is_ready = true;
    }

    return is_ready;
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
        struct MemList* list = GET_LIST(mem_list);
        STCMEMPRINT("mem list info");
        STCMEMPRINT1("[addr = %p]", list);
        STCMEMPRINT1("[alloc = %p]", (void*)&list->alloc_list_head);
        STCMEMPRINT1("[free = %p]", (void*)&list->free_list_head);
        STCMEMPRINT1("[buff = %p]", list->buff);
        STCMEMPRINT1("[size = %d]", list->buff_size);
        STCMEMHEXDUMP(list->buff, list->buff_size);
    }
#endif /* VERBOSE */
    (void)mem_list;
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
    struct Node* it_node = NULL;

    STCMEMPRINT("*****************************************");
    STCMEMPRINT("free list");
    EZMLL_FOR_EACH(it_node, &GET_LIST(mem_list)->free_list_head)
    {
        STCMEMPRINT1("[addr = %p]", it_node);
        STCMEMPRINT1("[next = %p]", it_node->next);
        STCMEMPRINT1("[prev = %p]", it_node->prev);
        STCMEMPRINT1("[buff = %p]", GET_BLOCK(it_node)->buff);
        STCMEMPRINT1("[size = %d]", GET_BLOCK(it_node)->buff_size);
        STCMEMPRINT("<======>");
    }
    STCMEMPRINT("*****************************************\n");
#endif
    (void)mem_list;
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
    struct Node* it_node = NULL;

    STCMEMPRINT("*****************************************");
    STCMEMPRINT("allocated list");
    EZMLL_FOR_EACH(it_node, &GET_LIST(mem_list)->alloc_list_head)
    {
        STCMEMPRINT1("[addr = %p]", it_node);
        STCMEMPRINT1("[next = %p]", it_node->next);
        STCMEMPRINT1("[prev = %p]", it_node->prev);
        STCMEMPRINT1("[buff = %p]", GET_BLOCK(it_node)->buff);
        STCMEMPRINT1("[size = %d]", GET_BLOCK(it_node)->buff_size);
        STCMEMPRINT("<======>");
    }
    STCMEMPRINT("*****************************************\n");
#endif
    (void)mem_list;
}


uint16_t ezmStcMem_GetNumOfAllocBlock(ezmMemList* mem_list)
{
    return ezmLL_GetListSize(&GET_LIST(mem_list)->alloc_list_head);
}

uint16_t ezmStcMem_GetNumOfFreeBlock(ezmMemList* mem_list)
{
    return ezmLL_GetListSize(&GET_LIST(mem_list)->free_list_head);
}
/**************************** Private function *******************************/

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
static void ezmStcMem_ReturnHeaderToFreeList(struct Node* free_list_head, struct Node* free_node)
{
    memset(GET_BLOCK(free_node)->buff, 0, GET_BLOCK(free_node)->buff_size);
    struct Node* it_node = NULL;

    if (free_list_head != NULL && free_node != NULL)
    {
        if (IS_LIST_EMPTY(free_list_head))
        {
            EZMLL_ADD_HEAD(free_list_head, free_node);
        }
        else
        {
            /* tranverse the list to add the node, we aort the address in the order so merge operation will be easier*/
            EZMLL_FOR_EACH(it_node, free_list_head)
            {
                if (GET_BLOCK(free_node)->buff < GET_BLOCK(it_node)->buff)
                {
                    ezmLL_AppendNode(free_node, it_node->prev);
                    break;
                }
                else if (GET_BLOCK(free_node)->buff > GET_BLOCK(it_node)->buff)
                {
                    ezmLL_AppendNode(free_node, it_node);
                    break;
                }
                else
                {
                    /* do nothing, advance pointer */
                }
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
static void ezmSmalloc_Merge(struct Node* free_list_head)
{
    //MemHdr* head = free_list->pstHead;
    //MemHdr* next = head->pstNextNode;

    struct Node* it_node = free_list_head->next;
    struct Node* it_next = it_node->next;
    while (it_next != free_list_head &&
        ((uint8_t*)GET_BLOCK(it_node)->buff + GET_BLOCK(it_node)->buff_size) == (uint8_t*)GET_BLOCK(it_next)->buff)
    {

        STCMEMPRINT("Next adjacent block is free");
        GET_BLOCK(it_node)->buff_size += GET_BLOCK(it_next)->buff_size;
        EZMLL_UNLINK_NODE(it_next);
        ReleaseBlock(GET_BLOCK(it_next));
        it_next = it_node->next;
    }
}

struct MemBlock* GetFreeBlock(void)
{
    struct MemBlock* free_block = NULL;
    for (uint16_t i = 0; i < CONFIG_NUM_OF_MEM_BLOCK; i++)
    {
        if (block_pool[i].buff == NULL)
        {
            free_block = &block_pool[i];
            ezmLL_InitNode(&free_block->node);
            break;
        }
    }
    return free_block;
}

void ReleaseBlock(struct MemBlock* block)
{
    INIT_BLOCK(block, NULL, 0U);
}

struct Node* ezmStcMem_ReserveMemoryBlock(struct Node* free_list_head, uint16_t block_size_byte)
{
    struct MemBlock* remain_block = NULL;
    struct Node* iterate_Node = NULL;
    bool success = false;

    STCMEMPRINT("ezmStcMem_ReserveMemoryBlock()");

    if (NULL != free_list_head &&  block_size_byte > 0)
    {
        EZMLL_FOR_EACH(iterate_Node, free_list_head)
        {
            if (GET_BLOCK(iterate_Node)->buff_size >= block_size_byte)
            {
                if (GET_BLOCK(iterate_Node)->buff_size > block_size_byte)
                {
                    remain_block = GetFreeBlock();
                }

                if (remain_block)
                {
                    remain_block->buff_size = GET_BLOCK(iterate_Node)->buff_size - block_size_byte;
                    remain_block->buff = (uint8_t*)GET_BLOCK(iterate_Node)->buff + block_size_byte;
                    EZMLL_ADD_TAIL(free_list_head, &remain_block->node);
                }

                GET_BLOCK(iterate_Node)->buff_size = block_size_byte;

                success = true;
                break;
            }
        }
    }

    if (!success)
    {
        iterate_Node = NULL;
    }

    return iterate_Node;
}

bool ezmStcMem_MoveBlock(struct Node* move_node, struct Node* from_list_head, struct Node* to_list_head)
{
    bool is_success = true;

    if (NULL != move_node 
        && NULL != from_list_head 
        && NULL != to_list_head
        && ezmLL_IsNodeInList(from_list_head, move_node))
    {
        EZMLL_UNLINK_NODE(move_node);
        EZMLL_ADD_TAIL(to_list_head, move_node);
    }

    return is_success;
}
#endif /* CONFIG_STCMEM */
/* End of file */
