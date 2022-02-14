#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_LINKEDLIST
#define UNIT_TEST_LINKEDLIST

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/linked_list/linked_list.h"
}

#define NUM_NODE 10U

namespace
{
#if (HELPER_LINKEDLIST == 1U)
    TEST(Linked_List, Add_Node_To_Head)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        LinkedList_InsertToHead(NULL, NULL);
        LinkedList_InsertToHead(NULL, NULL);
        LinkedList_InsertToHead(&stMyList, NULL);
        LinkedList_InsertToHead(&stMyList, NULL);

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToHead(NULL, &astNode[i]);
            LinkedList_InsertToHead(&stMyList, &astNode[i]);
        }

        ASSERT_EQ(NUM_NODE, stMyList.u16Size);
        ASSERT_EQ((void*)astNode, stMyList.pstTail);
        ASSERT_EQ((void*)(astNode + NUM_NODE - 1), stMyList.pstHead);
    }

    TEST(Linked_List, Remove_Node_From_Head)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToHead(&stMyList, &astNode[i]);
        }

        for (uint8_t i = 0; i < NUM_NODE - 1; i++)
        {
            LinkedList_RemoveFromHead(&stMyList);
            ASSERT_EQ((void*)stMyList.pstHead, (void*)astNode[NUM_NODE - 1 - i].pstNextNode);
            ASSERT_EQ((void*)stMyList.pstTail, (void*)astNode);
        }

        LinkedList_RemoveFromHead(&stMyList);

        ASSERT_EQ(0U, stMyList.u16Size);
        EXPECT_TRUE((void*)stMyList.pstHead == nullptr);
        EXPECT_TRUE((void*)stMyList.pstTail == nullptr);
    }

    TEST(Linked_List, Add_Node_To_Tail)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];
        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        LinkedList_InsertToTail(NULL, NULL);
        LinkedList_InsertToTail(NULL, NULL);
        LinkedList_InsertToTail(&stMyList, NULL);
        LinkedList_InsertToTail(&stMyList, NULL);

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToTail(NULL, &astNode[i]);
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        ASSERT_EQ(NUM_NODE, stMyList.u16Size);
        ASSERT_EQ((void*)astNode, stMyList.pstHead);
        ASSERT_EQ((void*)(astNode + NUM_NODE - 1), stMyList.pstTail);
    }

    TEST(Linked_List, Remove_Node_From_Tail)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        for (uint8_t i = 0; i < NUM_NODE - 1; i++)
        {
            LinkedList_RemoveFromTail(&stMyList);
            ASSERT_EQ((void*)stMyList.pstTail, (void*)astNode[NUM_NODE - 1 - i].pstPrevNode);
            ASSERT_EQ((void*)stMyList.pstHead, (void*)astNode);
        }

        LinkedList_RemoveFromTail(&stMyList);

        ASSERT_EQ(0U, stMyList.u16Size);
        EXPECT_TRUE((void*)stMyList.pstHead == nullptr);
        EXPECT_TRUE((void*)stMyList.pstTail == nullptr);
    }

    TEST(Linked_List, Search_Node)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            Node* pstResultNode = LinkedList_SearchNode(&stMyList, &astNode[i]);
            EXPECT_TRUE(pstResultNode != nullptr);
            ASSERT_EQ((void*)pstResultNode, &astNode[i]);
        }

        Node anotherNode;
        Node* pstResultNode = LinkedList_SearchNode(&stMyList, &anotherNode);
        EXPECT_TRUE(pstResultNode == nullptr);
    }

    TEST(Linked_List, Insert_Node_After)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }
    }

    TEST(Linked_List, Remove_Node)
    {
        LinkedList stMyList;
        Node astNode[NUM_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for (uint8_t i = 0; i < NUM_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        bool bSuccess;
        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[0]);
        ASSERT_EQ(bSuccess, true);
        ASSERT_EQ(NUM_NODE - 1U, stMyList.u16Size);

        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[9]);
        ASSERT_EQ(bSuccess, true);
        ASSERT_EQ(NUM_NODE - 2U, stMyList.u16Size);

        for (uint8_t i = 1; i < NUM_NODE - 1U; i++)
        {
            bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[i]);
            ASSERT_EQ(bSuccess, true);
            ASSERT_EQ(NUM_NODE - 2U - i, stMyList.u16Size);
        }

        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[9]);
        ASSERT_EQ(bSuccess, false);
        ASSERT_EQ(0U, stMyList.u16Size);

        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[4]);
        ASSERT_EQ(bSuccess, false);
        ASSERT_EQ(0U, stMyList.u16Size);

        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[0]);
        ASSERT_EQ(bSuccess, false);
        ASSERT_EQ(0U, stMyList.u16Size);

    }
#endif /* HELPER_LINKEDLIST */
#endif /* UNIT_TEST_LINKEDLIST */
}