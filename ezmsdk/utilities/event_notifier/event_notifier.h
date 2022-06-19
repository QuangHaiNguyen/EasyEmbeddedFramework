
/*******************************************************************************
* Filename:         event_notifier.h
* Author:           Hai Nguyen
* Original Date:    08.06.2022
* Last Update:      19.06.2022
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
* Written by Hai Nguyen 08.06.2022
*
*******************************************************************************/

/** @file   event_notifier.h
 *  @author Hai Nguyen
 *  @date   08.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EVENT_NOTIFIER_H
#define _EVENT_NOTIFIER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if(EVENT_NOTIFIER == 1U)

#include <stdint.h>
#include <stdbool.h>
#include "utilities/linked_list/linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief event callback pointer. It is used by the event notifier module to 
 *         notify the subscriber about an event.
 *         event_code: number representing an event
 *         param1 : send data to the event subscriber, can be NULL if not used
 *         param2 : send data to the event subscriber, can be NULL if not used
 */
typedef int (*EVENT_CALLBACK)(uint32_t event_code, void * param1, void * param2);

/** @brief define event_subject type.
 *
 */
typedef struct event_subject event_subject;

/** @brief define event_observer type.
 *
 */
typedef struct event_observer event_observer;


/** @brief structure to manage the data of the publisher
 *
 */
struct event_subject
{
    uint32_t num_of_subcriber;  /**< number of allowed subcriber*/
    struct Node head;           /**< head of the subcriber list */
};


/** @brief structure to manage the data of the publisher
 *
 */
struct event_observer
{
    struct Node node;           /**< link list node */
    EVENT_CALLBACK callback;    /**< event call back function */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool evntNoti_CreateSubject     (event_subject * subject,
                                 uint32_t num_of_allow_sub);

void evntNoti_ResetSubject      (event_subject* subject);

bool evntNoti_CreateObserver    (event_observer * observer,
                                 EVENT_CALLBACK callback);

bool evntNoti_SubscribeEvent    (event_subject * subject,
                                 event_observer * observer);

bool evntNoti_UnsubscribeEvent(event_subject* subject,
                               event_observer* observer);

void     evntNoti_NotifyEvent(event_subject* subject,
                              uint32_t event_code,
                              void* param1,
                              void* param2);

#endif /* (EVENT_NOTIFIER == 1U) */

#endif /* _EVENT_NOTIFIER_H */

/* End of file */

