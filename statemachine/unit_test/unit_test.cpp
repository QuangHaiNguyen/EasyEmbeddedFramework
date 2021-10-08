#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../statemachine.h"
#include <stdlib.h>

typedef enum
{
    NO_EVENT,
	EVENT_1,
	EVENT_2,
	NUM_OF_EVENT,
}ENUM_EVENT;


typedef enum
{
	STATE_START,
	STATE_A,
	STATE_STOP,
    STATE_ERROR,
	NUM_OF_STATE
}ENUM_STATE;

uint8_t StateAEventHandler(uint8_t eEvent, void * pstData __attribute__((unused)))
{
    uint8_t u8NextState;

    printf("State A Entered\n");
    switch(eEvent)
    {
        case (uint8_t)NO_EVENT:
            printf("NO_EVENT\n");
            u8NextState = (uint8_t)STATE_A;
            break;
        
        case (uint8_t)EVENT_1:
            printf("EVENT_1\n");
            u8NextState = (uint8_t)STATE_STOP;
            break;

        case (uint8_t)EVENT_2:
            printf("EVENT_2\n");
            u8NextState = (uint8_t)STATE_START;
            break;

        default:
            u8NextState = (uint8_t)STATE_ERROR;
            break;
    }

    return u8NextState;
}

uint8_t StateStartEventHandler(uint8_t eEvent, void * pstData __attribute__((unused)))
{
    uint8_t u8NextState;
    printf("State Start Entered\n");
    switch(eEvent)
    {
        case (uint8_t)NO_EVENT:
            printf("NO_EVENT\n");
            u8NextState = (uint8_t)STATE_START;
            break;
        
        case (uint8_t)EVENT_1:
            printf("EVENT_1\n");
            u8NextState = (uint8_t)STATE_A;
            break;

        case (uint8_t)EVENT_2:
            printf("EVENT_2\n");
            u8NextState = (uint8_t)STATE_STOP;
            break;

        default:
            u8NextState = (uint8_t)STATE_ERROR;
            break;
    }

    return u8NextState;
}

uint8_t StateStopEventHandler(uint8_t eEvent, void * pstData __attribute__((unused)))
{
    uint8_t u8NextState;
    printf("State Stop Entered\n");
    switch(eEvent)
    {
        case (uint8_t)NO_EVENT:
            printf("NO_EVENT\n");
            u8NextState = (uint8_t)STATE_STOP;
            break;
        
        case (uint8_t)EVENT_1:
            printf("EVENT_1\n");
            u8NextState = (uint8_t)STATE_START;
            break;

        case (uint8_t)EVENT_2:
            printf("EVENT_2\n");
            u8NextState = (uint8_t)STATE_A;
            break;

        default:
            u8NextState = (uint8_t)STATE_ERROR;
            break;
    }

    return u8NextState;
}

uint8_t StateErrorEventHandler(uint8_t eEvent, void * pstData __attribute__((unused)))
{
    uint8_t u8NextState;
    printf("State Error Entered\n");
    switch(eEvent)
    {
        case (uint8_t)NO_EVENT:
            printf("NO_EVENT\n");
            u8NextState = (uint8_t)STATE_START;
            break;
        
        case (uint8_t)EVENT_1:
            printf("EVENT_1\n");
            u8NextState = (uint8_t)STATE_START;
            break;

        case (uint8_t)EVENT_2:
            printf("EVENT_2\n");
            u8NextState = (uint8_t)STATE_START;
            break;

        default:
            u8NextState = (uint8_t)STATE_START;
            break;
    }

    return u8NextState;
}

void StateAEnter(void)
{
    printf("StateAEnter\n");
}
void StateStartEnter(void)
{
    printf("StateStartEnter\n");
}
void StateStopEnter(void)
{
    printf("StateStopEnter\n");
}

void StateAExit(void)
{
    printf("StateAExit\n");
}

void StateStarExit(void)
{
    printf("StateStarExit\n");
}

void StateStopExit(void)
{
    printf("StateStopExit\n");
}

void StateMachineErrorHandler(void)
{
    printf("error!!!!!\n");
}

uint8_t GetExternalEvent(void)
{
    uint8_t u8NextEvent = rand() % 3;
    printf("Next event: %d\n", u8NextEvent);
    return u8NextEvent;
}


ezmState StateStart = { "state start", &StateStartEventHandler, &StateStartEnter, &StateStarExit};
ezmState StateA  = { "state A", &StateAEventHandler, &StateAEnter, &StateAExit};
ezmState StateStop  = { "state stop", &StateStopEventHandler, &StateStopEnter, &StateStopExit};
ezmState StateError = { "state error", &StateErrorEventHandler, NULL, NULL};
ezmState astState[NUM_OF_STATE] = {StateStart, StateA, StateStop,  StateError};
ezmStateMachine stStatemachine;

}



namespace 
{
    TEST(StateMachine, TryRun) 
    {
        ezmStateMachine_Init(&stStatemachine,
                                NO_EVENT,
                                STATE_START,
                                NUM_OF_EVENT,
                                NUM_OF_STATE,
                                astState,
                                &StateMachineErrorHandler,
                                &GetExternalEvent,
                                NULL);
        for(uint8_t i = 0; i < 20; i++)
        {
            printf("round %d\n", i);
            ezmStateMachine_Execution(&stStatemachine);
        }
        
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}