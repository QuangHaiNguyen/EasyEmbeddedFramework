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


#ifndef _MODULE_H
#define _MODULE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */
typedef struct
{
    const char * pcStateName;
	uint8_t (*pfEventHandling)(uint8_t u8Event, void* pstData);
	void (*pfEntry)(void);
	void (*pfExit)(void);
}ezmState;

/** @brief definition of a new type
 *  
 */
typedef struct
{
	uint8_t u8CurrEvent;
	uint8_t u8CurrState;
    uint8_t u8LastState;
	uint8_t u8NumOfState;
	uint8_t u8NumOfEvent;
	ezmState  * pstState;
	void (*ErrorHandler)(void);
	uint8_t (*GetExternalEvent)(void);
    void* pstData;
}ezmStateMachine;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void ezmStateMachine_Init(ezmStateMachine * pstStateMachine, 
                        uint8_t u8StartEvent,
                        uint8_t u8StartState,
                        uint8_t u8NumOfEvent,
                        uint8_t u8NumOfState,
                        ezmState * pstArrayOfstate,
                        void (*ErrorHandler)(void),
                        uint8_t (*GetExternalEvent)(void),
                        void * pstStatemachineData);

void ezmStateMachine_Execution(ezmStateMachine * pstStateMachine);

#endif /* _MODULE_H */

/* End of file*/
