/*******************************************************************************
* Title                 :   link_list 
* Filename              :   link_list.c
* Author                :   Quang Hai Nguyen
*
*******************************************************************************/

/** @file   link_list.c
 *  @brief  This is the implementation of the doubly linked list data structure
 *          It is inspired by the linked list implementation in the linux kernel
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "linked_list.h"

#if (HELPER_LINKEDLIST == 1U)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

#define NODE_INVALID_ID     0xFFFF

#define UNLINK_NODE(node)   node->prev->next = node->next;node->next->prev = node->prev;

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static Node node_pool[NUM_OF_NODE] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/


void ezmLL_Initialization(void)
{
    for (uint16_t i = 0; i < NUM_OF_NODE; i++)
    {
        //ezmLL_ResetNode(&node_pool[i]);
    }
}


/******************************************************************************
* Function : ezmLL_AppendNode
*//**
* \b Description:
*
* Append a node after a node
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *new_node: (IN)pointer to the new node
* @param    *node (IN)pointer to the node, which new node will be appended to
* @return   true if suceess
*
*******************************************************************************/
bool ezmLL_AppendNode(Node* new_node, Node* node)
{
    bool is_success = false;
    if (new_node != NULL && node != NULL)
    {
        new_node->next = node->next;
        new_node->prev = node;
        node->next->prev = new_node;
        node->next = new_node;

        is_success = true;
    }

    return is_success;
}

/******************************************************************************
* Function : ezmLL_InsertNewHead
*//** 
* \b Description:
*
* Add a new node to the current head position
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    *current_head: (IN)pointer to the current head
* @param    *new_node (IN)pointer to the new node
* @return   pointer to the new head  or NULL
*
*******************************************************************************/
Node* ezmLL_InsertNewHead(Node *current_head, Node * new_node)
{
    bool is_success = true;
    Node* new_head = NULL;

    if(current_head == NULL || new_node == NULL)
    {
        is_success = false;
    }

    if (is_success)
    {
        is_success && ezmLL_AppendNode(new_node, current_head->prev);
    }

    if (is_success)
    {
        new_head = new_node;
    }

    return new_head;
}

/******************************************************************************
* Function : ezmLL_UnlinkCurrentHead
*//**
* \b Description:
*
* unlink the current head of the linked list, the head position will be changed to
* head->next
*
* PRE-CONDITION: None
* POST-CONDITION: None
*
* @param    *head: (IN)pointer to the current head
* @return   pointer to the unlinked head or NULL
*
*******************************************************************************/
Node * ezmLL_UnlinkCurrentHead(Node * head)
{
    Node* new_head = head;

    if(head->next != head)
    {
        new_head = head->next;
        UNLINK_NODE(head);
    }

    return new_head;
}

/******************************************************************************
* Function : ezmLL_IsNodeInList
*//**
* \b Description:
*
* Check if a node is in the list
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *head: (IN)pointer to the list head
* @param    *searched_node (IN)node to be search for existance
* @return   True if exsisting
*
*******************************************************************************/
bool ezmLL_IsNodeInList(Node* head, Node* searched_node)
{
    bool is_existing = false;
    Node* node;

    if (head == searched_node)
    {
        is_existing = true;
    }
    else if (head != NULL && searched_node != NULL)
    {
        EZMLL_FOR_EACH(node, head)
        {
            if (node == searched_node)
            {
                is_existing = true;
                break;
            }
        }
    }
    else
    {
        /* could not find node, return false */
    }

    return is_existing;
}

/******************************************************************************
* Function : ezmLL_UnlinkNode
*//**
* \b Description:
*
* Unlinked a node
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *unlinked_node: (IN)pointer to the node, which will be unlinked
* @return   None
*
*******************************************************************************/
void ezmLL_UnlinkNode(Node * unlinked_node)
{
    UNLINK_NODE(unlinked_node);
}


#if 0
Node* ezmLL_GetFreeNode(void)
{
    Node *free_node = NULL;

    for (uint16_t i = 0; i < NUM_OF_NODE; i++)
    {
        if (node_pool[i].u16NodeIndex == NODE_INVALID_ID)
        {
            /* store its own index for eaiser look up*/
            node_pool[i].u16NodeIndex = i;

            free_node = &node_pool[i];
            break;
        }
    }

    return free_node;
}

void ezmLL_ResetNode(Node * node)
{
    if (NULL != node && node->u16NodeIndex < NUM_OF_NODE)
    {
        node->pstNextNode = NULL;
        node->pstPrevNode = NULL;
        node->u16NodeIndex = NODE_INVALID_ID;
    }
}
#endif

#endif /* HELPER_LINKEDLIST */
/* End of file*/