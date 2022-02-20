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
    TEST(Linked_List, Test_Macro)
    {
        struct test_struct1
        {
            char c;
            Node node;
        };

        struct a_more_complex_one
        {
            char c;
            uint8_t buffer[5];
            uint32_t data;
            Node node;
            bool busy;
        };

        Node head = EZMLL_INIT_NODE(head);

        EXPECT_TRUE(head.next == &head);
        EXPECT_TRUE(head.prev == &head);

        Node node1 = EZMLL_INIT_NODE(node1);
        ASSERT_EQ(node1.next, &node1);
        ASSERT_EQ(node1.prev, &node1);

        Node node2 = EZMLL_INIT_NODE(node2);
        ASSERT_EQ(node2.next, &node2);
        ASSERT_EQ(node2.prev, &node2);

        Node node3 = EZMLL_INIT_NODE(node3);
        ASSERT_EQ(node3.next, &node3);
        ASSERT_EQ(node3.prev, &node3);

        struct test_struct1 data1;
        data1.c = 'a';
        data1.node = EZMLL_INIT_NODE(data1.node);

        test_struct1* test1 = EZMLL_GET_PARENT_OF(&data1.node, node, test_struct1);
        ASSERT_EQ(test1, &data1);
        ASSERT_EQ(test1->c, data1.c);

        struct a_more_complex_one complex_one;
        complex_one.busy = false;
        complex_one.data = 123456;

        a_more_complex_one * complex_one_ptr = EZMLL_GET_PARENT_OF(&complex_one.node, node, a_more_complex_one);
        ASSERT_EQ(complex_one_ptr, &complex_one);
        ASSERT_EQ(complex_one_ptr->c, complex_one.c);
        ASSERT_EQ(complex_one_ptr->busy, complex_one.busy);
        ASSERT_EQ(complex_one_ptr->data, complex_one.data);
    }

    TEST(Linked_List, LinkedList_InsertNewHead)
    {
        uint32_t count = 0;
        Node *iterate_node; 

        Node node1 = EZMLL_INIT_NODE(node1);
        Node node2 = EZMLL_INIT_NODE(node2);
        Node node3 = EZMLL_INIT_NODE(node3);
        Node node4 = EZMLL_INIT_NODE(node4);
        Node node5 = EZMLL_INIT_NODE(node5);

        void* address[5] =
        {
            &node1,
            &node2,
            &node3,
            &node4,
            &node5,
        };

        Node* head = &node1;

        ASSERT_EQ(false, ezmLL_InsertNewHead(NULL, NULL));
        ASSERT_EQ(false, ezmLL_InsertNewHead(&node1, NULL));
        ASSERT_EQ(false, ezmLL_InsertNewHead(NULL, &node2));

        EZMLL_FOR_EACH(iterate_node, head)
        {
            count = count + 1;
        }

        ASSERT_EQ(0, count);

        head = ezmLL_InsertNewHead(head, &node2);
        ASSERT_EQ(head, &node2);

        head = ezmLL_InsertNewHead(head, &node3);
        ASSERT_EQ(head, &node3);

        head = ezmLL_InsertNewHead(head, &node4);
        ASSERT_EQ(head, &node4);

        head = ezmLL_InsertNewHead(head, &node5);
        ASSERT_EQ(head, &node5);


        count = 0U;
        EZMLL_FOR_EACH(iterate_node, head)
        {
            ASSERT_EQ(iterate_node, address[3 - count]);
            count = count + 1;
        }
    }


    TEST(Linked_List, LinkedList_UnlinkCurrentHead)
    {
        uint32_t count = 0;
        Node* iterate_node;
        Node* new_head;

        Node node1 = EZMLL_INIT_NODE(node1);
        Node node2 = EZMLL_INIT_NODE(node2);
        Node node3 = EZMLL_INIT_NODE(node3);
        Node node4 = EZMLL_INIT_NODE(node4);
        Node node5 = EZMLL_INIT_NODE(node5);

        Node* head = &node1;
        head = ezmLL_InsertNewHead(head, &node2);
        head = ezmLL_InsertNewHead(head, &node3);
        head = ezmLL_InsertNewHead(head, &node4);
        head = ezmLL_InsertNewHead(head, &node5);

        new_head = ezmLL_UnlinkCurrentHead(head);
        ASSERT_EQ(new_head, &node4);
        ASSERT_EQ(head, &node5);

        count = 1;
        EZMLL_FOR_EACH(iterate_node, new_head)
        {
            count++;
        }
        ASSERT_EQ(count, 4);

        head = &node4;
        new_head = ezmLL_UnlinkCurrentHead(new_head);
        ASSERT_EQ(new_head, &node3);
        ASSERT_EQ(head, &node4);

        count = 1;
        EZMLL_FOR_EACH(iterate_node, new_head)
        {
            count++;
        }
        ASSERT_EQ(count, 3);

        head = &node3;
        new_head = ezmLL_UnlinkCurrentHead(head);
        ASSERT_EQ(new_head, &node2);
        ASSERT_EQ(head, &node3);

        count = 1;
        EZMLL_FOR_EACH(iterate_node, new_head)
        {
            count++;
        }
        ASSERT_EQ(count, 2);

        head = &node2;
        new_head = ezmLL_UnlinkCurrentHead(head);
        ASSERT_EQ(new_head, &node1);
        ASSERT_EQ(head, &node2);

        count = 1;
        EZMLL_FOR_EACH(iterate_node, new_head)
        {
            count++;
        }
        ASSERT_EQ(count, 1);

        head = &node1;
        new_head = ezmLL_UnlinkCurrentHead(head);
        ASSERT_EQ(new_head, &node1);
        ASSERT_EQ(head, &node1);

        count = 1;
        EZMLL_FOR_EACH(iterate_node, new_head)
        {
            count++;
        }
        ASSERT_EQ(count, 1);
    }

    TEST(Linked_List, ezmLL_IsNodeInList)
    {
        uint32_t count = 0;

        Node node1 = EZMLL_INIT_NODE(node1);
        Node node2 = EZMLL_INIT_NODE(node2);
        Node node3 = EZMLL_INIT_NODE(node3);
        Node node4 = EZMLL_INIT_NODE(node4);
        Node node5 = EZMLL_INIT_NODE(node5);

        Node* head = &node1;
        head = ezmLL_InsertNewHead(head, &node2);
        head = ezmLL_InsertNewHead(head, &node3);
        head = ezmLL_InsertNewHead(head, &node4);
        head = ezmLL_InsertNewHead(head, &node5);

        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node1));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node2));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node3));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node4));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node5));

    }

    TEST(Linked_List, ezmLL_UnlinkNode)
    {
        Node node1 = EZMLL_INIT_NODE(node1);
        Node node2 = EZMLL_INIT_NODE(node2);
        Node node3 = EZMLL_INIT_NODE(node3);
        Node node4 = EZMLL_INIT_NODE(node4);
        Node node5 = EZMLL_INIT_NODE(node5);

        Node* head = &node1;
        head = ezmLL_InsertNewHead(head, &node2);
        head = ezmLL_InsertNewHead(head, &node3);
        head = ezmLL_InsertNewHead(head, &node4);
        head = ezmLL_InsertNewHead(head, &node5);

        ezmLL_UnlinkNode(&node1);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node1));

        ezmLL_UnlinkNode(&node2);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node2));

        ezmLL_UnlinkNode(&node3);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node3));

        ezmLL_UnlinkNode(&node4);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node4));
    }

    TEST(Linked_List, ezmLL_AppendNode)
    {
        Node node1 = EZMLL_INIT_NODE(node1);
        Node node2 = EZMLL_INIT_NODE(node2);
        Node node3 = EZMLL_INIT_NODE(node3);
        Node* head = &node1;

        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node2));
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node3));

        ASSERT_EQ(true, ezmLL_AppendNode(&node2, head));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node2));

        ASSERT_EQ(true, ezmLL_AppendNode(&node3, &node2));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node3));
    }

#endif /* HELPER_LINKEDLIST */
#endif /* UNIT_TEST_LINKEDLIST */
}