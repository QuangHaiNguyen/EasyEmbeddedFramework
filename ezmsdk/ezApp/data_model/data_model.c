
/*******************************************************************************
* Filename:         data_model.c
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

/** @file   data_model.c
 *  @author Hai Nguyen
 *  @date   19.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/


#if (DATA_MODEL == 1U)

#define DEBUG_LVL   LVL_DEBUG       /**< logging level */
#define MOD_NAME    "DATA_MODEL"    /**< module name */

#include <string.h>
#include "utilities/logging/ez_logging.h"
#include "data_model.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NUM_OF_DATA_ELEMENT     10   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/** @brief definition of a new type
 *
 */
struct DataModelElement
{
    bool            is_avail;
    bool            is_locked;
    uint32_t        data_size;
    void            *data;
    ezSubject       subject;
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct DataModelElement data_model[NUM_OF_DATA_ELEMENT] = {0};

#if (DEBUG_LVL > LVL_INFO)
static uint32_t data_point_count = 0U;
#endif
/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : DataModel_Initialization
*//** 
* @Description:
*
* This function initializes the Data model module
* 
* @param    None
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* DataModel_Initialization();
* @endcode
*
*******************************************************************************/
bool DataModel_Initialization(void)
{
    bool is_success = true;

    EZTRACE("DataModel_Initialization");

    for (uint32_t i = 0; i < NUM_OF_DATA_ELEMENT; i++)
    {
        data_model[i].is_avail = true;
        data_model[i].data = NULL;
        data_model[i].data_size = 0;
        data_model[i].is_locked = false;

        if (ezEventNotifier_CreateSubject(&data_model[i].subject) == ezFAIL)
        {
            EZERROR("init failed");
            is_success = false;
            break;
        }
    }

    return is_success;
}

/******************************************************************************
* Function : DataModel_ReleaseDataPoint
*//**
* @Description:
*
* This function releases and reset the data point when it is unused
*
* @param    data_point: (IN)data point handle
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* DataModel_ReleaseDataPoint(my_data_point);
* @endcode
*
*******************************************************************************/
void DataModel_ReleaseDataPoint(DataPoint data_point)
{
    if (data_point < NUM_OF_DATA_ELEMENT)
    {
        data_model[data_point].data = NULL;
        data_model[data_point].data_size = 0;
        data_model[data_point].is_avail = true;
        data_model[data_point].is_locked = false;
        ezEventNotifier_ResetSubject(&data_model[data_point].subject);

#if (DEBUG_LVL > LVL_INFO)
        data_point_count--;
        EZDEBUG("num of data point [size = %lu]", data_point_count);
#endif
    }
}

/******************************************************************************
* Function : DataModel_CreateDataPoint
*//**
* @Description:
*
* This function creates a data point by getting a free data point from
* data_model, and it init the data point according to the arguments
*
* @param    *data: (IN)pointer to the actual data
* @param    size: (IN)size of data, for sanity checking and future use
* @return   DataPoint handle or DATA_POINT_INVALID
*
* @Example Example:
* @code
* uint32_t my_data = 100;
* DataPoint my_data_point = DataModel_CreateDataPoint(&my_data, sizeof(uint32_t));
* if(my_data_point != DATA_POINT_INVALID)
* {
*     printf("success");
* }
* @endcode
*
*******************************************************************************/
DataPoint DataModel_CreateDataPoint(void * data,
                                    uint32_t size)
{
    EZTRACE("DataModel_CreateDataElement()");

    bool is_success = true;
    DataPoint data_point = DATA_POINT_INVALID;

    for (uint32_t i = 0; i < NUM_OF_DATA_ELEMENT; i++)
    {
        if (data_model[i].is_avail)
        {
            data_point = i;
            data_model[i].is_avail = false;
            data_model[i].data = data;
            data_model[i].data_size = size;

            EZDEBUG("found free data element [index = %lu]", i);

#if (DEBUG_LVL > LVL_INFO)
            data_point_count++;
            EZDEBUG("num of data point [size = %lu]", data_point_count);
#endif
            break;
        }
    }

    if (!is_success)
    {
        EZERROR("Do not have enough data point");
    }

    return data_point;
}


/******************************************************************************
* Function : DataModel_WriteDataPoint
*//**
* @Description:
*
* This function write new data to a data point
*
* @param    data_point: (IN)data point handle, where data is written to
* @param    *data: (IN)pointer to the written data
* @param    size: (IN)size of data, for sanity checking and future use
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* uint32_t my_data = 100;
* uint32_t new_data = 200;
* 
* DataPoint my_data_point = DataModel_CreateDataPoint(&my_data, sizeof(uint32_t));
* 
* if(my_data_point != DATA_POINT_INVALID)
* {
*     printf("success");
*    DataModel_WriteDataPoint(my_data_point, &new_data, sizeof(uint32_t));
* }
* @endcode
*
*******************************************************************************/
bool DataModel_WriteDataPoint(DataPoint data_point,
                              void* data,
                              uint32_t size)
{
    EZTRACE("DataModel_WriteDataPoint()");

    bool is_success = false;

    if (data_point < NUM_OF_DATA_ELEMENT &&
        data_model[data_point].is_avail == false &&
        data != NULL)
    {
        EZTRACE("params sanity check OK");

        if (size == data_model[data_point].data_size &&
            data_model[data_point].is_locked == false)
        {
            EZTRACE("internal data check OK");

#if (DEBUG_LVL > LVL_DEBUG)
            HEXDUMP(data, size);
#endif /* (DEBUG_LVL > LVL_DEBUG) */

            data_model[data_point].is_locked = true;

            if (memcmp(data_model[data_point].data, data, size) != 0)
            {
                EZTRACE("data @ [index = %lu] has changed", data_point);

                memcpy(data_model[data_point].data, data, size);
                ezEventNotifier_NotifyEvent(&data_model[data_point].subject,
                                            DATA_MODIFY,
                                            data_model[data_point].data,
                                            &data_model[data_point].data_size);
            }
            else
            {
                EZTRACE("data unchanged");
            }

            data_model[data_point].is_locked = false;

            is_success = true;
        }
    }

    return is_success;
}

/******************************************************************************
* Function : DataModel_WriteDataPoint
*//**
* @Description:
*
* This function reads data from a data point
*
* @param    data_point: (IN)data point handle, where data is read to
* @param    *data: (IN)pointer to the read data
* @param    size: (IN)size of data, for sanity checking and future use
* @return   true: success
*           false: fail
*
* @Example Example:
* @code
* uint32_t my_data = 100;
* uint32_t new_data;
*
* DataPoint my_data_point = DataModel_CreateDataPoint(&my_data, sizeof(uint32_t));
*
* if(my_data_point != DATA_POINT_INVALID)
* {
*     printf("success");
*    DataModel_ReadDataPoint(my_data_point, &new_data, sizeof(uint32_t));
* }
* @endcode
*
*******************************************************************************/
bool DataModel_ReadDataPoint(DataPoint data_point,
                             void* data,
                             uint32_t size)
{
    EZTRACE("DataModel_ReadDataElement()");

    bool is_success = false;

    if (data_point < NUM_OF_DATA_ELEMENT &&
        data_model[data_point].is_avail == false &&
        data != NULL)
    {
        if (size == data_model[data_point].data_size &&
            data_model[data_point].is_locked == false)
        {
            EZTRACE("Read data @ [index = %lu]", data_point);

            data_model[data_point].is_locked = true;

            memcpy(data_model[data_point].data, data, size);

            data_model[data_point].is_locked = false;

            is_success = true;
        }
    }
    return is_success;
}

/******************************************************************************
* Function : DataModel_SubscribeDataPointEvent
*//**
* @Description:
*
* This function allow a module to receive data change event from the data model
*
* @param    data_point: (IN)data point handle, where data is read to
* @param    *observer: (IN)pointer to the observer
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool DataModel_SubscribeDataPointEvent(DataPoint data_point,
    ezObserver * observer)
{
    EZTRACE("DataModel_SubscribeDataPointEvent()");

    bool is_success = false;

    if (data_point < NUM_OF_DATA_ELEMENT &&
        data_model[data_point].is_avail == false &&
        observer != NULL)
    {
        is_success = (ezEventNotifier_SubscribeToSubject(&data_model[data_point].subject, observer) == ezSUCCESS);
    }
    return is_success;
}

/******************************************************************************
* Function : DataModel_UnsubscribeDataPointEvent
*//**
* @Description:
*
* This function allow a module to stop receiving data change event from the data model
*
* @param    data_point: (IN)data point handle, where data is read to
* @param    *observer: (IN)pointer to the observer
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool DataModel_UnsubscribeDataPointEvent(DataPoint data_point,
    ezObserver * observer)
{
    EZTRACE("DataModel_UnsubscribeDataPointEvent()");

    bool is_success = false;

    if (data_point < NUM_OF_DATA_ELEMENT &&
        data_model[data_point].is_avail == false &&
        observer != NULL)
    {

        is_success = (ezEventNotifier_UnsubscribeFromSubject(&data_model[data_point].subject, observer) == ezSUCCESS);
    }
    return is_success;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* (CONFIG_DATA_MODEL == 1U) */

/* End of file*/
