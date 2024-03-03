/*****************************************************************************
* Filename:         ez_ring_buffer.h
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


/** @file   ez_ring_buffer.h
 *  @author Hai Nguyen
 *  @date   26.02.2024
 *  @brief  Public API of ring buffer component
 *
 *  @details
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/** @brief Data structure of a ring buffer
 */
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


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/*****************************************************************************
* Function : ezmRingBuffer_Init
*//** 
* @brief This function initializes the ring buffer
*
* @details
*
* @param[out]   pstBuff: pointer to the ring buffer
* @param[in]    u16Size: size of the ring buffer
* @return       true if success, else false
*
* @pre None
* @post None
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
bool ezmRingBuffer_Init(RingBuffer* pstBuff, uint8_t * pu8Buff, uint16_t u16Size);


/*****************************************************************************
* Function : ezmRingBuffer_IsEmpty
*//** 
* @brief Check if the buffer is empty
*
* @details
*
* @param[in]    pstBuff: (IN)pointer to the ring buffer
* @return       true if empty, otherwise false
*
* @pre A RingBuffer is exsiting
* @post None
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* RingBuff_Status status;
* status = RingBuffer_IsEmpty(&buffer);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
bool ezmRingBuffer_IsEmpty(RingBuffer * pstBuff);


/*****************************************************************************
* Function : ezmRingBuffer_IsFull
*//** 
* @brief Check if the buffer is full
*
* @details
*
* @param[in]    pstBuff: pointer to the ring buffer
* @return       True if full, else false
*
* @pre a RingBuffer is exsiting
* @post None
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* RingBuff_Status status;
* status = RingBuffer_IsFull(&buffer);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
bool ezmRingBuffer_IsFull(RingBuffer * pstBuff);


/*****************************************************************************
* Function : ezmRingBuffer_Push
*//** 
* @brief Push data into the ring buffet
*
* @details
*
* @param[in]    pstBuff: pointer to the ring buffer
* @param[in]    pu8Data: pointer to the pushed data
* @param[in]    u16Size: size of the data
* @return       Number of bytes pushed into the ring buffer
*
* @pre a RingBuffer is exsiting
* @post None
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8Data[3U] = {0U,1U,2U}
* RingBuffer_Push(&buffer, u8Data, 3U);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
uint16_t ezmRingBuffer_Push(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);


/*****************************************************************************
* Function : ezmRingBuffer_Pop
*//** 
* @brief Pop data out of the ring buffer
*
* @details
*
* @param[in]    pstBuff: pointer to the ring buffer
* @param[out]   pu8Data: pointer to the popped data
* @param[in]    u16Size: size of the popped data
* @return       Number of bytes popped out of the ring buffer
*
* @pre a RingBuffer is exsiting
* @post Postcondition
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8Data[3U];
* RingBuffer_Pop(&buffer, u8Data, 3U);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
uint16_t ezmRingBuffer_Pop(RingBuffer * pstBuff, uint8_t * pu8Data, uint16_t u16Size);


/*****************************************************************************
* Function : ezmRingBuffer_Reset
*//** 
* @brief A RingBuffer is exsiting
*
* @details
*
* @param[in]    pstBuff: pointer to the ring buffer
* @return       None
*
* @pre a RingBuffer is exsiting
* @post None
*
* \b Example
* @code
* RingBuffer_Reset(&buffer);
* @endcode
*
* @see ezmRingBuffer_Init
*
*****************************************************************************/
void ezmRingBuffer_Reset(RingBuffer * pstBuff);


/*****************************************************************************
* Function : ezmRingBuffer_GetAvailableMemory
*//** 
* @brief Return number of bytes available
*
* @details
*
* @param[in]    pstBuff: pointer to the ring buffer
* @param[out]   avail_mem: pointer to the variable holding the available memory
* @return       Number of available bytes
*
* \b Example
* @code
* RingBuffer buffer;
* RingBuffer_Init(&buffer, 30);
* uint8_t u8AvailMem;
* RingBuffer_GetAvailableMemory(&buffer, &u8AvailMem);
* @endcode
*
* @see RingBuffer_Init
*
*****************************************************************************/
uint16_t ezmRingBuffer_GetAvailableMemory( RingBuffer * pstBuff);

#endif	/* RING_BUFFER_H */

/* End of file */
