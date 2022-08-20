
/*******************************************************************************
* Filename:         eez_queue.h
* Author:           Hai Nguyen
* Original Date:    20.08.2022
* Last Update:      20.08.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
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

#ifndef _EEZ_QUEUE_H
#define _EEZ_QUEUE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_EZ_QUEUE == 1U)

#include <stdint.h>
#include "utilities/linked_list/linked_list.h"
#include "utilities/stcmem/stcmem.h"
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *
 */
typedef struct ezQueue ezQueue;

struct ezQueue
{
    struct Node q_item_list;
    struct MemList mem_list;
};
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
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
ezSTATUS ezQueue_CreateQueue(ezQueue *queue, uint8_t *buff, uint32_t buff_size);


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
ezSTATUS ezQueue_Pop(ezQueue *queue);


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
ezSTATUS ezQueue_Push(ezQueue* queue, void *data, uint32_t data_size);


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
ezSTATUS ezQueue_GetFront(ezQueue* queue, void **data, uint32_t *data_size);


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
ezSTATUS ezQueue_GetBack(ezQueue* queue, void **data, uint32_t *data_size);


/******************************************************************************
* Function : ezQueue_CreateQueue
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
uint32_t ezQueue_GetNumOfElement(ezQueue* queue);


#endif /* CONFIG_EZ_QUEUE */
#endif /* _EEZ_QUEUE_H */

/* End of file */

