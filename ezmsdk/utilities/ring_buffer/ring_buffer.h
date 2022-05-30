/*******************************************************************************
* Title                 :   ring buffer component
* Filename              :   ring_buffer.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*  28.03.2021   1.0.1       Quang Hai Nguyen    increase buff to uint16_t,
*                                               add static memory
*
*******************************************************************************/
/** @file   ring_buffer.h
 *  @brief  Header file for the ring buffer
 */

#ifndef RING_BUFFER_H
#define	RING_BUFFER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**
 *
 * @brief   metadata of the ring buffer
**/
typedef struct
{
    uint8_t *pu8Buff;
    /**< pointer to the data buffer*/
    uint16_t u16Capacity;
    /**< size of the buffer*/
    uint16_t u16Head;
    /**< buffer head*/
    uint16_t u16Tail;
    /**< buffer tail*/
    uint16_t u16Count;
    /**< number of byte written*/
}RingBuffer;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//None

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool        ezmRingBuffer_Init(RingBuffer* pstBuff, uint8_t * pu8Buff, uint16_t u16Size);
bool        ezmRingBuffer_IsEmpty(RingBuffer * pstBuff);
bool        ezmRingBuffer_IsFull(RingBuffer * pstBuff);
uint16_t    ezmRingBuffer_Push(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);
uint16_t    ezmRingBuffer_Pop(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);
void        ezmRingBuffer_Reset(RingBuffer * pstBuff);
uint16_t    ezmRingBuffer_GetAvailableMemory( RingBuffer * pstBuff);

#endif	/* RING_BUFFER_H */

/* End of file*****************************************************************/
