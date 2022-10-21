#include <limits.h>
#include "gtest/gtest.h"
#include "../../app/app_config.h"

#if ( SCHEDULER == 1U )
extern "C" {
#include "../scheduler.h"
#include "stdint.h"
#include <stdbool.h>
#include <time.h>

#define TOTAL_RUNTIME               10000
#define TASK_DEFAULT_INTERVAL       500
#define TASK1_INTERVAL              50U
#define TASK2_INTERVAL              100U
#define TASK3_INTERVAL              150U
#define TASK4_INTERVAL              200U
#define TASK5_INTERVAL              250U

uint32_t u32SumDefault = 0;
uint32_t u32Sum1 = 0;
uint32_t u32Sum2 = 0;
uint32_t u32Sum3 = 0;
uint32_t u32Sum4 = 0;
uint32_t u32Sum5 = 0;

uint32_t ImplementTickFunction(void)
{
    uint32_t u32TickMillis;
    clock_t tick;
    tick = clock();
    u32TickMillis = tick/(CLOCKS_PER_SEC/1000);

    return u32TickMillis;
}

void NewDefaulfTask(void)
{
    u32SumDefault++;
}

void Task1(void)
{
    u32Sum1++;
}

void Task2(void)
{
    u32Sum2++;
}

void Task3(void)
{
    u32Sum3++;
}

void Task4(void)
{
    u32Sum4++;
}

void Task5(void)
{
    u32Sum5++;
}

}
#endif

namespace 
{
#if (SCHEDULER == 1U)

    TEST(Scheduler, Init) 
    {
        ezmScheduler_Task * pstDefaultTask;
        ezmScheduler_Task * pstTask;
        (void) ezmScheduler_Init(&ImplementTickFunction);
        
        pstDefaultTask = ezmScheduler_GetTask(0x00);

        ASSERT_EQ(pstDefaultTask->u32IntervalMillis, 100);
        ASSERT_EQ(pstDefaultTask->u8TaskId, 0x00);
        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 1U);
        EXPECT_TRUE(pstDefaultTask->fpApplication != NULL);

        pstTask = ezmScheduler_GetTask(12);
        EXPECT_TRUE(pstTask == NULL);

        pstTask = ezmScheduler_GetTask(23);
        EXPECT_TRUE(pstTask == NULL);

        pstTask = ezmScheduler_GetTask(50);
        EXPECT_TRUE(pstTask == NULL);

        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 1U);
    }

    TEST(Scheduler, NewDefaultTask) 
    {
        ezmScheduler_Task * pstDefaultTask;
        void (*fpTest)(void) = &NewDefaulfTask;

        (void)ezmScheduler_Init(&ImplementTickFunction);

        ezmScheduler_OverwriteDefaulTask(&NewDefaulfTask, TASK_DEFAULT_INTERVAL);

        pstDefaultTask = ezmScheduler_GetTask(0x00);

        ASSERT_EQ(pstDefaultTask->u32IntervalMillis, 500);
        ASSERT_EQ(pstDefaultTask->u8TaskId, 0x00);
        EXPECT_TRUE(pstDefaultTask->fpApplication == fpTest);

        uint32_t u32Currenttime = ezmScheduler_GetCurrentTickMillis();

        while(u32Currenttime + TOTAL_RUNTIME > ezmScheduler_GetCurrentTickMillis())
        {
            ezmScheduler_Run();
        }
        GTEST_ASSERT_GE(u32SumDefault, TOTAL_RUNTIME/TASK_DEFAULT_INTERVAL - 1);
    }

    TEST(Scheduler, AddTask)
    {
        ezmScheduler_Task * pstTask;
        uint8_t u8TaskId;
        (void) ezmScheduler_Init(&ImplementTickFunction);

        u8TaskId = ezmScheduler_RegisterTask(&Task1, TASK1_INTERVAL);
        pstTask = ezmScheduler_GetTask(u8TaskId);
        ASSERT_EQ(pstTask->u32IntervalMillis, TASK1_INTERVAL);
        ASSERT_EQ(pstTask->u8TaskId, u8TaskId);
        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 2U);
        EXPECT_TRUE(pstTask->fpApplication != NULL);

        u8TaskId = ezmScheduler_RegisterTask(&Task2, TASK2_INTERVAL);
        pstTask = ezmScheduler_GetTask(u8TaskId);
        ASSERT_EQ(pstTask->u32IntervalMillis, TASK2_INTERVAL);
        ASSERT_EQ(pstTask->u8TaskId, u8TaskId);
        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 3U);
        EXPECT_TRUE(pstTask->fpApplication != NULL);

        u8TaskId = ezmScheduler_RegisterTask(&Task3, TASK3_INTERVAL);
        pstTask = ezmScheduler_GetTask(u8TaskId);
        ASSERT_EQ(pstTask->u32IntervalMillis, TASK3_INTERVAL);
        ASSERT_EQ(pstTask->u8TaskId, u8TaskId);
        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 4U);
        EXPECT_TRUE(pstTask->fpApplication != NULL);

        u8TaskId = ezmScheduler_RegisterTask(&Task4, TASK4_INTERVAL);
        pstTask = ezmScheduler_GetTask(u8TaskId);
        ASSERT_EQ(pstTask->u32IntervalMillis, TASK4_INTERVAL);
        ASSERT_EQ(pstTask->u8TaskId, u8TaskId);
        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 5U);
        EXPECT_TRUE(pstTask->fpApplication != NULL);

        u8TaskId = ezmScheduler_RegisterTask(&Task5, TASK5_INTERVAL);
        ASSERT_EQ(u8TaskId, 0xFF);
    }

    TEST(Scheduler, RemoveTask)
    {
        uint8_t u8TaskId1;
        uint8_t u8TaskId2;
        uint8_t u8TaskId3;
        uint8_t u8TaskId4;
        
        (void) ezmScheduler_Init(&ImplementTickFunction);
        u8TaskId1 = ezmScheduler_RegisterTask(&Task1, TASK1_INTERVAL);
        u8TaskId2 = ezmScheduler_RegisterTask(&Task2, TASK2_INTERVAL);
        u8TaskId3 = ezmScheduler_RegisterTask(&Task3, TASK3_INTERVAL);
        u8TaskId4 = ezmScheduler_RegisterTask(&Task4, TASK4_INTERVAL);
    
        ezmScheduler_RemoveTask(u8TaskId1);
        ezmScheduler_RemoveTask(u8TaskId2);
        ezmScheduler_RemoveTask(u8TaskId3);
        ezmScheduler_RemoveTask(u8TaskId4);

        ASSERT_EQ(ezmScheduler_GetNumOfActiveTasks(), 1U);
    } 

    TEST(Scheduler, RunTask)
    {
        uint32_t u32SumCheckpoint1 = 0;
        uint32_t u32SumCheckpoint2 = 0;
        uint32_t u32SumCheckpoint3 = 0;
        uint32_t u32SumCheckpoint4 = 0;
        uint32_t u32Currenttime;

        uint8_t u8TaskId3;
        uint8_t u8TaskId4;
        
        (void) ezmScheduler_Init(&ImplementTickFunction);
        (void) ezmScheduler_RegisterTask(&Task1, TASK1_INTERVAL);
        (void) ezmScheduler_RegisterTask(&Task2, TASK2_INTERVAL);
        u8TaskId3 = ezmScheduler_RegisterTask(&Task3, TASK3_INTERVAL);
        u8TaskId4 = ezmScheduler_RegisterTask(&Task4, TASK4_INTERVAL);

        u32Currenttime = ezmScheduler_GetCurrentTickMillis();
        while(u32Currenttime + TOTAL_RUNTIME > ezmScheduler_GetCurrentTickMillis())
        {
            ezmScheduler_Run();
        }
        GTEST_ASSERT_GE(u32Sum1, TOTAL_RUNTIME/TASK1_INTERVAL - 1);
        GTEST_ASSERT_GE(u32Sum2, TOTAL_RUNTIME/TASK2_INTERVAL - 1);
        GTEST_ASSERT_GE(u32Sum3, TOTAL_RUNTIME/TASK3_INTERVAL - 1);
        GTEST_ASSERT_GE(u32Sum4, TOTAL_RUNTIME/TASK4_INTERVAL - 1);

        u32SumCheckpoint1 = u32Sum1;
        u32SumCheckpoint2 = u32Sum2;
        u32SumCheckpoint3 = u32Sum3;
        u32SumCheckpoint4 = u32Sum4;

        ezmScheduler_RemoveTask(u8TaskId3);
        ezmScheduler_RemoveTask(u8TaskId4);

        u32Currenttime = ezmScheduler_GetCurrentTickMillis();
        while(u32Currenttime + TOTAL_RUNTIME > ezmScheduler_GetCurrentTickMillis())
        {
            ezmScheduler_Run();
        }
        GTEST_ASSERT_GE(u32Sum1, u32SumCheckpoint1 + TOTAL_RUNTIME/TASK1_INTERVAL - 1);
        GTEST_ASSERT_GE(u32Sum2, u32SumCheckpoint2 + TOTAL_RUNTIME/TASK2_INTERVAL - 1);
        ASSERT_EQ(u32Sum3, u32SumCheckpoint3);
        ASSERT_EQ(u32Sum4, u32SumCheckpoint4);
    }
#endif /* SCHEDULER */
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}