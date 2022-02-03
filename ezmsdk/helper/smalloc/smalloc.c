/*******************************************************************************
* Title                 :   my_math module 
* Filename              :   my_math.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*******************************************************************************/

/*************** SOURCE REVISION LOG *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Initial Release.
*  24.12.2021   1.0.1       Quang Hai Nguyen    Refactor and add code document
*
*******************************************************************************/
/** @file  smalloc.c
 *  @brief This is the source file for static memory allocation. It is basically,
 *          works as the normal malloc, but the memory is allocated from a static
 *          memory buffer
 */

/******************************************************************************
* Includes
******************************************************************************/
#include "../../app/app_config.h"

#if (SMALLOC == 1U)

#include "../linked_list/linked_list.h"
#include "smalloc.h"
#include "stdbool.h"
#include <string.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ENABLE_HEXDUMP              0U

#if STATIC_MEMORY_SIZE == 0U
#error memory size must be bigger than 0
#endif /* STATIC_MEMORY_SIZE */

#define MOD_NAME    "SMALLOC"

#if (MODULE_DEBUG == 1U) && (SMALLOC_DEBUG == 1U)
    #define SMALLOCPRINT(a)                 PRINT_DEBUG(MOD_NAME,a)
    #define SMALLOCPRINT1(a,b)              PRINT_DEBUG1(MOD_NAME,a,b)
    #define SMALLOCPRINT2(a,b,c)            PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define SMALLOCPRINT3(a,b,c,d)          PRINT_DEBUG2(MOD_NAME,a,b,c,d)
    #define SMALLOCPRINT4(a,b,c,d,e)        PRINT_DEBUG2(MOD_NAME,a,b,c,d,e)
    #define SMALLOCFUNCENTER()              SMALLOCPRINT("<--")
    #define SMALLOCFUNCLEAVE()              SMALLOCPRINT("-->")
#else 
    #define SMALLOCPRINT1(a)           
    #define SMALLOCPRINT2(a,b)           
    #define SMALLOCPRINT3(a,b,c)
    #define SMALLOCFUNCENTER()
    #define SMALLOCFUNCLEAVE()
#endif

#if (ENABLE_HEXDUMP == 1U)
    #if (MODULE_DEBUG == 1U) && (SMALLOC_DEBUG == 1U)
        #define SMALLOCHEXDUMP(a,b,c)           ezmHexdump(a,b,c)
    #else 
        #define SMALLOCHEXDUMP(a,b,c)
    #endif
#else
    #define SMALLOCHEXDUMP(a,b,c)
#endif

#if (MODULE_DEBUG == 1U) && (SMALLOC_DEBUG == 1U)
    #define SMALLOC_PRINT_LISTMETA(x)       ezmSmalloc_PrintListMetadata(x)
    #define SMALLOC_PRINT_BLOCKMETA(x)      ezmSmalloc_PrintBlockMetadata(x)
    #define SMALLOC_PRINT_LIST(x)           ezmSmalloc_PrintList(x)
#else
    #define SMALLOC_PRINT_LISTMETA(x)
    #define SMALLOC_PRINT_BLOCKMETA(x)
    #define SMALLOC_PRINT_LIST(x)
#endif

#if (ENABLE_STATS == 1U)
    #define PRINTSTATS()    ezmSmalloc_PrintStats()
#else
    #define PRINTSTATS()
#endif

#define SIZE_OF_METADATA        sizeof(ezmMemoryBlock)                          /**< size of the metadata */
#define END_OF_MEMORY           ((void*)au8StaticMemory + STATIC_MEMORY_SIZE)   /**< address of the end of the memory buffer*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

#if (ENABLE_STATS == 1U)
typedef struct
{
    uint32_t u32NumOfAlloc;
    uint32_t u32NumOfFree;
    uint32_t u32NumOfAllocBlock;
    uint32_t u32NumOfFreeByte;
}emzSmalloc_Statistic;

static emzSmalloc_Statistic stStats = {0, 0, 0, STATIC_MEM_SIZE};
#endif

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t      au8StaticMemory[STATIC_MEMORY_SIZE];    /**< array for statically allocate memory */
static LinkedList   stFreeBlockList;                        /**< list to manage the free block */
static LinkedList   stAllocBlockList;                       /**< list to manage the allocated block */
/******************************************************************************
* Function Definitions
*******************************************************************************/
static void     ezmSmalloc_Merge                    (void);
static bool     ezmSmalloc_SplitBlock               (ezmMemoryBlock * SplitBlock, uint16_t u16SplitedSize);
static void *   ezmSmalloc_GetFreeBlockApendToList  (uint16_t u16BlockSize, LinkedList * pstApendList);
static void     ezmSmalloc_ReturnBlockToFreeList    (ezmMemoryBlock * pstBlock);

#if (SMALLOC_DEBUG == 1U)
void ezmSmalloc_PrintListMetadata   (LinkedList * pstList);
void ezmSmalloc_PrintBlockMetadata  (ezmMemoryBlock * pstMemBlock);
void ezmSmalloc_PrintList           (LinkedList * pstList);
#endif

#if (ENABLE_STATS == 1U)
void ezmSmalloc_PrintStats(void);
#endif

/**************************** Public function ********************************/

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//** 
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @return   None
*
*******************************************************************************/
void ezmSmalloc_Initialize(void)
{
    SMALLOCFUNCENTER();

    /*Setup free list*/
    stFreeBlockList.u16Size = 0U;
    stFreeBlockList.pstHead = NULL;
    stFreeBlockList.pstTail = NULL;

    /*Setup allocated list*/
    stAllocBlockList.pstHead = NULL;
    stAllocBlockList.pstTail = NULL;
    stAllocBlockList.u16Size = 0U;

    /* point the free list to available buffer */
    ezmMemoryBlock * pstFirstFreeBlock;
    pstFirstFreeBlock = (ezmMemoryBlock *)au8StaticMemory;
    pstFirstFreeBlock->u16BufferSize = STATIC_MEMORY_SIZE - SIZE_OF_METADATA;
    pstFirstFreeBlock->pBuffer = (uint8_t *)pstFirstFreeBlock + SIZE_OF_METADATA;
    LinkedList_InsertToHead(&stFreeBlockList, pstFirstFreeBlock);

    SMALLOC_PRINT_LISTMETA(&stFreeBlockList);
    SMALLOC_PRINT_LISTMETA(&stAllocBlockList);
    SMALLOC_PRINT_LIST(&stFreeBlockList);

    PRINTSTATS();
    SMALLOCFUNCLEAVE();
}

/******************************************************************************
* Function : ezmSmalloc_Malloc
*//**
* \b Description:
*
* This function allocates a block of memory
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first
*
* POST-CONDITION: None
*
* @param    u16Size: (IN)number of byte to be allocated
* @return   address of the memory block
*
*******************************************************************************/
void* ezmSmalloc_Malloc(uint16_t u16Size)
{
    ezmMemoryBlock * pstFreeBlock;
    void * pFreeBlockAddress = NULL;

    SMALLOCFUNCENTER(); 
    SMALLOCPRINT1("allocate: %u bytes", u16Size);

    if(!stFreeBlockList.pstHead)
    {
        ezmSmalloc_Initialize();
    }

    pstFreeBlock = ezmSmalloc_GetFreeBlockApendToList(u16Size, &stAllocBlockList);

    if(pstFreeBlock != NULL)
    {
        pFreeBlockAddress = (void *)pstFreeBlock->pBuffer;
    }

    SMALLOCFUNCLEAVE();
    return pFreeBlockAddress;
}

/******************************************************************************
* Function : emzSmalloc_Free
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first
*
* POST-CONDITION: None
*
* @param    *address: (IN)pointer to the deallocated block
*
* @return   None
*
*******************************************************************************/
void emzSmalloc_Free(void* address)
{
    ezmMemoryBlock* pstDeallocBlock;
    bool bSuccess = false;

    SMALLOCFUNCENTER();

    /* Get the address of the soon to be freed block*/
    pstDeallocBlock = (ezmMemoryBlock*)((uint8_t*)address - SIZE_OF_METADATA);
    SMALLOC_PRINT_BLOCKMETA(pstDeallocBlock);

    /* remove it from the allocated list*/
    bSuccess = LinkedList_RemoveNode(&stAllocBlockList, pstDeallocBlock);

    if (bSuccess)
    {
        ezmSmalloc_ReturnBlockToFreeList(pstDeallocBlock);
    }

    SMALLOCFUNCLEAVE();
}

/******************************************************************************
* Function : ezmSmalloc_InitMemList
*//**
* \b Description:
*
* This function initializes a new memory list to mange the memory block
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first
*
* POST-CONDITION: None
*
* @param    *pstNewList:        (IN)pointer to the new list
* @param    u8ListOwnerModuleId:(IN)id of the module who owns this list
*
* @return   None
*
*******************************************************************************/
void ezmSmalloc_InitMemList(ezmMemList* pstNewList, uint8_t u8ListOwnerModuleId)
{
    SMALLOCFUNCENTER();

    if (!stFreeBlockList.pstHead)
    {
        ezmSmalloc_Initialize();
    }
    SMALLOCPRINT1("Init a list for module: 0x%02x", u8ListOwnerModuleId);
    pstNewList->pstHead = NULL;
    pstNewList->pstTail = NULL;
    pstNewList->u16Size = 0;
    pstNewList->u8ModuleId = u8ListOwnerModuleId;
    SMALLOC_PRINT_LISTMETA(pstNewList);

    SMALLOCFUNCLEAVE();
}

/******************************************************************************
* Function : ezmSmalloc_GetMemBlockInList
*//**
* \b Description:
*
* This function get a free block and puts into a memory list - Legacy, keep for
* compatiple only
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first and memory list
* is initialized
*
* POST-CONDITION: None
*
* @param    *pstNewList:    (IN)list where new block will be appended
* @param    u16BlockSize:   (IN)size of the block
*
* @return   pointer to the new block
*
*******************************************************************************/
ezmMemoryBlock* ezmSmalloc_GetMemBlockInList(ezmMemList* pstNewList, uint16_t u16BlockSize)
{
    ezmMemoryBlock* pstFreeMemBlock;

    SMALLOCFUNCENTER();

    pstFreeMemBlock = (ezmMemoryBlock*)ezmSmalloc_GetFreeBlockApendToList(u16BlockSize, pstNewList);

    SMALLOC_PRINT_LISTMETA(pstNewList);
    SMALLOC_PRINT_BLOCKMETA((ezmMemoryBlock*)pstFreeMemBlock);
#if (ENABLE_STATS == 1U)
    stStats.u32NumOfAllocBlock++;
    PRINTSTATS();
#endif
    SMALLOCFUNCLEAVE();
    return pstFreeMemBlock;
}

/******************************************************************************
* Function : ezmSmalloc_ReturnMemBlock
*//**
* \b Description:
*
* This function returns a block to the free list
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first and memory list
* is initialized
*
* POST-CONDITION: None
*
* @param    *pstNewList:    (IN)list owns the block
* @param    *pstBlock:      (IN)point to the deallocated block
*
* @return   None
*
*******************************************************************************/
void ezmSmalloc_ReturnMemBlock(ezmMemList* pstNewList, ezmMemoryBlock* pstBlock)
{
    ezmMemoryBlock* pstReturnedBlock = (ezmMemoryBlock*)pstBlock;

    SMALLOCFUNCENTER();

    if (LinkedList_RemoveNode(pstNewList, pstReturnedBlock) == true)
    {
        ezmSmalloc_ReturnBlockToFreeList(pstReturnedBlock);
        SMALLOCPRINT1("Return %u byte to the free list", (uint32_t)(SIZE_OF_METADATA + pstReturnedBlock->u16BufferSize));
#if (ENABLE_STATS == 1U)
        stStats.u32NumOfAllocBlock--;
        PRINTSTATS();
#endif
    }
    SMALLOCFUNCLEAVE();
}

/******************************************************************************
* Function : ezmSmalloc_GetFreeBlock
*//**
* \b Description:
*
* This function get a free block from the free list
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called
*
* POST-CONDITION: None
*
* @param    u16BlockSize: (IN)size of the block
*
* @return   address of the block, NULL if cannot allocate memory
*
*******************************************************************************/
ezmMemoryBlock* ezmSmalloc_GetFreeBlock(uint16_t u16BlockSize)
{
    ezmMemoryBlock* pstFreeBlock = stFreeBlockList.pstHead;

    /*Init if not*/
    if (!pstFreeBlock)
    {
        ezmSmalloc_Initialize();
    }

    if (pstFreeBlock)
    {
        /* Searching for suitable block */
        while ((pstFreeBlock->u16BufferSize < u16BlockSize + SIZE_OF_METADATA) &&
            (pstFreeBlock->pstNextNode != NULL))
        {
            /**Current block does not have enough space
             * move to the next block if applied
             * and increase block index
             */
            pstFreeBlock = pstFreeBlock->pstNextNode;
        }

        /* Found suitable block*/
        if (pstFreeBlock->u16BufferSize >= u16BlockSize + SIZE_OF_METADATA)
        {
            if (ezmSmalloc_SplitBlock(pstFreeBlock, u16BlockSize) == true)
            {
                if (LinkedList_RemoveNode(&stFreeBlockList, pstFreeBlock) != true)
                {
                    /*something wrong, we cannot move the block so we merge it back to the free list*/
                    ezmSmalloc_Merge();
                    pstFreeBlock = NULL;
                    SMALLOCPRINT("LinkedList_RemoveNode error");
                }
            }
            else
            {
                /*unalble to split, return NULL*/
                pstFreeBlock = NULL;
            }
        }
        /* No block is available */
        else
        {
            SMALLOCPRINT("Found no block");

            /* Try to merge blocks for the next call*/
            ezmSmalloc_Merge();

            pstFreeBlock = NULL;
        }
    }
    return (void*)pstFreeBlock;
}

/******************************************************************************
* Function : ezmSmalloc_ApendBlockToList
*//**
* \b Description:
*
* This function appends the block to a list
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called first and memory list
* is initialized
*
* POST-CONDITION: None
*
* @param    *pstBlock:  (IN)appended block
* @param    *pstNewList:(IN)memory list
* @return   None
*
*
*******************************************************************************/
void ezmSmalloc_ApendBlockToList(ezmMemoryBlock* pstBlock, ezmMemList* pstNewList)
{
    LinkedList_InsertToTail(pstNewList, pstBlock);
}

/*************************** Private function ********************************/

/******************************************************************************
* Function : ezmSmalloc_SplitBlock
*//**
* \b Description:
*
* This function splits the block into 2 smaller blocks
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called
*
* POST-CONDITION: None
*
* @param    *SplitBlock:    (IN)block to be splited
* @param    u16SplitedSize: (IN)size will be splited
*
* @return   True if block is splited, else false
*
*******************************************************************************/
static bool ezmSmalloc_SplitBlock(ezmMemoryBlock * SplitBlock, uint16_t u16SplitedSize)
{
    bool bSuccess = false;

    /* Pointer to the new block after being splitted */
    ezmMemoryBlock * pstNewBlock = (ezmMemoryBlock *)((uint8_t *)SplitBlock + SIZE_OF_METADATA + u16SplitedSize);

    SMALLOCFUNCENTER();

    /**Doing the split
     * it is just insert the new splitted block to the current block,
     * and adjust the sizes of the splitted block and the new block
     */
    if(LinkedList_InsertNewNodeAfterANode(&stFreeBlockList, SplitBlock, pstNewBlock) == true)
    {
        pstNewBlock->u16BufferSize = SplitBlock->u16BufferSize - SIZE_OF_METADATA - u16SplitedSize;
        pstNewBlock->pBuffer = (uint8_t *)pstNewBlock + SIZE_OF_METADATA;

        SplitBlock->u16BufferSize = u16SplitedSize;     
        SplitBlock->pBuffer = (uint8_t *)SplitBlock + SIZE_OF_METADATA;   

        bSuccess = true;
    }

    SMALLOCFUNCLEAVE();

    return bSuccess;
}


/******************************************************************************
* Function : ezmSmalloc_ReturnBlockToFreeList
*//**
* \b Description:
*
* This function returns the block to a free list
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called
*
* POST-CONDITION: None
*
* @param    *pstBlock: (IN)pointer to deallocated block
*
* @return   None
*
*******************************************************************************/
static void ezmSmalloc_ReturnBlockToFreeList(ezmMemoryBlock * pstBlock)
{
    SMALLOCFUNCENTER();   
    memset(pstBlock->pBuffer, 0, pstBlock->u16BufferSize);
    /** insert the block to a suitable location
     * We tried to sort the block in an order so that merge operation will be easier
     */
    if((void *)pstBlock  < (void *)stFreeBlockList.pstHead)
    {
        LinkedList_InsertToHead(&stFreeBlockList, pstBlock);
    }
    else if((void *)pstBlock  > (void *)stFreeBlockList.pstTail)
    {
        LinkedList_InsertToTail(&stFreeBlockList, pstBlock);
    }
    else
    {
        Node * pstHeadFreeBlock = stFreeBlockList.pstHead;
        
        while(((void *)pstBlock < (void *)pstHeadFreeBlock) && (pstHeadFreeBlock->pstNextNode != NULL))
        {
            pstHeadFreeBlock = pstHeadFreeBlock->pstNextNode;
        }
        
        LinkedList_InsertNewNodeAfterANode(&stFreeBlockList, pstHeadFreeBlock, pstBlock);
    }
    
#if (ENABLE_STATS == 1U)
    stStats.u32NumOfFreeByte += SIZE_OF_METADATA + pstBlock->u16BufferSize;
    stStats.u32NumOfFree++;
    PRINTSTATS();
#endif

    SMALLOC_PRINT_LISTMETA(&stFreeBlockList);

    ezmSmalloc_Merge();   
    SMALLOCFUNCLEAVE();
}

/******************************************************************************
* Function : ezmSmalloc_GetFreeBlockApendToList
*//**
* \b Description:
*
* This function gets a free block and append it into a list
*
* PRE-CONDITION: ezmSmalloc_Initialize() must be called
*
* POST-CONDITION: None
*
* @param    u16BlockSize:   (IN)size of the free block
* @param    *pstApendList:  (IN)list that block will be appended
* @return   None
*
*
*******************************************************************************/
static void * ezmSmalloc_GetFreeBlockApendToList(uint16_t u16BlockSize, LinkedList * pstApendList)
{
    ezmMemoryBlock * pstFreeBlock = stFreeBlockList.pstHead;

    SMALLOCFUNCENTER();

    if (pstFreeBlock)
    {
        /* Searching for suitable block */
        while ((pstFreeBlock->u16BufferSize < u16BlockSize + SIZE_OF_METADATA) &&
            (pstFreeBlock->pstNextNode != NULL))
        {
            /**Current block does not have enough space
             * move to the next block if applied
             * and increase block index
             */
            pstFreeBlock = pstFreeBlock->pstNextNode;
        }

        /* Found suitable block*/
        if (pstFreeBlock->u16BufferSize >= u16BlockSize + SIZE_OF_METADATA)
        {
            /* Split the block, and move it to allocated list */
            SMALLOCPRINT1("Suitable block @: %p", (void*)pstFreeBlock);
            SMALLOC_PRINT_BLOCKMETA(pstFreeBlock);

            if (ezmSmalloc_SplitBlock(pstFreeBlock, u16BlockSize) == true)
            {
                SMALLOC_PRINT_BLOCKMETA(pstFreeBlock);

                if (LinkedList_RemoveNode(&stFreeBlockList, pstFreeBlock) == true)
                {
                    LinkedList_InsertToTail(pstApendList, pstFreeBlock);

#if (ENABLE_STATS == 1U)
                    stStats.u32NumOfFreeByte -= SIZE_OF_METADATA + u16BlockSize;
                    stStats.u32NumOfAlloc++;
                    PRINTSTATS();
#endif
                }
                else
                {
                    /*something wrong, we cannot move the block so we merge it back to the free list*/
                    ezmSmalloc_Merge();
                    pstFreeBlock = NULL;
                    SMALLOCPRINT("LinkedList_RemoveNode error");
                }
            }
            else
            {
                SMALLOCPRINT("unable to split block");
                pstFreeBlock = NULL;
            }

        }
        /* No block is available */
        else
        {
            SMALLOCPRINT("Found no block");

            /* Try to merge blocks for the next call*/
            ezmSmalloc_Merge();

            pstFreeBlock = NULL;
        }
    }

    SMALLOC_PRINT_LISTMETA(&stFreeBlockList);
    SMALLOC_PRINT_LIST(&stFreeBlockList);
    SMALLOCFUNCLEAVE();
    
    return (void *)pstFreeBlock;
}

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
static void ezmSmalloc_Merge(void)
{
    ezmMemoryBlock * pstCurrentFreeBlock = stFreeBlockList.pstHead;
    ezmMemoryBlock * pstNextFreeBlock = pstCurrentFreeBlock->pstNextNode;
    SMALLOCFUNCENTER();

    /* Check if the next free block is adjacent to merge it*/
    while((ezmMemoryBlock*)((uint8_t*)pstCurrentFreeBlock + SIZE_OF_METADATA + pstCurrentFreeBlock->u16BufferSize) == pstNextFreeBlock && 
        pstNextFreeBlock != NULL)
    {
        SMALLOCPRINT("Next adjacent block is free");
        pstCurrentFreeBlock->u16BufferSize += pstNextFreeBlock->u16BufferSize + SIZE_OF_METADATA;

        LinkedList_RemoveNode(&stFreeBlockList, pstNextFreeBlock);

        memset((void *)pstNextFreeBlock, 0, pstNextFreeBlock->u16BufferSize + SIZE_OF_METADATA); /* clean the merged block */
        
        pstNextFreeBlock = pstCurrentFreeBlock->pstNextNode; /* Advance to next node */
    }

    SMALLOC_PRINT_LISTMETA(&stFreeBlockList);
    SMALLOC_PRINT_LIST(&stFreeBlockList);
    SMALLOCFUNCLEAVE();
}

#if (SMALLOC_DEBUG == 1)

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void ezmSmalloc_PrintListMetadata(LinkedList * pstList)
{
    PRINT1("*List metadata*****************************************************");
    PRINT2("address of list:  %p", (void *)pstList);
    PRINT2("address of head:  %p", (void *)pstList->pstHead);
    PRINT2("address of tail:  %p", (void *)pstList->pstTail);
    PRINT2("number of block:  %u", pstList->u16Size);
    PRINT1("*List metadata end*************************************************");
}

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void ezmSmalloc_PrintList(LinkedList * pstList)
{
    ezmMemoryBlock * pstBlock;
    pstBlock = pstList->pstHead;
    PRINT1("*Block detail******************************************************");     
    do
    {
        ezmSmalloc_PrintBlockMetadata(pstBlock);
        pstBlock = pstBlock->pstNextNode;
    }
    while(pstBlock != NULL);
    PRINT1("*Block detail end**************************************************");  
}

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void ezmSmalloc_PrintBlockMetadata(ezmMemoryBlock * pstMemBlock)
{
    PRINT1("*Block metadata****************************************************");
    PRINT2("address: %p", (void *)pstMemBlock);
    PRINT2("size: %u", pstMemBlock->u16BufferSize);
    PRINT2("buffer address: %p", (void *)pstMemBlock->pBuffer);
    PRINT2("next block: %p", (void *)pstMemBlock->pstNextNode);
    PRINT2("previous block: %p", (void *)pstMemBlock->pstPrevNode);
    SMALLOCHEXDUMP((void *)pstMemBlock->pBuffer, pstMemBlock->u16BufferSize, true);
    PRINT1("*Block metadata end************************************************");
}

#if (ENABLE_STATS == 1U)

/******************************************************************************
* Function : ezmSmalloc_Initialize
*//**
* \b Description:
*
* This function initializes the the static memory allocation module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void ezmSmalloc_PrintStats(void)
{
    PRINT1("*Block statistic***************************************************");
    PRINT2("number of alloc called %u", stStats.u32NumOfAlloc);
    PRINT2("number of free called %u", stStats.u32NumOfFree);
    if(stStats.u32NumOfFree > stStats.u32NumOfAlloc)
    {
        PRINT2("delta between alloc and free called: %u", (uint32_t)(stStats.u32NumOfFree - stStats.u32NumOfAlloc));
    }
    else
    {
        PRINT2("delta between alloc and free called %u", (uint32_t)(stStats.u32NumOfAlloc - stStats.u32NumOfFree));
    }
    PRINT2("number of alloc blocks %u", stStats.u32NumOfAllocBlock);
    PRINT2("number of free bytes %u", stStats.u32NumOfFreeByte);
    PRINT1("*Block statistic end***********************************************");
}
#endif
#endif /* SMALLOC_ANALYSIS */

#endif /* SMALLOC */
/* End of file */