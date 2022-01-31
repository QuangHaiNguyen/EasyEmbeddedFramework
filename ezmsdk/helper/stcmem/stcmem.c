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

#if (STCMEM == 1U)

#include "../linked_list/linked_list.h"
#include "../hexdump/hexdump.h"
#include "stcmem.h"
#include "stdbool.h"
#include <string.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define VERBOSE         1U

#if STATIC_MEMORY_SIZE == 0U
#error memory size must be bigger than 0
#endif /* STATIC_MEMORY_SIZE */

#if (MODULE_DEBUG == 1U) && (STCMEM_DEBUG == 1U)
    #define STCMEMPRINT1(a)             PRINT_DEBUG1(a)
    #define STCMEMPRINT2(a,b)           PRINT_DEBUG2(a,b)
    #define STCMEMPRINT3(a,b,c)         PRINT_DEBUG3(a,b,c)
    #define STCMEMHEXDUMP(a,b)        ezmHexdump(a,b)
#else 
    #define STCMEMPRINT1(a)
    #define STCMEMPRINT2(a,b)
    #define STCMEMPRINT3(a,b,c)
    #define STCMEMHEXDUMP(a,b,c)
#endif


/******************************************************************************
* Module Typedefs
*******************************************************************************/


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static MemHdr   astMemHeader[NUM_OF_MEMHDR] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void     ezmStcMem_ResetHeader           (uint16_t HeaderId);
static MemHdr*  ezmStcMem_GetFreeHeader         (void);
static void     ezmStcMem_ReturnHeaderToFreeList(LinkedList* pstFreeList, MemHdr* pstHeader);
static void     ezmSmalloc_Merge                (LinkedList* pstFreeList);

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
void ezmStcMem_Initialization(void)
{
    STCMEMPRINT1("ezmStcMem_Initialization()");
    /* Reset the list of memory header*/
    for (uint16_t i = 0; i < NUM_OF_MEMHDR; i++)
    {
        ezmStcMem_ResetHeader(i);
    }
}

bool ezmStcMem_InitMemList(ezmMemList* pstList, uint8_t* pu8Buffer, uint16_t u16BufferSize)
{
    STCMEMPRINT1("ezmStcMem_InitMemList()");
    bool bSuccess = true;
    MemHdr* pstHeader = NULL;

    if (pstList == NULL || pu8Buffer == NULL || u16BufferSize == 0)
    {
        bSuccess = false;
    }

    if (bSuccess)
    {
        pstList->pau8Buffer = pu8Buffer;
        pstList->u16BufferSize = u16BufferSize;

        pstList->stAllocList.pstHead = NULL;
        pstList->stAllocList.pstTail = NULL;
        pstList->stAllocList.u16Size = 0U;

        pstHeader = ezmStcMem_GetFreeHeader();
        
        if (pstHeader != NULL)
        {
            pstList->stFreeList.pstHead = NULL;
            pstList->stFreeList.pstTail = NULL;
            pstList->stFreeList.u16Size = 0;

            pstHeader->pBuffer = pu8Buffer;
            pstHeader->u16BufferSize = u16BufferSize;
            LinkedList_InsertToHead(&pstList->stFreeList, pstHeader);
        }
        else
        {
            bSuccess = false;
        }
    }

    return bSuccess;
}

void* ezmStcMem_Malloc(ezmMemList* pstList, uint16_t u16Size)
{
    STCMEMPRINT1("ezmStcMem_Malloc()");
    void* pReturn = NULL;
    bool bSuccess = true;
    MemHdr* pstHeader = NULL;
    MemHdr* pstNextFreeHeader = NULL;

    if (pstList == NULL || u16Size == 0U)
    {
        bSuccess = false;
    }

    if (bSuccess)
    {
        pstNextFreeHeader = ezmStcMem_GetFreeHeader();

        if (pstNextFreeHeader == NULL)
        {
            bSuccess = false;
        }
    }

    if (bSuccess)
    {
        pstHeader = pstList->stFreeList.pstHead;
        while (pstHeader->u16BufferSize < u16Size && pstHeader->pstNextNode != NULL)
        {
            pstHeader = pstHeader->pstNextNode;
        }

        if (pstHeader->u16BufferSize >= u16Size)
        {
            pstNextFreeHeader->u16BufferSize = pstHeader->u16BufferSize - u16Size;

            /* wrap around point */
            if (pstNextFreeHeader->u16BufferSize == 0)
            {
                pstNextFreeHeader->pBuffer = pstList->pau8Buffer;
                STCMEMPRINT2("buffer reach the end, set [pointer = %p]", pstNextFreeHeader->pBuffer);
            }
            else
            {
                pstNextFreeHeader->pBuffer = (uint8_t*)(pstHeader->pBuffer + u16Size);
            }

            LinkedList_InsertToTail(&pstList->stFreeList, pstNextFreeHeader);

            pstHeader->u16BufferSize = u16Size;
            bSuccess = bSuccess & LinkedList_RemoveNode(&pstList->stFreeList ,pstHeader);
            LinkedList_InsertToTail(&pstList->stAllocList, pstHeader);
        }
        else
        {
            bSuccess = false;
        }
    }

    if (bSuccess)
    {
        pReturn = (void*)pstHeader->pBuffer;
    }

    ezmStcMem_PrintFreeList(pstList);
    ezmStcMem_PrintAllocList(pstList);

    return pReturn;
}

bool ezmStcMem_Free (ezmMemList* pstList, void* pAddress)
{
    bool bSuccess = true;
    LinkedList* pstAllocList = &pstList->stAllocList;
    LinkedList* pstFreeList = &pstList->stFreeList;
    MemHdr* pstNext = pstAllocList->pstHead;

    STCMEMPRINT2("ezmStcMem_Free() - [address = %p]", pAddress);

    if (pstList == NULL || pAddress == NULL)
    {
        bSuccess = false;
    }

    if (bSuccess)
    {
        while(pstNext != NULL)
        {
            if (pstNext->pBuffer == (uint8_t*)pAddress)
            {
                /* Do something */
                bSuccess = bSuccess & LinkedList_RemoveNode(pstAllocList, pstNext);
                ezmStcMem_ReturnHeaderToFreeList(pstFreeList, pstNext);
                ezmSmalloc_Merge(pstFreeList);
                ezmSmalloc_Merge(pstFreeList);
                STCMEMPRINT1("Free OK");
                break;
            }
            else
            {
                pstNext = pstNext->pstNextNode;
            }
        }
    }

    ezmStcMem_PrintFreeList(pstList);
    ezmStcMem_PrintAllocList(pstList);
    return bSuccess;
}

void ezmStcMem_HexdumpBuffer(ezmMemList* pstList)
{
#if (VERBOSE == 1U)
    if (pstList)
    {
        STCMEMHEXDUMP(pstList->pau8Buffer, pstList->u16BufferSize);
    }
#endif /* VERBOSE */
}

void ezmStcMem_PrintFreeList(ezmMemList* pstList)
{
#if (VERBOSE == 1U)
    MemHdr* pstNextHeader = pstList->stFreeList.pstHead;
    STCMEMPRINT1("*****************************************");
    while (pstNextHeader)
    {
        STCMEMPRINT2("[addr = %p]", pstNextHeader);
        STCMEMPRINT2("[next = %p]", pstNextHeader->pstNextNode);
        STCMEMPRINT2("[prev = %p]", pstNextHeader->pstPrevNode);
        STCMEMPRINT2("[buff = %p]", pstNextHeader->pBuffer);
        STCMEMPRINT2("[size = %d]", pstNextHeader->u16BufferSize);
        STCMEMPRINT2("[index = %d]", pstNextHeader->u16NodeIndex);
        STCMEMPRINT1("<======>");

        pstNextHeader = pstNextHeader->pstNextNode;
    }
    STCMEMPRINT1("*****************************************\n");
#endif
}

void ezmStcMem_PrintAllocList(ezmMemList* pstList)
{
#if (VERBOSE == 1U)
    MemHdr* pstNextHeader = pstList->stAllocList.pstHead;
    STCMEMPRINT1("*****************************************");
    while (pstNextHeader)
    {
        STCMEMPRINT2("[addr = %p]", pstNextHeader);
        STCMEMPRINT2("[next = %p]", pstNextHeader->pstNextNode);
        STCMEMPRINT2("[prev = %p]", pstNextHeader->pstPrevNode);
        STCMEMPRINT2("[buff = %p]", pstNextHeader->pBuffer);
        STCMEMPRINT2("[size = %d]", pstNextHeader->u16BufferSize);
        STCMEMPRINT2("[index = %d]", pstNextHeader->u16NodeIndex);
        STCMEMPRINT1("<======>");

        pstNextHeader = pstNextHeader->pstNextNode;
    }
    STCMEMPRINT1("*****************************************\n");
#endif
}

static void ezmStcMem_ResetHeader(uint16_t HeaderId)
{
    if (HeaderId < NUM_OF_MEMHDR)
    {
        astMemHeader[HeaderId].pBuffer = NULL;
        astMemHeader[HeaderId].pstNextNode = NULL;
        astMemHeader[HeaderId].pstPrevNode = NULL;
        astMemHeader[HeaderId].u16BufferSize = 0U;
        astMemHeader[HeaderId].u16NodeIndex = MEMHDR_INVALID_ID;
    }
}

static MemHdr* ezmStcMem_GetFreeHeader(void)
{
    STCMEMPRINT1("ezmStcMem_GetFreeHeader()");
    MemHdr* pstFreeHeader = NULL;

    for (uint16_t i = 0; i < NUM_OF_MEMHDR; i++)
    {
        if (astMemHeader[i].u16NodeIndex == MEMHDR_INVALID_ID)
        {
            /* store its own index for eaiser look up*/
            astMemHeader[i].u16NodeIndex = i;

            pstFreeHeader = &astMemHeader[i];

            STCMEMPRINT2("Found free instance [inst = %d]", i);
            break;
        }
    }

    return pstFreeHeader;
}

static void ezmStcMem_ReturnHeaderToFreeList(LinkedList * pstFreeList, MemHdr* pstHeader)
{
    memset(pstHeader->pBuffer, 0, pstHeader->u16BufferSize);
    MemHdr* pstNextHeader = pstFreeList->pstHead;

    while (pstNextHeader != NULL)
    {
        if ((uint8_t*)pstNextHeader->pBuffer <= (uint8_t*)pstHeader->pBuffer)
        {
            LinkedList_InsertNewNodeAfterANode(pstFreeList, pstNextHeader, pstHeader);
            break;
        }
        else
        {
            pstNextHeader = pstNextHeader->pstNextNode;
        }
    }
}

static void ezmSmalloc_Merge(LinkedList * pstFreeList)
{
    MemHdr* pstCurrentFreeBlock = pstFreeList->pstHead;
    MemHdr* pstNextFreeBlock = pstCurrentFreeBlock->pstNextNode;

    while (pstNextFreeBlock != NULL && ((uint8_t*)pstCurrentFreeBlock->pBuffer + pstCurrentFreeBlock->u16BufferSize) == pstNextFreeBlock->pBuffer)
    {
        STCMEMPRINT1("Next adjacent block is free");
        pstCurrentFreeBlock->u16BufferSize += pstNextFreeBlock->u16BufferSize;
        LinkedList_RemoveNode(pstFreeList, pstNextFreeBlock);
        ezmStcMem_ResetHeader(pstNextFreeBlock->u16NodeIndex);
        pstNextFreeBlock = pstCurrentFreeBlock->pstNextNode; /* Advance to next node */
    }
}
#endif /* STCMEM */
/* End of file */