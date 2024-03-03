/*******************************************************************************
* Title                 :   statemachine 
* Filename              :   statemachine.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  statemachine.c
 *  @brief This is the source template for a statemachine
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "statemachine.h"

#if ( CONFIG_STATEMACHINE == 1U )

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "STATEMACHINE"  /**< module name */

#include "ezUtilities/logging/logging.h"

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


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

void SM_Execution(SM_StateMachine *statemachine)
{
    uint8_t current_state = statemachine->current_state;

    if(statemachine->current_event < statemachine->num_of_event 
        && current_state < statemachine->num_of_state)
    {
        if(statemachine->state_table[current_state].StateEntry != NULL)
        {
            TRACE("Enter state: %s", statemachine->state_table[current_state].pcStateName);
            statemachine->state_table[current_state].StateEntry(statemachine);
        }

        TRACE("Handling...");
        if (statemachine->state_table[current_state].StateHandle != NULL)
        {
            statemachine->current_state = statemachine->state_table[current_state].StateHandle(statemachine);
            TRACE("Next state %d", statemachine->current_state);
        }
        else
        {
            ERROR("no handling function");
            statemachine->ErrorHandler(statemachine);
        }
        

        if(statemachine->state_table[current_state].StateExit != NULL)
        {
            statemachine->state_table[current_state].StateExit(statemachine);
            TRACE("Leave state: %s", statemachine->state_table[current_state].pcStateName);
        }

        statemachine->last_state = current_state;
    }
    else
    {
        ERROR("unknown event");
        statemachine->ErrorHandler(statemachine);
    }
}

void SM_Initialization(SM_StateMachine *statemachine,
                       uint8_t num_of_event,
                       uint8_t num_of_state,
                       SM_State *state_table,
                       StateFunction ErrorHandler,
                       void *statemachine_data)
{

    /**statemachine_data can be NULL be cause we dont need to pass around data
     * in every use case
     */

    if (statemachine != NULL
        && state_table != NULL
        && ErrorHandler != NULL)
    {
        statemachine->num_of_event = num_of_event;
        statemachine->num_of_state = num_of_state;
        statemachine->state_table = state_table;
        statemachine->ErrorHandler = ErrorHandler;
        statemachine->data = statemachine_data;
    }
}


void SM_SetState(SM_StateMachine *statemachine, uint8_t new_state)
{
    if (statemachine)
    {
        statemachine->current_state = new_state;
    }
}

void SM_SetEvent(SM_StateMachine* statemachine, uint8_t new_event)
{
    if (statemachine)
    {
        statemachine->current_event = new_event;
    }
}

void SM_SetData(SM_StateMachine* statemachine, void* new_data)
{
    if (statemachine)
    {
        statemachine->data = new_data;
    }
}


/******************************************************************************
* Internal functions
*******************************************************************************/
/* None */

#endif /* STATEMACHINE */

/* End of file ****************************************************************/
