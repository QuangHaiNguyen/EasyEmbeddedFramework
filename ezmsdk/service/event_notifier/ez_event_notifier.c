
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
#include "ez_event_notifier.h"


#define DEBUG_LVL   LVL_NO_LOG      /**< logging level */
#define MOD_NAME    "EVENT_NOTIFY"  /**< module name */

#if(EVENT_NOTIFIER == 1U)

#include "ezUtilities/logging/logging.h"
#include "ezUtilities/ezmAssert/ezmAssert.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
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
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

ezSTATUS ezEventNotifier_CreateSubject(ezSubject *subject)
{
    DEBUG("evntNoti_CreateSubject()");
    ezSTATUS status = ezFAIL;

    if (subject)
    {
        ezmLL_InitNode(subject);
        status = ezSUCCESS;
        DEBUG("  Create OK");
    }
    else
    {
        ERROR("  Cannot create subject, invalid argument");
    }

    return status;
}


void ezEventNotifier_ResetSubject(ezSubject * subject)
{
    if (subject)
    {
        ezmLL_InitNode(subject);
    }
}


ezSTATUS ezEventNotifier_CreateObserver(ezObserver *observer,
                                        EVENT_CALLBACK callback)
{
    DEBUG("evntNoti_CreateObserver()");
    ezSTATUS status = ezFAIL;

    if (observer != NULL && callback != NULL)
    {
        observer->callback = callback;
        status = ezSUCCESS;
        DEBUG("  Create Observer OK");
    }
    else
    {
        ERROR("  Cannot create observer, invalid argument");
    }

    return status;
}


ezSTATUS ezEventNotifier_SubscribeToSubject(ezSubject *subject,
                                            ezObserver *observer)
{
    DEBUG("evntNoti_SubscribeEvent()");
    ezSTATUS status = ezFAIL;

    if (subject != NULL
        && observer != NULL)
    {
        EZMLL_ADD_HEAD(subject, &observer->node);

        DEBUG("  subscribing success");
        DEBUG("  num of subscriber [num = %d]", ezmLL_GetListSize(subject));
        status = ezSUCCESS;
    }
    else
    {
        WARNING("  cannot subscribe - null pointer, or max subscribers reached");
    }

    return status;
}


ezSTATUS ezEventNotifier_UnsubscribeFromSubject(ezSubject *subject,
                                                ezObserver *observer)
{
    DEBUG("evntNoti_UnsubscribeEvent()");
    ezSTATUS status = ezFAIL;

    if (subject != NULL &&
        observer != NULL &&
        ezmLL_IsNodeInList(subject, &observer->node))
    {
        EZMLL_UNLINK_NODE(&observer->node);

        DEBUG("  unsubscribing success");
        DEBUG("  num of subscriber [num = %d]", ezmLL_GetListSize(subject));
        status = ezSUCCESS;
    }
    else
    {
        WARNING("  cannot unsubscribe - null pointer");
    }

    return status;
}


uint16_t ezEventNotifier_GetNumOfObservers(ezSubject *subject)
{
    uint16_t num_of_observers = 0;

    DEBUG("ezEventNotifier_GetNumOfObservers()");

    if (subject)
    {
        num_of_observers = ezmLL_GetListSize(subject);
        DEBUG("  num of observer = %d", num_of_observers);
    }
    else
    {
        WARNING("  Invalid argument");
    }
    return num_of_observers;
}


void ezEventNotifier_NotifyEvent(ezSubject *subject,
                                 uint32_t event_code,
                                 void *param1,
                                 void *param2)
{
    DEBUG("evntNoti_NotifyEnvent()");

    struct Node *iterate = NULL;
    struct ezObserver *sub = NULL;

    if (subject != NULL)
    {
        EZMLL_FOR_EACH(iterate, subject)
        {
            sub = EZMLL_GET_PARENT_OF(iterate, node, struct ezObserver);
            if (sub->callback)
            {
                DEBUG("  notify observer");
                sub->callback(event_code, param1, param2);
            }
        }
    }
    else
    {
        WARNING("  Invalid argument");
    }
}

/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* (CONFIG_EVENT_NOTIFIER == 1U) */
/* End of file*/

