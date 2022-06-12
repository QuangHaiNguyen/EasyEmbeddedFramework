
/*******************************************************************************
* Filename:         event_notifier.c
* Author:           Hai Nguyen
* Original Date:    08.06.2022
* Last Update:      08.06.2022
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


#define DEBUG_LVL   LVL_ERROR       /**< logging level */
#define MOD_NAME    "EVENT_NOTI"    /**< module name */

#if(EVENT_NOTIFIER == 1U)


#include "utilities/logging/logging.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NUM_EVENT_PUB   10U     /* Maximum number of event publisher */
#define NUM_EVENT_SUB   32U     /* Maximum number of event publisher */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief structure to manage the data of the publisher
 *
 */
struct event_publisher
{
    bool is_avail;
    uint32_t num_of_subcriber;  /**< number of allowed subcriber*/
    struct Node head;           /**< head of the subcriber list */
};


/** @brief structure to manage the data of the publisher
 *
 */
struct event_subscriber
{
    bool is_avail;              /**< available flag */
    struct Node node;           /**< link list node */
    EVENT_CALLBACK callback;    /**< event call back function */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct event_publisher   pub_pool[NUM_EVENT_PUB] = { 0 };
static struct event_subscriber  sub_pool[NUM_EVENT_SUB] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : evntNoti_Initialize
*//**
* @Description:
*
* This function initializes the event notifier module
*
* @param    None
* @return   None
*
* @Example Example:
* @code
* evntNoti_Initialize();
* @endcode
*
*******************************************************************************/
void evntNoti_Initialize(void)
{
    TRACE("evntNoti_Initialize()");
    DEBUG("[num of pub = %d]", NUM_EVENT_PUB);
    DEBUG("[num of sub = %d]", NUM_EVENT_SUB);

    for (uint16_t i = 0; i < NUM_EVENT_PUB; i++)
    {
        pub_pool[i].is_avail = true;
        pub_pool[i].num_of_subcriber = 0U;
        ezmLL_InitNode(&pub_pool[i].head);
    }

    for (uint16_t i = 0; i < NUM_EVENT_SUB; i++)
    {
        sub_pool[i].is_avail = true;
        sub_pool[i].callback = NULL;
        ezmLL_InitNode(&sub_pool[i].node);
    }
}

/******************************************************************************
* Function : evntNoti_CreatePublisher
*//** 
* @Description:
*
* This function creates a publisher. It looks for an availalbe publisher in the pool
* 
* @param    num_of_allow_sub: (IN)number of supported subscriber
* @return   handle to the publisher
*
* @Example Example:
* @code
* evnt_pub my_pub = evntNoti_CreatePublisher(10);
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
evnt_pub evntNoti_CreatePublisher(uint32_t num_of_allow_sub)
{
    TRACE("evntNoti_CreatePublisher()");
    evnt_pub ret_pub = UINT32_MAX;

    for (uint16_t i = 0; i < NUM_EVENT_PUB; i++)
    {
        if (pub_pool[i].is_avail)
        {
            pub_pool[i].is_avail = false;
            pub_pool[i].num_of_subcriber = num_of_allow_sub;
            ret_pub = i;

            DEBUG("get an instance [index = %d]", i);
            DEBUG("[num of sub = %d]", pub_pool[i].num_of_subcriber);
            break;
        }
    }

    if (ret_pub == UINT32_MAX)
    {
        ERROR("Do not have enough event_publisher instance");
    }

    return ret_pub;
}


/******************************************************************************
* Function : evntNoti_SubscribeEvent
*//**
* @Description:
*
* This function subcribes the subcriber to an event
*
* @param    pub_handle: (IN)publisher handle
* @param    callback:   (IN)callback function when an event is receive
*
* @return   subscriber handle if success
*           UINT32_MAX if fail
*
* @Example Example:
* @code
* evnt_sub my_sub = evntNoti_SubscribeEvent(my_pub, event_handler);
* if(evnt_sub != UINT32_MAX)
* {
*     printf("success");
* }
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
evnt_sub evntNoti_SubscribeEvent(evnt_pub pub_handle, EVENT_CALLBACK callback)
{
    TRACE("evntNoti_SubscribeEvent()");
    evnt_sub ret_sub = UINT32_MAX;

    if (pub_handle < NUM_EVENT_PUB &&
        !pub_pool[pub_handle].is_avail &&
        ezmLL_GetListSize(&pub_pool[pub_handle].head) < pub_pool[pub_handle].num_of_subcriber)
    {
        for (uint16_t i = 0; i < NUM_EVENT_SUB; i++)
        {
            if (sub_pool[i].is_avail)
            {
                sub_pool[i].is_avail = false;
                sub_pool[i].callback = callback;
                
                EZMLL_ADD_HEAD(&pub_pool[pub_handle].head, &sub_pool[i].node);

                ret_sub = i;
                DEBUG("subscribe [index = %d]", ret_sub);
                DEBUG("num of subscriber [num = %d]",
                      ezmLL_GetListSize(&pub_pool[pub_handle].head));
                break;
            }
        }
    }
    else
    {
        WARNING("cannot subscribe - illegal params, or max subscribers reached");
    }

    return ret_sub;
}


/******************************************************************************
* Function : evntNoti_UnsubscribeEvent
*//**
* @Description:
*
* This function unsubcribes the subcriber fr0m an event
*
* @param    pub_handle: (IN)publisher handle
* @param    sub_handle: (IN)subscriber handle
* 
* @return   true success
*           false fail
*
* @Example Example:
* @code
* if(evntNoti_UnsubscribeEvent(my_pub, my_sub))
* {
*      printf("success");
* }
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
bool evntNoti_UnsubscribeEvent(evnt_pub pub_handle, evnt_sub sub_handle)
{
    TRACE("evntNoti_UnsubscribeEvent()");
    bool is_success = false;

    if (pub_handle < NUM_EVENT_PUB &&
        sub_handle < NUM_EVENT_SUB &&
        !pub_pool[pub_handle].is_avail &&
        !sub_pool[sub_handle].is_avail)
    {
        if (ezmLL_IsNodeInList(&pub_pool[pub_handle].head,
                               &sub_pool[sub_handle].node))
        {
            EZMLL_UNLINK_NODE(&sub_pool[sub_handle].node);
            sub_pool[sub_handle].is_avail = true;
            sub_pool[sub_handle].callback = NULL;

            DEBUG("unsubscribe [index = %d]", sub_handle);
            DEBUG("num of subscriber [num = %d]",
                  ezmLL_GetListSize(&pub_pool[pub_handle].head));
            is_success = true;
        }
    }
    else
    {
        WARNING("cannot subscribe - illegal params, or max subscribers reached");
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
* @param    pub_handle: (IN)publisher handle
* @param    event_code: (IN)event code
* @param    param1:     (IN)companion paramneter 1, set to NULL if unused
* @param    param1:     (IN)companion paramneter 2, set to NULL if unused
*
* @return   None
*
* @Example Example:
* @code
* evntNoti_NotifyEnvent(my_pub, ENUM_ERROR_CODE, NULL, NULL);
* @endcode
*
* @see module must be initialized
*
*******************************************************************************/
void evntNoti_NotifyEvent( evnt_pub pub_handle,
                            uint32_t event_code,
                            void* param1,
                            void* param2)
{
    TRACE("evntNoti_NotifyEnvent()");

    struct Node * iterate = NULL;
    struct event_subscriber * sub = NULL;

    if (pub_handle < NUM_EVENT_PUB && !pub_pool[pub_handle].is_avail)
    {
        EZMLL_FOR_EACH(iterate, &pub_pool[pub_handle].head)
        {
            sub = EZMLL_GET_PARENT_OF(iterate, node, struct event_subscriber);
            if (sub->callback)
            {
                DEBUG("notify subscriber [index = %d]",
                      ((uint32_t)sub - (uint32_t)&sub_pool[0]) / sizeof(struct event_subscriber));

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

