/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   module.h
 *  @brief  Header template for a module
 */


#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#if (CONFIG_STATEMACHINE == 1U)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief
 *
 */
typedef struct SM_StateMachine SM_StateMachine;


/** @brief
 *
 */
typedef uint8_t (*StateFunction)(SM_StateMachine * statemachine);


/** @brief
 *  
 */
typedef struct
{
    const char * pcStateName;   /**< */
    StateFunction StateHandle;  /**< */
    StateFunction StateEntry;   /**< */
    StateFunction StateExit;    /**< */
}SM_State;


/** @brief
 *  
 */
struct SM_StateMachine
{
    uint8_t current_event;      /**< */
    uint8_t current_state;      /**< */
    uint8_t last_state;         /**< */
    uint8_t num_of_state;       /**< */
    uint8_t num_of_event;       /**< */
    SM_State* state_table;      /**< */
    StateFunction ErrorHandler; /**< */
    void* data;                 /**< */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : sum
*//**
* \b Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void SM_Initialization(SM_StateMachine* statemachine,
                       uint8_t num_of_event,
                       uint8_t num_of_state,
                       SM_State *state_table,
                       StateFunction ErrorHandler,
                       void *statemachine_data);


/******************************************************************************
* Function : sum
*//**
* \b Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void SM_Execution(SM_StateMachine *statemachine);


/******************************************************************************
* Function : sum
*//**
* \b Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void SM_SetState(SM_StateMachine *statemachine, uint8_t new_state);


/******************************************************************************
* Function : sum
*//**
* \b Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void SM_SetEvent(SM_StateMachine* statemachine, uint8_t new_event);


/******************************************************************************
* Function : sum
*//**
* \b Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
*
*******************************************************************************/
void SM_SetData(SM_StateMachine* statemachine, void *new_data);

#endif /* CONFIG_STATEMACHINE */
#endif /* _STATE_MACHINE_H */

/* End of file*/
