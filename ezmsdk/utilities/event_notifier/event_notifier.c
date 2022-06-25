
/*******************************************************************************
* Filename:         event_notifier.c
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

/** @file   event_notifier.c
 *  @author Hai Nguyen
 *  @date   08.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "event_notifier.h"


#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "EVENT_NOTI"    /**< module name */

#if(EVENT_NOTIFIER == 1U)


#include "utilities/logging/logging.h"
#include "utilities/ezmAssert/ezmAssert.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

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
* External functions
*******************************************************************************/


/******************************************************************************
* Function : evntNoti_CreateSubject
*//** 
* @Description:
*
* This function creates a subject for observers subscribe to. The user is
* RECOMMENDED to use this function to create the subject instead of modify the
* struct by themself
* 
* @param    *subject: (IN)pointer to the subject
* @param    num_of_allow_sub: (IN)number of supported subscriber
* @return   true: success
*           fail: false
*
* @Example Example:
* @code
* event_subject subject;
* (void) evntNoti_CreatePublisher(&subject, 10);
* @endcode
*
*******************************************************************************/
bool evntNoti_CreateSubject(event_subject * subject, uint32_t num_of_allow_sub)
{
    TRACE("evntNoti_CreateSubject()");
    bool is_success = false;

    if (subject)
    {
        ezmLL_InitNode(&subject->head);
        subject->num_of_subcriber = num_of_allow_sub;
        is_success = true;
    }

    return is_success;
}

/******************************************************************************
* Function : evntNoti_ResetSubject
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
* evntNoti_ResetSubject(&subject);
* @endcode
*
*******************************************************************************/
void evntNoti_ResetSubject(event_subject* subject)
{
    if (subject != NULL)
    {
        subject->num_of_subcriber = 0;
        ezmLL_InitNode(&subject->head);
    }
}

/******************************************************************************
* Function : evntNoti_CreateObserver
*//**
* @Description:
*
* This function creates an observer for the events from a subject. The user is
* RECOMMENDED to use this function to create the observer instead of modify the
* struct by themself
*
* @param    *observer: (IN)pointer to the obsever
* @param    callback: (IN)callback to handle event from the subject
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* event_observer subject;
* (void) evntNoti_CreateObserver(&observer, EventHandleFunction);
* @endcode
*
*******************************************************************************/
bool evntNoti_CreateObserver(event_observer * observer, EVENT_CALLBACK callback)
{
    TRACE("evntNoti_CreateObserver()");
    bool is_success = false;

    if (observer != NULL && callback != NULL)
    {
        observer->callback = callback;
        is_success = true;
    }

    return is_success;
}

/******************************************************************************
* Function : evntNoti_SubscribeEvent
*//**
* @Description:
*
* This function subcribes the observer to the subject
*
* @param    *subject: (IN)pointer to the subject
* @param    *observer: (IN)publisher handle
*
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* evntNoti_SubscribeEvent(&subject, &observer);
* @endcode
*
*******************************************************************************/
bool evntNoti_SubscribeEvent( event_subject* subject,
                              event_observer* observer)
{
    TRACE("evntNoti_SubscribeEvent()");
    bool is_success = false;

    if (subject != NULL &&
        observer != NULL &&
        ezmLL_GetListSize(&subject->head) < subject->num_of_subcriber)
    {
        EZMLL_ADD_HEAD(&subject->head, &observer->node);

        DEBUG("subscribing success");
        DEBUG("num of subscriber [num = %d]",
              ezmLL_GetListSize(&subject->head));
        is_success = true;
    }
    else
    {
        WARNING("cannot subscribe - null pointer, or max subscribers reached");
    }

    return is_success;
}


/******************************************************************************
* Function : evntNoti_UnsubscribeEvent
*//**
* @Description:
*
* This function unsubcribes the observer a subject
*
* @param    *subject: (IN)pointer to the subject
* @param    *observer: (IN)publisher handle
* 
* @return   true success
*           false fail
*
* @Example Example:
* @code
* if(evntNoti_UnsubscribeEvent(&subject, &observer))
* {
*      printf("success");
* }
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
bool evntNoti_UnsubscribeEvent(event_subject* subject, event_observer* observer)
{
    TRACE("evntNoti_UnsubscribeEvent()");
    bool is_success = false;

    if (subject != NULL &&
        observer != NULL &&
        ezmLL_IsNodeInList(&subject->head, &observer->node))
    {
        EZMLL_UNLINK_NODE(&observer->node);

        DEBUG("unsubscribing success");
        DEBUG("num of subscriber [num = %d]",
               ezmLL_GetListSize(&subject->head));
        is_success = true;
    }
    else
    {
        WARNING("cannot unsubscribe - null pointer");
    }

    return is_success;
}


/******************************************************************************
* Function : evntNoti_NotifyEvent
*//**
* @Description:
*
* This function notifies the subscriber about an event has just been happening
*
* @param    *subject: (IN)pointer to the subject
* @param    event_code: (IN)event code
* @param    param1:     (IN)companion paramneter 1, set to NULL if unused
* @param    param1:     (IN)companion paramneter 2, set to NULL if unused
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
void evntNoti_NotifyEvent(event_subject* subject,
                          uint32_t event_code,
                          void* param1,
                          void* param2)
{
    TRACE("evntNoti_NotifyEnvent()");

    struct Node * iterate = NULL;
    struct event_observer* sub = NULL;

    if (subject != NULL)
    {
        EZMLL_FOR_EACH(iterate, &subject->head)
        {
            sub = EZMLL_GET_PARENT_OF(iterate, node, struct event_observer);
            if (sub->callback)
            {
                DEBUG("notify subscriber ");
                sub->callback(event_code, param1, param2);
            }
        }
    }
}

/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* (EVENT_NOTIFIER == 1U) */
/* End of file*/

