/*******************************************************************************
* Title                 :   cli 
* Filename              :   cli.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.12.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  cli.c
 *  @brief This is the source code for a cli
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmKernel.h"

#if (CONFIG_KERNEL == 1U)
#include "string.h"
#include "ezUtilities/hexdump/hexdump.h"
#include "ezApp/ezmDebug/ezmDebug.h"

#define MOD_NAME        "KERNEL"


#if (MODULE_DEBUG == 1U) && (KERNEL_DEBUG == 1U)
#define KERNELPRINT(a)                     PRINT_DEBUG(MOD_NAME,a)
#define KERNELPRINT1(a,b)                  PRINT_DEBUG1(MOD_NAME,a,b)
#define KERNELPRINT2(a,b,c)                PRINT_DEBUG2(MOD_NAME,a,b,c)
#define KERNELPRINT3(a,b,c,d)              PRINT_DEBUG3(MOD_NAME,a,b,c,d)
#define KERNELPRINT4(a,b,c,d,e)            PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#else 
#define KERNELPRINT(a)
#define KERNELPRINT(a,b)
#define KERNELPRINT2(a,b,c)
#define KERNELPRINT3(a,b,c,d)
#define KERNELPRINT4(a,b,c,d,e)
#endif

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define SAMPLING_TIME_MS        10000U
#define PROCESS(proc)           ((struct Process*)proc)
#define GET_PROCESS(node_ptr)   (EZMLL_GET_PARENT_OF(node_ptr, node, struct Process))
/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct ProcessAnalysis
{
    uint32_t sampling_time;
    uint32_t proc_run_time;
    uint32_t before;
    uint32_t after;
    uint8_t load;
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct Node proc_list_head = EZMLL_INIT_NODE(proc_list_head);
static struct ProcessAnalysis kernel_load = {0};
static int current_tick = 0U;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : ezmKernel_AddProcess
*//**
* \b Description:
*
* add a process to the kernel
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *proc: (IN)pointer to the new process
* @param    proc_type: (IN)type of the process
* @param    period_ms: (IN)period
* @param    handler: (IN)pointer to the process function
* @return   true if suceess
*
*******************************************************************************/
bool ezmKernel_AddProcess(EzmProcess* proc, PROCESS_TYPE proc_type, uint32_t period_ms, process_handler handler)
{
    bool is_success = false;

    if (NULL != proc && NULL != handler && period_ms > 0U)
    {
        EZMLL_ADD_TAIL(&proc_list_head, &PROCESS(proc)->node);

        PROCESS(proc)->handler = handler;
        PROCESS(proc)->period = period_ms;
        PROCESS(proc)->exec_cnt_down = period_ms;
        PROCESS(proc)->proc_type = proc_type;
        is_success = true;
        KERNELPRINT("add process successfully");
    }

    return is_success;
}

/******************************************************************************
* Function : ezmKernel_RemoveProcess
*//**
* \b Description:
*
* renove a process from the kernel
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *proc: (IN)pointer to the new process
* @return   true if suceess
*
*******************************************************************************/
bool ezmKernel_RemoveProcess(EzmProcess* proc)
{
    bool is_success = false;
    if(proc)
    {
        EZMLL_UNLINK_NODE(&PROCESS(proc)->node);
        is_success = true;
    }
    return is_success;
}

/******************************************************************************
* Function : ezmKernel_Run
*//**
* \b Description:
*
* run the kernel, must be put in a while loop
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezmKernel_Run(void)
{
    struct Node* smallest_exe_node = NULL;
    struct Node* it_node = NULL;

    smallest_exe_node = proc_list_head.next;

    if (smallest_exe_node != &proc_list_head)
    {
        EZMLL_FOR_EACH(it_node, &proc_list_head)
        {
            if (GET_PROCESS(it_node)->exec_cnt_down < GET_PROCESS(smallest_exe_node)->exec_cnt_down)
            {
                smallest_exe_node = it_node;
            }
        }

        EZMLL_UNLINK_NODE(smallest_exe_node);
        EZMLL_ADD_HEAD(&proc_list_head, smallest_exe_node);

        if (GET_PROCESS(smallest_exe_node)->exec_cnt_down <= 0)
        {
            kernel_load.before = kernel_load.sampling_time;
            GET_PROCESS(smallest_exe_node)->handler();
            kernel_load.after = kernel_load.sampling_time;
            kernel_load.proc_run_time = kernel_load.proc_run_time + (kernel_load.after - kernel_load.before);

            switch (GET_PROCESS(smallest_exe_node)->proc_type)
            {
            case PROC_REPEATED:
                GET_PROCESS(smallest_exe_node)->exec_cnt_down = GET_PROCESS(smallest_exe_node)->period;
                break;

            case PROC_ONCE:
            default:
                EZMLL_UNLINK_NODE(smallest_exe_node);
                break;
            }
        }
    }

    if (SAMPLING_TIME_MS == kernel_load.sampling_time)
    {
        kernel_load.load = (uint8_t)((kernel_load.proc_run_time / kernel_load.sampling_time) * 100);
        kernel_load.proc_run_time = 0;
        kernel_load.sampling_time = 0;
    }
}

/******************************************************************************
* Function : ezmKernel_UpdateClock
*//**
* \b Description:
*
* update the counter of the process, must be called in a 1ms timer inerrupt
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezmKernel_UpdateClock(void)
{
    struct Node* it_node = NULL;
    EZMLL_FOR_EACH(it_node, &proc_list_head)
    {
        if (GET_PROCESS(it_node)->exec_cnt_down > INT32_MIN)
        {
            GET_PROCESS(it_node)->exec_cnt_down--;
        }
    }
    kernel_load.sampling_time++;
    current_tick++;
}


/******************************************************************************
* Function : ezmKernel_GetTick
*//**
* \b Description:
*
* return the current tick of the system
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   current tick in millisecond
*
*******************************************************************************/
int ezmKernel_GetTickMillis(void)
{
    return current_tick;
}

/******************************************************************************
* Function : ezmKernel_GetLoad
*//**
* \b Description:
*
* Get the cpu load, not sure if it work
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   cpu load in percentage
*
*******************************************************************************/
uint8_t ezmKernel_GetLoad(void)
{
    return kernel_load.load;
}

/******************************************************************************
* Function : ezmKernel_GetNumOfActiveProc
*//**
* \b Description:
*
* Get the number of active process
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   number of active process
*
*******************************************************************************/
int ezmKernel_GetNumOfActiveProc(void)
{
    return ezmLL_GetListSize(&proc_list_head);
}
#endif /* CLI */

/* End of file*/