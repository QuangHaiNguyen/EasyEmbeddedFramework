/*****************************************************************************
* Filename:         ez_event_notifier.h
* Author:           Hai Nguyen
* Original Date:    27.02.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_event_notifier.h
 *  @author Hai Nguyen
 *  @date   27.02.2024
 *  @brief  Public API of the event notifier component.
 *
 *  @details Event notifier component implements the observer design pattern.
 */

#ifndef _EZ_EVENT_NOTIFIER_H
#define _EZ_EVENT_NOTIFIER_H

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_BUILD_WITH_CMAKE == 0U)
#include "ez_target_config.h"
#endif

#if(EZ_EVENT_NOTIFIER == 1U)
#include <stdint.h>
#include <stdbool.h>
#include "ez_linked_list.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */


/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/** @brief event callback pointer. It is used by the event notifier module to 
 *         notify the subscriber about an event.
 *         event_code: number representing an event. Specified by the event
 *         subject
 *         param1: pointer to first parameter, can be NULL if not used
 *         param2: pointer to second parameter, can be NULL if not used
 *
 */
typedef int (*EVENT_CALLBACK)(uint32_t event_code, void *param1, void *param2);


/** @brief Observer object, used to subscribed to a subject to receive event
 *         notification
 */
struct ezObserver
{
    struct Node node;           /**< linked list node */
    EVENT_CALLBACK callback;    /**< event call back function */
};


/** @brief define event_subject type.
 */
typedef struct Node ezSubject;


/** @brief define event_observer type.
 */
typedef struct ezObserver ezObserver;


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function : ezEventNotifier_CreateSubject
*//** 
* @brief This function creates a subject for observers to subscribe to.
*
* @details The user is RECOMMENDED to use this function to create the subject
* instead of modify the struct by themselve.
*
* @param[in]    *subject: Pointer to the subject
* @return       ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezSubject subject;
* (void) ezEventNotifier_CreateSubject(&subject);
* @endcode
*
*****************************************************************************/
ezSTATUS ezEventNotifier_CreateSubject(ezSubject *subject);


/******************************************************************************
* Function: ezEventNotifier_ResetSubject
*//**
* @brief This function resets a subject
*
* @details It resets the linked list and unlinks all nodes of that list
*
* @param[in]    *subject: Pointer to the subject
* @return       None
*
* @pre Subject must be created
* @post None
*
* \b Example
* @code
* ezEventNotifier_ResetSubject(&subject);
* @endcode
*
* @see ezEventNotifier_CreateSubject
*
*******************************************************************************/
void ezEventNotifier_ResetSubject(ezSubject *subject);


/******************************************************************************
* Function: ezEventNotifier_CreateObserver
*//**
* @brief This function creates an observer for the events from a subject.
*
* @details The user is RECOMMENDED to use this function to create the observer
* instead of modify the struct by themselve.
*
* @param[in]    *observer: Pointer to the obsever
* @param[in]    callback: Callback to handle event from the subject
* @return       ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezObserver subject;
* (void) ezEventNotifier_CreateObserver(&observer, EventHandleFunction);
* @endcode
*
*******************************************************************************/
ezSTATUS ezEventNotifier_CreateObserver(ezObserver * observer,
                                        EVENT_CALLBACK callback);


/******************************************************************************
* Function: ezEventNotifier_SubscribeEvent
*//**
* @brief This function subcribes the observer to the subject
*
* @details
*
* @param[in]    *subject: Pointer to the subject
* @param[in]    *observer: Publisher handle
* @return       ezSTATUS
*
* @pre subject and observer must be created
* @post None
*
* \b Example
* @code
* ezEventNotifier_SubscribeEvent(&subject, &observer);
* @endcode
*
* @see ezEventNotifier_CreateSubject, ezEventNotifier_CreateObserver
*
*******************************************************************************/
ezSTATUS ezEventNotifier_SubscribeToSubject(ezSubject *subject,
                                            ezObserver *observer);


/******************************************************************************
* Function: ezEventNotifier_UnsubscribeEvent
*//**
* @brief This function unscribe the observer to from subject
*
* @details
*
* @param[in]    *subject: Pointer to the subject
* @param[in]    *observer: Publisher handle
* @return       ezSTATUS
*
* @pre subject and observer must be created
* @post None
*
* \b Example
* @code
* ezEventNotifier_UnsubscribeEvent(&subject, &observer);
* @endcode
*
* @see ezEventNotifier_CreateSubject, ezEventNotifier_CreateObserver
*
*******************************************************************************/
ezSTATUS ezEventNotifier_UnsubscribeFromSubject(ezSubject *subject,
                                                ezObserver *observer);


/******************************************************************************
* Function: ezEventNotifier_GetNumOfObservers
*//**
* @brief This function returns the number of observers in a subject
*
* @details
*
* @param    *observer: (IN)publisher handle
* @return   number of observers
*
* @pre subject must be created
* @post None
*
* \b Example
* @code
* uint16_t num_observer = ezEventNotifier_GetNumOfObservers(&subject);
* @endcode
*
* @see ezEventNotifier_CreateSubject
*
*******************************************************************************/
uint16_t ezEventNotifier_GetNumOfObservers(ezSubject *subject);


/******************************************************************************
* Function: ezEventNotifier_NotifyEvent
*//**
* @brief This function notifies the subscriber about an event has just happened
*
* @details
*
* @param[in]    *subject: Pointer to the subject
* @param[in]    event_code: Event code. Defined by users
* @param[in]    param1: Companion paramneter 1, set to NULL if unused
* @param[in]    param1: Companion paramneter 2, set to NULL if unused
* @return       None
*
* @pre  subject must be created. event_code, param1, and param2 are defined by
*       the user
* @post None
*
* \b Example
* @code
* evntNoti_NotifyEnvent(&subject, ENUM_ERROR_CODE, NULL, NULL);
* @endcode
*
* @see ezEventNotifier_CreateSubject
*
*******************************************************************************/
void ezEventNotifier_NotifyEvent(ezSubject *subject,
                                 uint32_t event_code,
                                 void* param1,
                                 void* param2);

#ifdef __cplusplus
}
#endif

#endif /* (EZ_EVENT_NOTIFIER == 1U) */
#endif /* _EZ_EVENT_NOTIFIER_H */

/* End of file */
