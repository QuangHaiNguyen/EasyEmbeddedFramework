/*****************************************************************************
* Filename:         ez_ring_buffer.c
* Author:           Hai Nguyen
* Original Date:    26.02.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_ring_buffer.c
 *  @author Hai Nguyen
 *  @date   26.02.2024
 *  @brief  Implementation of the ring data structure
 *
 *  @details
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_ring_buffer.h"

#if (EZ_RING_BUFFER == 1U)
#include "string.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */


/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Public functions
*****************************************************************************/
bool ezmRingBuffer_Init(RingBuffer* pstBuff, uint8_t* pu8Buff, uint16_t u16Size)
{
    bool bResult = true;

    pstBuff->u16Head = 0;
    pstBuff->u16Tail = 0;
    pstBuff->u16Count = 0;

    if(pu8Buff == NULL && u16Size == 0)
    {
        bResult = false;
    }
    else
    {
        pstBuff->pu8Buff = pu8Buff;
        pstBuff->u16Capacity = u16Size;
    }
    return bResult;
}


bool ezmRingBuffer_IsEmpty(RingBuffer * pstBuff)
{
    return (0 == pstBuff->u16Count);
}


bool ezmRingBuffer_IsFull(RingBuffer * pstBuff)
{
    return (pstBuff->u16Count == pstBuff->u16Capacity);
}


uint16_t ezmRingBuffer_Push(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size)
{
    uint16_t u16RemainByte = 0U;
    uint16_t u16NumPushedData = pstBuff->u16Capacity - pstBuff->u16Count;

    if(u16NumPushedData >= u16Size)
    {
        u16NumPushedData = u16Size;
    }

    if (pstBuff->u16Capacity - pstBuff->u16Head >= u16NumPushedData)
    {
        memcpy(&pstBuff->pu8Buff[pstBuff->u16Head], pu8Data, u16NumPushedData);
        pstBuff->u16Head = pstBuff->u16Head + u16NumPushedData;
        pstBuff->u16Count = pstBuff->u16Count + u16NumPushedData;
    }
    else
    {
        /* Handle warpping */
        u16RemainByte = pstBuff->u16Capacity - pstBuff->u16Head;
        memcpy(&pstBuff->pu8Buff[pstBuff->u16Head], pu8Data, u16RemainByte);
        pstBuff->u16Head = 0;
        pstBuff->u16Count = pstBuff->u16Count + u16RemainByte;
        pu8Data = pu8Data + u16RemainByte;

        u16RemainByte = u16NumPushedData - u16RemainByte;
        memcpy(&pstBuff->pu8Buff[pstBuff->u16Head], pu8Data, u16RemainByte);
        pstBuff->u16Head = pstBuff->u16Head + u16RemainByte;
        pstBuff->u16Count = pstBuff->u16Count + u16RemainByte;
    }

    return u16NumPushedData;
}


uint16_t ezmRingBuffer_Pop(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size)
{

    uint16_t u16NumPoppedData = u16Size;
    uint16_t u16RemainByte = 0U;

    if(pstBuff->u16Count < u16NumPoppedData)
    {
        u16NumPoppedData = pstBuff->u16Count;
    }

    if (pstBuff->u16Capacity - pstBuff->u16Tail >= u16NumPoppedData)
    {
        memcpy(pu8Data, &pstBuff->pu8Buff[pstBuff->u16Tail], u16NumPoppedData);
        pstBuff->u16Tail = pstBuff->u16Tail + u16NumPoppedData;
        pstBuff->u16Count = pstBuff->u16Count - u16NumPoppedData;
    }
    else
    {
        /* Handle warpping */
        u16RemainByte = pstBuff->u16Capacity - pstBuff->u16Tail;
        memcpy(pu8Data, &pstBuff->pu8Buff[pstBuff->u16Tail], u16RemainByte);
        pstBuff->u16Tail = 0;
        pstBuff->u16Count = pstBuff->u16Count - u16RemainByte;

        pu8Data = pu8Data + u16RemainByte;

        u16RemainByte = u16NumPoppedData - u16RemainByte;
        memcpy(pu8Data, &pstBuff->pu8Buff[pstBuff->u16Tail], u16RemainByte);
        pstBuff->u16Tail = pstBuff->u16Tail + u16RemainByte;
        pstBuff->u16Count = pstBuff->u16Count - u16RemainByte;
    }

    return u16NumPoppedData;
}


void ezmRingBuffer_Reset(RingBuffer * pstBuff)
{
    pstBuff->u16Head = 0;
    pstBuff->u16Tail = 0;
    pstBuff->u16Count = 0;
    memset(pstBuff->pu8Buff, 0, pstBuff->u16Capacity);
}


uint16_t ezmRingBuffer_GetAvailableMemory( RingBuffer * pstBuff)
{
    return pstBuff->u16Capacity - pstBuff->u16Count;
}

#endif /* CONFIG_RING_BUFFER */

/* End of file */
