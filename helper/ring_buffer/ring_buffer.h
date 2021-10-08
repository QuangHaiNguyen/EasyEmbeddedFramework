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
#include "../../app/app_config.h"
#include "string.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define STATIC_MEM  0

#if (STATIC_MEM == 1)
    #define STATIC_MEM_SIZE 512U
    #if STATIC_MEM_SIZE > 0xFFFF
        #error size is bigger than 16 bits
    #endif
#else
    #if (SMALLOC == 1U)
        #include "../smalloc/smalloc.h"
    #else
        #error SMALLOC module must be activated
    #endif
#endif
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**
 *
 * @brief   metadata of the ring buffer
**/
typedef struct
{
#if (STATIC_MEM == 0)
    uint8_t *pu8Buff;
#else
    uint8_t buff[STATIC_MEM_SIZE];
#endif
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


/**
 *
 * @brief   Status of the ring buffer
**/
typedef enum
{
    BUFF_OK = 0,
    /**< Buffer OK*/
    BUFF_NO_MEMORY,
    /**<Buffer does not have enough memory */
	BUFF_ERROR,
    /**< buffer error */
	BUFF_FULL,
    /**< buffer is full */
	BUFF_EMPTY
    /**< buffer is empty */
}RingBuff_Status;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//None

/******************************************************************************
* Function Prototypes
*******************************************************************************/
RingBuff_Status ezmRingBuffer_IsEmpty(RingBuffer * pstBuff);
RingBuff_Status ezmRingBuffer_IsFull(RingBuffer * pstBuff);
RingBuff_Status ezmRingBuffer_Push(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);
RingBuff_Status ezmRingBuffer_Pop(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);
RingBuff_Status ezmRingBuffer_Deinit(RingBuffer * pstBuff);
RingBuff_Status ezmRingBuffer_Reset(RingBuffer * pstBuff);
RingBuff_Status ezmRingBuffer_Init( RingBuffer * pstBuff, uint16_t u16Size );
RingBuff_Status ezmRingBuffer_GetAvailableMemory( RingBuffer * pstBuff, uint16_t * pu16AvailableMem);

#endif	/* RING_BUFFER_H */

/* End of file*****************************************************************/
