/*****************************************************************************
* Filename:         main.c
* Author:           Hai Nguyen
* Original Date:    11.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   main.c
 *  @author Hai Nguyen
 *  @date   11.03.2024
 *  @brief  main.c file for linux target
 *
 *  @details
 */

/** @file   main.c
 *  @author Hai Nguyen
 *  @date   02.05.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include "ez_easy_embedded.h"
#include "tx_api.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define DEMO_STACK_SIZE 1024
#define DEMO_BYTE_POOL_SIZE 9120
#define DEMO_BLOCK_POOL_SIZE 100
#define DEMO_QUEUE_SIZE 100

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
TX_THREAD thread_0;
TX_THREAD thread_1;

/******************************************************************************
* Function Definitions
*******************************************************************************/
TX_BYTE_POOL byte_pool_0;
TX_BLOCK_POOL block_pool_0;

/******************************************************************************
* External functions
*******************************************************************************/
void thread_0_entry(ULONG thread_input);
void thread_1_entry(ULONG thread_input);
void tx_application_define(void *first_unused_memory);

void main(void)
{
    /* Enter the ThreadX kernel. */
    tx_kernel_enter();
}

/******************************************************************************
* Internal functions
*******************************************************************************/
/* Define what the initial system looks like. */
void tx_application_define(void *first_unused_memory)
{

    ezEasyEmbedded_Initialize();

    CHAR *pointer;

    /* Create a byte memory pool from which to allocate the thread stacks. */
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", first_unused_memory,
        DEMO_BYTE_POOL_SIZE);

    /* Put system definition stuff in here, e.g., thread creates and other assorted
        create information. */

    /* Allocate the stack for thread 0. */
    tx_byte_allocate(&byte_pool_0, &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create the main thread. */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
        pointer, DEMO_STACK_SIZE,
        1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

    /* Allocate the stack for thread 1. */
    tx_byte_allocate(&byte_pool_0, &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create threads 1 and 2. These threads pass information through a ThreadX
        message queue. It is also interesting to note that these threads have a time
        slice. */
    tx_thread_create(&thread_1, "thread 1", thread_1_entry, 1,
        pointer, DEMO_STACK_SIZE,
        16, 16, 4, TX_AUTO_START);
}


/* Define the test threads. */
void thread_0_entry(ULONG thread_input)
{
    UINT status;


    /* This thread simply sits in while-forever-sleep loop. */
    while(1)
    {
        printf("thread 0\n");
        /* Sleep for 10 ticks. */
        tx_thread_sleep(10);
    }
}


void thread_1_entry(ULONG thread_input)
{
    UINT status;


    /* This thread simply sends messages to a queue shared by thread 2. */
    while(1)
    {
        printf("thread 1\n");
        /* Sleep for 10 ticks. */
        tx_thread_sleep(50);
    }
}
/* End of file*/

