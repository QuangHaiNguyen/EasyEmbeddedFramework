
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
#include "ezApp/ezSdk_config.h"

#if(CONFIG_EVENT_NOTIFIER == 1U)

#include <stdint.h>
#include <stdbool.h>
#include "ezUtilities/linked_list/linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief event callback pointer. It is used by the event notifier module to 
 *         notify the subscriber about an event.
 *         event_code: number representing an event. Specified by the event subject
 *         param1: pointer to first parameter, can be NULL if not used
 *         param2: pointer to second parameter, can be NULL if not used
 */
typedef int (*EVENT_CALLBACK)(uint32_t event_code, void *param1, void *param2);

/** @brief define event_subject type.
 *
 */
typedef struct Node ezSubject;

/** @brief define event_observer type.
 *
 */
typedef struct ezObserver ezObserver;


/** @brief Observer object, used to subscribed to
 *
 */
struct ezObserver
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


/******************************************************************************
* Function : ezEventNotifier_CreateSubject
*//**
* @Description:
*
* This function creates a subject for observers subscribe to. The user is
* RECOMMENDED to use this function to create the subject instead of modify the
* struct by themself
*
* @param    *subject: (IN)pointer to the subject
* @return   ezSTATUS
*
* @Example Example:
* @code
* ezSubject subject;
* (void) ezEventNotifier_CreateSubject(&subject);
* @endcode
*
*******************************************************************************/
ezSTATUS ezEventNotifier_CreateSubject(ezSubject *subject);


/******************************************************************************
* Function : ezEventNotifier_ResetSubject
*//**
* @Description:
*
* This function reset a subject
*
* @param    *subject: (IN)pointer to the subject
* @return   None
*
* @Example Example:
* @code
* ezEventNotifier_ResetSubject(&subject);
* @endcode
*
*******************************************************************************/
void ezEventNotifier_ResetSubject(ezSubject *subject);


/******************************************************************************
* Function : ezEventNotifier_CreateObserver
*//**
* @Description:
*
* This function creates an observer for the events from a subject. The user is
* RECOMMENDED to use this function to create the observer instead of modify the
* struct by themself
*
* @param    *observer: (IN)pointer to the obsever
* @param    callback: (IN)callback to handle event from the subject
* @return   ezSTATUS
*
* @Example Example:
* @code
* ezObserver subject;
* (void) ezEventNotifier_CreateObserver(&observer, EventHandleFunction);
* @endcode
*
*******************************************************************************/
ezSTATUS ezEventNotifier_CreateObserver(ezObserver * observer,
                                        EVENT_CALLBACK callback);


/******************************************************************************
* Function : ezEventNotifier_SubscribeEvent
*//**
* @Description:
*
* This function subcribes the observer to the subject
*
* @param    *subject: (IN)pointer to the subject
* @param    *observer: (IN)publisher handle
*
* @return   ezSTATUS
*
* @Example Example:
* @code
* ezEventNotifier_SubscribeEvent(&subject, &observer);
* @endcode
*
*******************************************************************************/
ezSTATUS ezEventNotifier_SubscribeToSubject(ezSubject *subject,
                                            ezObserver *observer);


/******************************************************************************
* Function : ezEventNotifier_UnsubscribeEvent
*//**
* @Description:
*
* This function unscribe the observer to from subject
*
* @param    *subject: (IN)pointer to the subject
* @param    *observer: (IN)publisher handle
*
* @return   ezSTATUS
*
* @Example Example:
* @code
* ezEventNotifier_UnsubscribeEvent(&subject, &observer);
* @endcode
*
*******************************************************************************/
ezSTATUS ezEventNotifier_UnsubscribeFromSubject(ezSubject *subject,
                                                ezObserver *observer);


/******************************************************************************
* Function : ezEventNotifier_GetNumOfObservers
*//**
* @Description:
*
* This function returns the number of observers in a subject
*
* @param    *observer: (IN)publisher handle
*
* @return   number of observers
*
* @Example Example:
* @code
* uint16_t num_observer = ezEventNotifier_GetNumOfObservers(&subject);
* @endcode
*
*******************************************************************************/
uint16_t ezEventNotifier_GetNumOfObservers(ezSubject *subject);


/******************************************************************************
* Function : evntNoti_NotifyEvent
*//**
* @Description:
*
* This function notifies the subscriber about an event has just been happening
*
* @param    *subject: (IN)pointer to the subject
* @param    event_code: (IN)event code
* @param    param1: (IN)companion paramneter 1, set to NULL if unused
* @param    param1: (IN)companion paramneter 2, set to NULL if unused
*
* @return   None
*
* @Example Example:
* @code
* evntNoti_NotifyEnvent(&subject, ENUM_ERROR_CODE, NULL, NULL);
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
void ezEventNotifier_NotifyEvent(ezSubject *subject,
                                 uint32_t event_code,
                                 void* param1,
                                 void* param2);

#endif /* (EVENT_NOTIFIER == 1U) */

#endif /* _EVENT_NOTIFIER_H */

/* End of file */

