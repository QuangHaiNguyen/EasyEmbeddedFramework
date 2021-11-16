#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../linked_list.h"
}

#define NUM_OF_NODE 10U

namespace 
{
#if (HELPER_LINKEDLIST == 1U)
    TEST(Linked_List, Add_Node_To_Head) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        LinkedList_InsertToHead(NULL, NULL);
        LinkedList_InsertToHead(NULL, NULL);
        LinkedList_InsertToHead(&stMyList, NULL );
        LinkedList_InsertToHead(&stMyList, NULL );

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToHead(NULL, &astNode[i]);
            LinkedList_InsertToHead(&stMyList, &astNode[i]);
        }

        ASSERT_EQ(NUM_OF_NODE, stMyList.u16Size);
        ASSERT_EQ((void*)astNode, stMyList.pstTail);
        ASSERT_EQ((void*)(astNode + NUM_OF_NODE - 1), stMyList.pstHead);
    }

    TEST(Linked_List, Remove_Node_From_Head) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToHead(&stMyList, &astNode[i]);
        }

        for(uint8_t i = 0; i < NUM_OF_NODE - 1; i++)
        {
            LinkedList_RemoveFromHead(&stMyList);
            ASSERT_EQ((void*)stMyList.pstHead, (void*)astNode[NUM_OF_NODE - 1 - i].pstNextNode);
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
        Node astNode[NUM_OF_NODE];
        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U;

        LinkedList_InsertToTail(NULL, NULL);
        LinkedList_InsertToTail(NULL, NULL);
        LinkedList_InsertToTail(&stMyList, NULL );
        LinkedList_InsertToTail(&stMyList, NULL );

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(NULL, &astNode[i]);
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        ASSERT_EQ(NUM_OF_NODE, stMyList.u16Size);
        ASSERT_EQ((void*)astNode, stMyList.pstHead);
        ASSERT_EQ((void*)(astNode + NUM_OF_NODE - 1), stMyList.pstTail);
    }

    TEST(Linked_List, Remove_Node_From_Tail) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        for(uint8_t i = 0; i < NUM_OF_NODE - 1; i++)
        {
            LinkedList_RemoveFromTail(&stMyList);
            ASSERT_EQ((void*)stMyList.pstTail, (void*)astNode[NUM_OF_NODE - 1 - i].pstPrevNode);
            ASSERT_EQ((void*)stMyList.pstHead, (void*)astNode);
        }

        LinkedList_RemoveFromTail(&stMyList);

        ASSERT_EQ(0U, stMyList.u16Size);
        EXPECT_TRUE((void*)stMyList.pstHead == nullptr);
        EXPECT_TRUE((void*)stMyList.pstTail == nullptr);
    }

    TEST(Linked_List, Search_Node_By_Index) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        Node * pstFoundNode = NULL;

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 10U);
        EXPECT_TRUE(pstFoundNode== nullptr);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 30U);
        EXPECT_TRUE(pstFoundNode== nullptr);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 0U);
        ASSERT_EQ((void*)stMyList.pstHead, pstFoundNode);
        ASSERT_EQ((void*)astNode, pstFoundNode);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 9U);
        ASSERT_EQ((void*)stMyList.pstTail, pstFoundNode);
        ASSERT_EQ((void*)(astNode + 9U), pstFoundNode);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 5U);
        ASSERT_EQ((void*)(astNode + 5U), pstFoundNode);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 3U);
        ASSERT_EQ((void*)(astNode + 3U), pstFoundNode);

        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 7U);
        ASSERT_EQ((void*)(astNode + 7U), pstFoundNode);
    
        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, i);
            ASSERT_EQ((void*)(astNode + i), pstFoundNode);
        }
    }

    TEST(Linked_List, Insert_Node_By_Index) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        Node stHeadNode;
        Node stTailNode;
        Node stNodeidx3;
        Node stNodeidx5;
        Node stNodeidx7;
        Node stNodeidx9;

        Node * pstFoundNode = NULL;

        LinkedList_InsertNodeAtIndex(&stMyList, &stHeadNode, 0U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 0U);
        ASSERT_EQ((void*)pstFoundNode, &stHeadNode);
        ASSERT_EQ(stMyList.u16Size, 11U);

        LinkedList_InsertNodeAtIndex(&stMyList, &stTailNode, 10U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 11U);
        ASSERT_EQ((void*)pstFoundNode, &stTailNode);
        ASSERT_EQ(stMyList.u16Size, 12U);
        
        LinkedList_InsertNodeAtIndex(&stMyList, &stNodeidx3, 3U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 3U);
        ASSERT_EQ((void*)pstFoundNode, &stNodeidx3);
        ASSERT_EQ(stMyList.u16Size, 13U);
        
        LinkedList_InsertNodeAtIndex(&stMyList, &stNodeidx5, 5U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 5U);
        ASSERT_EQ((void*)pstFoundNode, &stNodeidx5);
        ASSERT_EQ(stMyList.u16Size, 14U);

        LinkedList_InsertNodeAtIndex(&stMyList, &stNodeidx7, 7U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 7U);
        ASSERT_EQ((void*)pstFoundNode, &stNodeidx7);
        ASSERT_EQ(stMyList.u16Size, 15U);

        LinkedList_InsertNodeAtIndex(&stMyList, &stNodeidx9, 9U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 9U);
        ASSERT_EQ((void*)pstFoundNode, &stNodeidx9);
        ASSERT_EQ(stMyList.u16Size, 16U);

        Node stOutOfBoundNode1;
        Node stOutOfBoundNode2;
        Node stOutOfBoundNode3;

        LinkedList_InsertNodeAtIndex(&stMyList, &stOutOfBoundNode1, 19U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 19U);
        EXPECT_TRUE(pstFoundNode== nullptr);
        ASSERT_EQ(stMyList.u16Size, 16U);

        LinkedList_InsertNodeAtIndex(&stMyList, &stOutOfBoundNode2, 29U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 29U);
        EXPECT_TRUE(pstFoundNode== nullptr);
        ASSERT_EQ(stMyList.u16Size, 16U);

        LinkedList_InsertNodeAtIndex(&stMyList, &stOutOfBoundNode3, 39U);
        pstFoundNode = LinkedList_FindNodeAtIndex(&stMyList, 39U);
        EXPECT_TRUE(pstFoundNode== nullptr);
        ASSERT_EQ(stMyList.u16Size, 16U);
    }

    TEST(Linked_List, Remove_Node_By_Index) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }


        Node * pstRemoveNode;

        pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, 0);
        ASSERT_EQ((void*)pstRemoveNode, &astNode[0U]);
        ASSERT_EQ(stMyList.u16Size, 9U);
        
        pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, stMyList.u16Size - 1U);
        ASSERT_EQ((void*)pstRemoveNode, &astNode[NUM_OF_NODE - 1U]);
        ASSERT_EQ(stMyList.u16Size,8U);

        pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, 2U);
        ASSERT_EQ((void*)pstRemoveNode, &astNode[3U]);
        ASSERT_EQ(stMyList.u16Size,7U);

        pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, 5U);
        ASSERT_EQ((void*)pstRemoveNode, &astNode[7U]);
        ASSERT_EQ(stMyList.u16Size, 6U);

        for(uint8_t i = 0; i < 6U; i++)
        {
            pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, 0U);
        }
        
        pstRemoveNode = LinkedList_RemoveNodeAtIndex(&stMyList, 0U);

        EXPECT_TRUE(pstRemoveNode== nullptr);
        ASSERT_EQ(stMyList.u16Size, 0U);
    }

    TEST(Linked_List, Search_Node) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }
        
        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            Node * pstResultNode = LinkedList_SearchNode(&stMyList, &astNode[i]);
            EXPECT_TRUE(pstResultNode != nullptr);
            ASSERT_EQ((void*)pstResultNode, &astNode[i]);
        }

        Node anotherNode;
        Node * pstResultNode = LinkedList_SearchNode(&stMyList, &anotherNode);
        EXPECT_TRUE(pstResultNode == nullptr);
    }

    TEST(Linked_List, Insert_Node_After) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }
 
        Node anotherNode1;
        Node * pstTestNode;
        LinkedList_InsertNewNodeAfterANode(&stMyList,  &astNode[0], &anotherNode1);
        pstTestNode = LinkedList_FindNodeAtIndex(&stMyList, 1);  
        ASSERT_EQ(&anotherNode1, pstTestNode);
        ASSERT_EQ(NUM_OF_NODE + 1U, stMyList.u16Size);

        Node anotherNode2;
        LinkedList_InsertNewNodeAfterANode(&stMyList,  &astNode[9], &anotherNode2);
        pstTestNode = LinkedList_FindNodeAtIndex(&stMyList, 11);  
        ASSERT_EQ(&anotherNode2, pstTestNode);
        ASSERT_EQ(NUM_OF_NODE + 2U, stMyList.u16Size);
    }

     TEST(Linked_List, Remove_Node) 
    {
        LinkedList stMyList;
        Node astNode[NUM_OF_NODE];

        stMyList.pstHead = NULL;
        stMyList.pstTail = NULL;
        stMyList.u16Size = 0U; 

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            astNode[i].pstNextNode = NULL;
            astNode[i].pstPrevNode = NULL;
        }

        for(uint8_t i = 0; i < NUM_OF_NODE; i++)
        {
            LinkedList_InsertToTail(&stMyList, &astNode[i]);
        }

        bool bSuccess;
        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[0]);
        ASSERT_EQ(bSuccess, true);
        ASSERT_EQ(NUM_OF_NODE - 1U, stMyList.u16Size);

        bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[9]);
        ASSERT_EQ(bSuccess, true);
        ASSERT_EQ(NUM_OF_NODE - 2U, stMyList.u16Size);

        for(uint8_t i = 1; i < NUM_OF_NODE - 1U; i++)
        {
            bSuccess = LinkedList_RemoveNode(&stMyList, &astNode[i]);
            ASSERT_EQ(bSuccess, true);
            ASSERT_EQ(NUM_OF_NODE - 2U - i, stMyList.u16Size);
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
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}