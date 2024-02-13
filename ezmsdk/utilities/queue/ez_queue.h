
/*******************************************************************************
* Filename:         eez_queue.h
* Author:           Hai Nguyen
* Original Date:    20.08.2022
* Last Update:      20.08.2022
*
* -----------------------------------------------------------------------------
* Company:          Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 20.08.2022
*
*******************************************************************************/

/** @file   eez_queue.h
 *  @author Hai Nguyen
 *  @date   20.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZ_QUEUE_H
#define _EZ_QUEUE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezSdk_config.h"

#if (EZ_QUEUE == 1U)

#include <stdint.h>
#include "ez_linked_list.h"
#include "ez_static_alloc.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief queue structure
 *
 */
typedef struct ezQueue ezQueue;


/** @brief queue structure
 *
 */
struct ezQueue
{
    struct Node q_item_list;    /**< list of queue element */
    struct MemList mem_list;    /**< memory list, needed for static memory allocation*/
};


/** @brief queue structure
 *
 */
typedef void* ezReservedElement;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function creates a data queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    *buff: (IN) memory buffer providind to the queue to work
* @param    *buff_size: (IN)size of the memory buffer
* @return   ezSUCCESS or ezFAIL
*
* @Example Example:
* @code
* ezQueue queue;
* uint8_t queue_buff[32] = {0};
* ezSTATUS status = ezQueue_CreateQueue(&queue, queue_buff, 32);
* if(status == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_CreateQueue(ezQueue *queue, uint8_t *buff, uint32_t buff_size);


/******************************************************************************
* Function : ezQueue_PopFront
*//**
* @Description:
*
* This function pops the front element out of the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @return   ezSUCCESS or ezFAIL
*
* @Example Example:
* @code
* if(ezQueue_Pop(&queue) == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_PopFront(ezQueue *queue);


/******************************************************************************
* Function : ezQueue_PopBack
*//**
* @Description:
*
* This function pops the back element out of the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @return   ezSUCCESS or ezFAIL
*
* @Example Example:
* @code
* if(ezQueue_Pop(&queue) == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_PopBack(ezQueue *queue);


/******************************************************************************
* Function : ezQueue_ReserveElement
*//**
* @Description:
*
* This function reserves an element but does not link it to the queue and
* returns the pointer to the memory buff of the element. To link it to the queue
* user must call ezQueue_PushReservedElement(). In case this reserved element is
* not needed, the user MUST call ezQueue_ReleaseReservedElement() to "free" the
* element. This function gives the ability to block the memory first, and let
* the user to write the data into the queue later.
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    **data: (IN)pointer to the reserve memory block
* @param    data_size: (IN)size of the reserve memeory
* @return   NULL if fail
*
* @Example Example:
* @code
* uint8_t *buff;
* ezReservedElement elem;
* 
* elem = ezQueue_ReserveElement(&queue, buff, 32);
* if(elem != NULL)
* {
*     memset(buff, 0xaa, 32);
*     ezQueue_PushReservedElement(&queue, elem);
* }
* @endcode
*
*******************************************************************************/
ezReservedElement ezQueue_ReserveElement(ezQueue* queue, void **data, uint32_t data_size);


/******************************************************************************
* Function : ezQueue_PushReservedElement
*//**
* @Description: This function links the reserved element  to the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    element: (IN)reserved element
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezQueue_PushReservedElement(ezQueue *queue, ezReservedElement element);


/******************************************************************************
* Function : ezQueue_ReleaseReservedElement
*//**
* @Description: release the reserve element when it is not nedded
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    element: (IN)reserved element
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezQueue_ReleaseReservedElement(ezQueue *queue, ezReservedElement element);


/******************************************************************************
* Function : ezQueue_Push
*//**
* @Description:
*
* This function pushes data to the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    *data: (IN)data to be written in the queue
* @param    data_size: (IN)size of the data written to the queue
* @return   ezSUCCESS if success
*           ezFAIL: if the queue is full or invalid function arguments
*
* @Example Example:
* @code
* uint8_t queue_buff[32] = {0};
* if(ezQueue_Push(&queue, queue_buff, 32) == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_Push(ezQueue* queue, void *data, uint32_t data_size);


/******************************************************************************
* Function : ezQueue_GetFront
*//**
* @Description:
*
* This function let the user access to the front element of the queue.
* NOTE: since the users have the access to the queue it is NOT SAFE to write
* more than the size of this element
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    **data: (OUT)pointer the the data of the front element
* @param    *data_size: (OUT)pointer to the size of data
* @return   ezSUCCESS if success
*           ezFAIL: if the queue is empty or invalid function arguments
*
* @Example Example:
* uint8_t *front_element_data = NULL;
* uint32_t *data_size = NULL;
* if(ezQueue_GetFront(&queue, &front_element_data, &data_size) == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_GetFront(ezQueue *queue, void **data, uint32_t *data_size);


/******************************************************************************
* Function : ezQueue_GetBack
*//**
* @Description:
*
* This function let the user access to the back element of the queue.
* NOTE: since the users have the access to the queue it is NOT SAFE to write
* more than the size of this element
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @param    **data: (OUT)pointer the the data of the back element
* @param    *data_size: (OUT)pointer to the size of data
* @return   ezSUCCESS if success
*           ezFAIL: if the queue is empty or invalid function arguments
*
* @Example Example:
* uint8_t *back_element_data = NULL;
* uint32_t *data_size = NULL;
* if(ezQueue_GetFront(&queue, &back_element_data, &data_size) == ezSUCCESS)
* {
*     printf("Success");
* }
* @endcode
*
*******************************************************************************/
ezSTATUS ezQueue_GetBack(ezQueue* queue, void **data, uint32_t *data_size);


/******************************************************************************
* Function : ezQueue_GetNumOfElement
*//**
* @Description:
*
* This function returns the number of elements of the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @return   Number of elements
*
* @Example Example:
* @code
* uint32_t queue_size = ezQueue_GetNumOfElement(&queue);
* @endcode
*
*******************************************************************************/
uint32_t ezQueue_GetNumOfElement(ezQueue* queue);


/******************************************************************************
* Function : ezQueue_IsQueueReady
*//**
* @Description:
*
* This function returns readay status of the queue
*
* @param    *queue: (IN)pointer to the a queue structure, see ezQueue
* @return   True if ready, else false
*
*******************************************************************************/
uint32_t ezQueue_IsQueueReady(ezQueue *queue);


#endif /* CONFIG_EZ_QUEUE */
#endif /* _EZ_QUEUE_H */

/* End of file */

