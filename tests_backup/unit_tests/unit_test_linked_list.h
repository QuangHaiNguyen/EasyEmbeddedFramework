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

#if (CONFIG_HELPER_LINKEDLIST == 1U)
    TEST(Linked_List, Test_Macro)
    {
        struct test_struct1
        {
            char c;
            struct Node node;
        };

        struct a_more_complex_one
        {
            char c;
            uint8_t buffer[5];
            uint32_t data;
            struct Node node;
            bool busy;
        };

        struct Node head = EZMLL_INIT_NODE(head);

        EXPECT_TRUE(head.next == &head);
        EXPECT_TRUE(head.prev == &head);

        struct Node node1 = EZMLL_INIT_NODE(node1);
        ASSERT_EQ(node1.next, &node1);
        ASSERT_EQ(node1.prev, &node1);

        struct Node node2 = EZMLL_INIT_NODE(node2);
        ASSERT_EQ(node2.next, &node2);
        ASSERT_EQ(node2.prev, &node2);

        struct Node node3 = EZMLL_INIT_NODE(node3);
        ASSERT_EQ(node3.next, &node3);
        ASSERT_EQ(node3.prev, &node3);

        EZMLL_ADD_HEAD(&head, &node1);
        EZMLL_ADD_TAIL(&head, &node2);

        struct Node* it_node = NULL;
        uint32_t count = 0;

        EZMLL_FOR_EACH(it_node, &head)
        {
            count++;
        }
        ASSERT_EQ(count, 2);

        EZMLL_UNLINK_NODE(&node2);

        count = 0;
        EZMLL_FOR_EACH(it_node, &head)
        {
            count++;
        }
        ASSERT_EQ(count, 1);

        it_node = &head;
        EZMLL_TO_NEXT_NODE(it_node);
        ASSERT_EQ(it_node, &node1);

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
        struct Node *iterate_node;

        struct Node node1 = EZMLL_INIT_NODE(node1);
        struct Node node2 = EZMLL_INIT_NODE(node2);
        struct Node node3 = EZMLL_INIT_NODE(node3);
        struct Node node4 = EZMLL_INIT_NODE(node4);
        struct Node node5 = EZMLL_INIT_NODE(node5);

        void* address[5] =
        {
            &node1,
            &node2,
            &node3,
            &node4,
            &node5,
        };

        struct Node* head = &node1;

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
        struct Node* iterate_node;
        struct Node* new_head;

        struct Node node1 = EZMLL_INIT_NODE(node1);
        struct Node node2 = EZMLL_INIT_NODE(node2);
        struct Node node3 = EZMLL_INIT_NODE(node3);
        struct Node node4 = EZMLL_INIT_NODE(node4);
        struct Node node5 = EZMLL_INIT_NODE(node5);

        struct Node* head = &node1;
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

        struct Node node1 = EZMLL_INIT_NODE(node1);
        struct Node node2 = EZMLL_INIT_NODE(node2);
        struct Node node3 = EZMLL_INIT_NODE(node3);
        struct Node node4 = EZMLL_INIT_NODE(node4);
        struct Node node5 = EZMLL_INIT_NODE(node5);

        struct Node* head = &node1;
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

    TEST(Linked_List, Unlink_node)
    {
        struct Node node1 = EZMLL_INIT_NODE(node1);
        struct Node node2 = EZMLL_INIT_NODE(node2);
        struct Node node3 = EZMLL_INIT_NODE(node3);
        struct Node node4 = EZMLL_INIT_NODE(node4);
        struct Node node5 = EZMLL_INIT_NODE(node5);

        struct Node* head = &node1;
        head = ezmLL_InsertNewHead(head, &node2);
        head = ezmLL_InsertNewHead(head, &node3);
        head = ezmLL_InsertNewHead(head, &node4);
        head = ezmLL_InsertNewHead(head, &node5);

        EZMLL_UNLINK_NODE(&node1);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node1));

        EZMLL_UNLINK_NODE(&node2);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node2));

        EZMLL_UNLINK_NODE(&node3);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node3));

        EZMLL_UNLINK_NODE(&node4);
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node4));
    }

    TEST(Linked_List, ezmLL_AppendNode)
    {
        struct Node node1 = EZMLL_INIT_NODE(node1);
        struct Node node2 = EZMLL_INIT_NODE(node2);
        struct Node node3 = EZMLL_INIT_NODE(node3);
        struct Node* head = &node1;

        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node2));
        ASSERT_EQ(false, ezmLL_IsNodeInList(head, &node3));

        ASSERT_EQ(true, ezmLL_AppendNode(&node2, head));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node2));

        ASSERT_EQ(true, ezmLL_AppendNode(&node3, &node2));
        ASSERT_EQ(true, ezmLL_IsNodeInList(head, &node3));
    }

#endif /* CONFIG_HELPER_LINKEDLIST */
#endif /* UNIT_TEST_LINKEDLIST */
}