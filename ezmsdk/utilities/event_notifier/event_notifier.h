
/*******************************************************************************
* Filename:         event_notifier.h
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
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief
 *
 */
typedef int (*EVENT_CALLBACK)(uint32_t event_code, void * param1, void * param2);

/** @brief
 *
 */
typedef uint32_t evnt_pub;

/** @brief
 *
 */
typedef uint32_t evnt_sub;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void     evntNoti_Initialize        (void);

evnt_pub evntNoti_CreatePublisher   (uint32_t num_of_allow_sub);

evnt_sub evntNoti_SubscribeEvent    (evnt_pub pub_handle, 
                                     EVENT_CALLBACK callback);

bool     evntNoti_UnsubscribeEvent  (evnt_pub pub_handle, evnt_sub sub_handle);

void     evntNoti_NotifyEnvent(evnt_pub pub_handle,
                               uint32_t event_code,
                               void* param1,
                               void* param2);

#endif /* (EVENT_NOTIFIER == 1U) */

#endif /* _EVENT_NOTIFIER_H */

/* End of file */

