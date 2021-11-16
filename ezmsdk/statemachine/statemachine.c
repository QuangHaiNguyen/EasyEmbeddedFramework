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
#include "../app/app_config.h"

#if ( STATEMACHINE == 1U )
#include "statemachine.h"
#include "stdlib.h"

#if (MODULE_DEBUG == 1U) && (STATEMACHINE_DEBUG == 1U)
    #define SMPRINT1(a)              PRINT_DEBUG1(a)               
    #define SMPRINT2(a,b)            PRINT_DEBUG2(a,b)             
    #define SMPRINT3(a,b,c)          PRINT_DEBUG3(a,b,c)
#else 
    #define SMPRINT1(a)           
    #define SMPRINT2(a,b)           
    #define SMPRINT3(a,b,c)
#endif

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
* Function : sum
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/


void ezmStateMachine_Execution(ezmStateMachine * pstStateMachine)
{
    uint8_t u8CurrState = pstStateMachine->u8CurrState;

    pstStateMachine->u8CurrEvent = pstStateMachine->GetExternalEvent();

    if(pstStateMachine->u8CurrEvent < pstStateMachine->u8NumOfEvent)
    {
        if(pstStateMachine->pstState[u8CurrState].pfEntry != NULL)
        {
            SMPRINT2("Enter state: %s", pstStateMachine->pstState[u8CurrState].pcStateName);
            pstStateMachine->pstState[u8CurrState].pfEntry();
        }

        SMPRINT1("Handling...");
        pstStateMachine->u8CurrState = pstStateMachine->pstState[u8CurrState].pfEventHandling(pstStateMachine->u8CurrEvent, pstStateMachine->pstData);
        SMPRINT2("Next state %d", pstStateMachine->u8CurrState);       

        if(pstStateMachine->pstState[u8CurrState].pfExit != NULL)
        {
            pstStateMachine->pstState[u8CurrState].pfExit();
            SMPRINT2("Leave state: %s", pstStateMachine->pstState[u8CurrState].pcStateName);
        }

        pstStateMachine->u8LastState = u8CurrState;
    }
    else
    {
        pstStateMachine->ErrorHandler();
    }
}

void ezmStateMachine_Init(ezmStateMachine * pstStateMachine, 
                        uint8_t u8StartEvent,
                        uint8_t u8StartState,
                        uint8_t u8NumOfEvent,
                        uint8_t u8NumOfState,
                        ezmState * pstArrayOfstate,
                        void (*ErrorHandler)(void),
                        uint8_t (*GetExternalEvent)(void),
                        void * pstStatemachineData)
{
    pstStateMachine->u8CurrEvent = u8StartEvent;
    pstStateMachine->u8CurrState = u8StartState;
    pstStateMachine->u8NumOfEvent = u8NumOfEvent;
    pstStateMachine->u8NumOfState = u8NumOfState;
    pstStateMachine->pstState = pstArrayOfstate;
    pstStateMachine->ErrorHandler = ErrorHandler;
    pstStateMachine->GetExternalEvent = GetExternalEvent;
    pstStateMachine->pstData = pstStatemachineData;
}

#endif /* STATEMACHINE */
/* End of file*/