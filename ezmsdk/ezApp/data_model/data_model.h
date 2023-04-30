
/*******************************************************************************
* Filename:         data_model.h
* Author:           Hai Nguyen
* Original Date:    19.06.2022
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
* Written by Hai Nguyen 19.06.2022
*
*******************************************************************************/

/** @file   data_model.h
 *  @author Hai Nguyen
 *  @date   19.06.2022
 *  @brief  This is the header file of Data Model module
 *
 *  @details It contains the public API, which will be used by the users
 * 
 */

#ifndef _DATA_MODEL_H
#define _DATA_MODEL_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if (DATA_MODEL == 1U)

#include <stdbool.h>
#include <stdint.h>

#include "ez_event_notifier.h"
#include "ezUtilities/linked_list/linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define DATA_POINT_INVALID      UINT32_MAX /**< */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef uint32_t DataPoint;

typedef enum
{
    DATA_MODIFY,
}DATA_POINT_EVENT;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool DataModel_Initialization(void);

DataPoint DataModel_CreateDataPoint(void* data,
                                    uint32_t size);

void DataModel_ReleaseDataPoint(DataPoint data_point);

bool DataModel_WriteDataPoint(DataPoint data_point,
                              void* data,
                              uint32_t size);

bool DataModel_ReadDataPoint(DataPoint data_point,
                             void * data,
                             uint32_t size);

bool DataModel_SubscribeDataPointEvent(DataPoint data_point,
                                        ezObserver *observer);

bool DataModel_UnsubscribeDataPointEvent(DataPoint data_point,
     ezObserver *observer);

#endif /* CONFIG_DATA_MODEL == 1U */

#endif /* _DATA_MODEL_H */

/* End of file */
