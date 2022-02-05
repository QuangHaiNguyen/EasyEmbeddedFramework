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

#if (KERNEL == 1U)
#include "string.h"
#include "../helper/hexdump/hexdump.h"
#include "../helper/linked_list/linked_list.h"
#include "../ezmDebug/ezmDebug.h"

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


/******************************************************************************
* Module Typedefs
*******************************************************************************/


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static LinkedList proccess_list = { 0 };
static uint32_t uuid = 0U;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
void ezmKernel_Initialization(void)
{
    /* Do nothing */
}

bool ezmKernel_AddProcess(process* proc, PROCESS_TYPE proc_type, uint32_t period_ms, process_handler handler)
{
    bool is_success = true;
    Node* node = NULL;

    process* free_proc = NULL;

    if (NULL == proc || NULL == handler || period_ms == 0U)
    {
        is_success = false;
    }

    if (is_success)
    {
        
        node = ezmLL_GetFreeNode();

        if (node)
        {
            node->pBuffer = (void*)proc;
            node->u16BufferSize = sizeof(process);

            LinkedList_InsertToTail(&proccess_list, node);

            proc->handler = handler;
            proc->period = period_ms;
            proc->exec_cnt_down = period_ms;
            proc->uuid = uuid++;
            proc->proc_type = proc_type;

            KERNELPRINT1("add process [id = %x] successfully", proc->uuid);
        }
        else
        {
            is_success = false;
        }
    }


    return is_success;
}

void ezmKernel_Run(void)
{
    Node *smallest_exec_node = NULL;
    Node *curr_node = proccess_list.pstHead;

    process *proc1 = NULL;
    process* proc2 = NULL;

    smallest_exec_node = curr_node;
 
    while (curr_node)
    {
        proc1 = (process*)smallest_exec_node->pBuffer;
        proc2 = (process*)curr_node->pBuffer;

        if (proc2->exec_cnt_down < proc1->exec_cnt_down)
        {
            smallest_exec_node = curr_node;
        }

        curr_node = curr_node->pstNextNode;
    }

    LinkedList_RemoveNode(&proccess_list, smallest_exec_node);
    LinkedList_InsertToHead(&proccess_list, smallest_exec_node);

    smallest_exec_node = proccess_list.pstHead;
    if (smallest_exec_node)
    {
        proc1 = (process*)smallest_exec_node->pBuffer;

        if (proc1)
        {
            if (proc1->exec_cnt_down <= 0)
            {
                proc1->handler();

                switch (proc1->proc_type)
                {
                case PROC_REPEATED:
                    proc1->exec_cnt_down = proc1->period;
                    break;

                case PROC_ONCE:
                default:
                    LinkedList_RemoveNode(&proccess_list, smallest_exec_node);
                    ezmLL_ResetNode(smallest_exec_node);
                    break;
                }
            }
            
        }
    }
}

void ezmKernel_Clock(void)
{
    Node* next_node = proccess_list.pstHead;
    process* proc = NULL;
    while (next_node)
    {
        proc = (process*)next_node->pBuffer;
        if (proc->exec_cnt_down > INT32_MIN)
        {
            proc->exec_cnt_down--;
        }
        next_node = next_node->pstNextNode;
    }
}
#endif /* CLI */

/* End of file*/