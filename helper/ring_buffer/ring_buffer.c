/*******************************************************************************
* Title                 :   ring buffer module 
* Filename              :   ring_buffer.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** SOURCE REVISION LOG *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Initial Release.
*  28.03.2021   1.0.1       Quang Hai Nguyen    Increase buff to uint16_t,
*                                               Add static memory
*
*******************************************************************************/
/** @file  ring_buffer.c
 *  @brief implemetation of the ring buffer
 * 
 * <br><b> - HISTORY OF CHANGES - </b>
 *  
 * <table align="left" style="width:800px">
 * <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
 * <tr><td> 24.01.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Init </td></tr>
 * <tr><td> 28.03.2021 </td><td> 1.0.1            </td><td> Quang Hai Nguyen </td><td> increase buff size and support static memory </td></tr>
 * </table><br><br>
 * <hr>
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ring_buffer.h"

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function : RingBuffer_Init
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (OUT)pointer to the ring buffer
* @param    u16Size:   (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* @endcode
*
* @see RingBuffer_Init
*
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_Init( RingBuffer * pstBuff, uint16_t u16Size)
{
    RingBuff_Status eResult = BUFF_OK;

    pstBuff->u16Head = 0;
    pstBuff->u16Tail = 0;
    pstBuff->u16Count = 0;

#if (STATIC_MEM == 1)
    pstBuff->u16Capacity = STATIC_MEM_SIZE;
#else
    pstBuff->u16Capacity = u16Size;
    pstBuff->pu8Buff = ezmSmalloc_Malloc(u16Size);

    if(NULL == pstBuff->pu8Buff)
    {
        eResult = BUFF_NO_MEMORY;
    }
#endif

    memset(pstBuff->pu8Buff, 0, pstBuff->u16Capacity);
    return eResult;
}


/******************************************************************************
* Function : RingBuffer_IsEmpty
*//** 
* \b Description:
*
* Check if the buffer is empty
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (IN)pointer to the ring buffer
* @return   Status of buffer
*           + OK (not empty)
*           + EMPTY
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* RingBuff_Status status;
* status = RingBuffer_IsEmpty(&buffer);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_IsEmpty(RingBuffer * pstBuff)
{
    return (0 == pstBuff->u16Count) ? BUFF_EMPTY : BUFF_OK;
}


/******************************************************************************
* Function : RingBuffer_IsFull
*//** 
* \b Description:
*
* Check if the buffer is full
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (IN)pointer to the ring buffer
* @return   Status of buffer
*           + OK (not empty)
*           + FULL
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* RingBuff_Status status;
* status = RingBuffer_IsFull(&buffer);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_IsFull(RingBuffer * pstBuff)
{
    return (pstBuff->u16Count == pstBuff->u16Capacity) ? BUFF_FULL : BUFF_OK;
}

/******************************************************************************
* Function : RingBuffer_Push
*//** 
* \b Description:
*
* Push data into buffer
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:    (IN)pointer to the ring buffer
* @param    pu8Data:    (IN)pointer to the pushed data
* @param    u16Size:    (IN)size of the data
* @return   Status of buffer
*           + BUFF_OK
*           + BUFF_NO_MEMORY
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8Data[3U] = {0U,1U,2U}
* RingBuffer_Push(&buffer, u8Data, 3U);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_Push(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size)
{
    if(pstBuff->u16Capacity - pstBuff->u16Count < u16Size){
        return BUFF_NO_MEMORY;
    }
    else{
        memcpy(&pstBuff->pu8Buff[pstBuff->u16Head], pu8Data, u16Size);
        pstBuff->u16Head = (pstBuff->u16Head + u16Size <  pstBuff->u16Capacity) ? (pstBuff->u16Head + u16Size) : 0U;
        pstBuff->u16Count = (pstBuff->u16Count + u16Size <  pstBuff->u16Capacity) ? (pstBuff->u16Count + u16Size) :  pstBuff->u16Capacity;
        return BUFF_OK;
    }
}

/******************************************************************************
* Function : RingBuffer_Pop
*//** 
* \b Description:
*
* pop data out of buffer
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:    (IN)pointer to the ring buffer
* @param    pu8Data:    (OUT)pointer to the popped data
* @param    u16Size:    (IN)size of the popped data
* @return   Status of buffer
*           + BUFF_OK
*           + BUFF_NO_MEMORY
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8Data[3U];
* RingBuffer_Pop(&buffer, u8Data, 3U);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_Pop(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size)
{
    if(pstBuff->u16Count < u16Size)
    {
        return BUFF_NO_MEMORY;
    }
    else
    {
        memcpy(pu8Data, &pstBuff->pu8Buff[pstBuff->u16Tail], u16Size);
        pstBuff->u16Tail = (pstBuff->u16Tail + u16Size < pstBuff->u16Capacity) ? (pstBuff->u16Tail + u16Size) : 0U;
        pstBuff->u16Count = (pstBuff->u16Count - u16Size > 0) ? (pstBuff->u16Count - u16Size) : 0U;
        return BUFF_OK;
        
    }
}

/******************************************************************************
* Function : RingBuffer_Deinit
*//** 
* \b Description:
*
* Deinit the ring buffer
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (IN)pointer to the ring buffer
* @return   Status of buffer
*           + BUFF_OK
*
* \b Example Example:
* @code
* RingBuffer_Deinit(&buffer);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_Deinit(RingBuffer * pstBuff)
{
    if(pstBuff->pu8Buff != NULL)
    {
        emzSmalloc_Free(pstBuff->pu8Buff);
    }
    return BUFF_OK;
}

/******************************************************************************
* Function : RingBuffer_Reset
*//** 
* \b Description:
*
* Reset the buffer
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (IN)pointer to the ring buffer
* @return   Status of buffer
*           + BUFF_OK
*
* \b Example Example:
* @code
* RingBuffer_Reset(&buffer);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_Reset(RingBuffer * pstBuff)
{
    pstBuff->u16Head = 0;
    pstBuff->u16Tail = 0;
    pstBuff->u16Count = 0;
    memset(pstBuff->pu8Buff, 0, pstBuff->u16Capacity);
    return BUFF_OK;
}

/******************************************************************************
* Function : RingBuffer_GetAvailableMemory
*//** 
* \b Description:
*
* Get avalable memory from the buffer
*
* PRE-CONDITION: a RingBuffer is exsiting
*
* POST-CONDITION: None
* 
* @param    pstBuff:  (IN)pointer to the ring buffer
* @param    avail_mem:   (OUT)pointer to the variable holding the available memory
* @return   Status of buffer
*           + BUFF_OK
*
* \b Example Example:
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8AvailMem;
* RingBuffer_GetAvailableMemory(&buffer, &u8AvailMem);
* @endcode
*
* @see RingBuffer_Init
*
*******************************************************************************/
RingBuff_Status ezmRingBuffer_GetAvailableMemory( RingBuffer * pstBuff, uint16_t * pu16AvailableMem)
{
    *pu16AvailableMem = pstBuff->u16Capacity - pstBuff->u16Count;
    return BUFF_OK;
}

/* End of file*****************************************************************/
