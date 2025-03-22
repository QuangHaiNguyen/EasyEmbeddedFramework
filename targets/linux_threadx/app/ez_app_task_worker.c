/*****************************************************************************
* Filename:         ez_app_task_worker.c
* Author:           Hai Nguyen
* Original Date:    16.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_app_task_worker.c
 *  @author Hai Nguyen
 *  @date   16.03.2025
 *  @brief  Application to test Task Worker component
 *
 *  @details
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_app_task_worker.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_app_task_worker"       /**< module name */
#include "ez_logging.h"
#include "ez_easy_embedded.h"
#include "ez_task_worker.h"
#include "ez_threadx_port.h"


/*the rest of include go here*/

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define BUFF_SIZE   256
#define PRIORITY    10
#define STACK_SIZE  512

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
typedef struct
{
    int a;
    int b;
}Worker1_SumContext;

static struct ezTaskWorkerThreadInterfaces *threadx_interfaces = NULL;
static INIT_WORKER(worker1, 0, PRIORITY, STACK_SIZE);
static INIT_WORKER(worker2, 200, PRIORITY, STACK_SIZE);
static uint8_t buff2[BUFF_SIZE] = {0};
static uint8_t buff1[BUFF_SIZE] = {0};

/******************************************************************************
* Function Definitions
*******************************************************************************/
INIT_THREAD_FUNCTIONS(worker1);
INIT_THREAD_FUNCTIONS(worker2);

static bool worker1_sum(int a, int b,  ezTaskWorkerCallbackFunc callback);
static bool worker1_sum_intern(void *context, ezTaskWorkerCallbackFunc callback);
static void worker2_callback(uint8_t event, void *ret_data);

/******************************************************************************
* External functions
*******************************************************************************/


/******************************************************************************
* Internal functions
*******************************************************************************/
/* Define what the initial system looks like. */
#if 0
void tx_application_define(void *first_unused_memory)
{
    bool ret = false;

    ezEasyEmbedded_Initialize();
    ezThreadXPort_Init(first_unused_memory);
    threadx_interfaces = ezThreadXPort_GetInterface();
    ret = (threadx_interfaces != NULL);
    if(ret == true)
    {
        ret = ezTaskWorker_SetRtosInterface(threadx_interfaces);
        if(ret == false)
        {
            EZERROR("Set interface failed");
        }
    }

    if(ret == true)
    {
        ret = ezTaskWorker_CreateWorker(&worker1,
                                        buff1,
                                        BUFF_SIZE,
                                        GET_THREAD_FUNC(worker1));

        ret &= ezTaskWorker_CreateWorker(&worker2,
                                         buff2,
                                         BUFF_SIZE,
                                         GET_THREAD_FUNC(worker2));
    }

    tx_kernel_enter();
}
#endif

THREAD_FUNC(worker1)
{
    ezTaskWorker_ExecuteTask(&worker1, EZ_THREAD_WAIT_FOREVER);
}

THREAD_FUNC(worker2)
{
    bool ret = worker1_sum(rand() % 255, rand() % 255, worker2_callback);
    if(ret == true)
    {
        EZINFO("Call sum service success");
    }
}


static bool worker1_sum(int a, int b, ezTaskWorkerCallbackFunc callback)
{
    bool ret = false;
    Worker1_SumContext context;
    context.a = a;
    context.b = b;

    ret = ezTaskWorker_EnqueueTask(&worker1,
                                   worker1_sum_intern,
                                   callback,
                                   (void*)&context,
                                   sizeof(context),
                                   EZ_THREAD_WAIT_FOREVER);
    return ret;
}

static bool worker1_sum_intern(void *context, ezTaskWorkerCallbackFunc callback)
{
    bool ret = false;
    int sum = 0;
    Worker1_SumContext *sum_context = (Worker1_SumContext *)context;
    if(sum_context != NULL && callback != NULL)
    {
        sum = sum_context->a + sum_context->b;
        callback(0, &sum);
        ret = true;
    }

    return true;
}

static void worker2_callback(uint8_t event, void *ret_data)
{
    switch (event)
    {
    case 0:
        if(ret_data != NULL)
        {
            EZINFO("sum = %d", *(int*)ret_data);
        }
        break;
    
    default:
        break;
    }
}

/* End of file*/
