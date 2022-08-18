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

#if (CONFIG_HELPER_LINKEDLIST == 1U)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

#define NODE_INVALID_ID     0xFFFF

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//static struct Node node_pool[NUM_OF_NODE] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/

/******************************************************************************
* Function : ezmLL_Initialization
*//**
* \b Description:
*
* Initialize linked list module. Do nothing at the moment
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezmLL_Initialization(void)
{
    /* Do nothing */
}

/******************************************************************************
* Function : ezmLL_InitNode
*//**
* \b Description:
*
* Initialize a new node
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    node: (IN) node to be initialized
* @return   None
*
*******************************************************************************/
void ezmLL_InitNode(struct Node* node)
{
    node->next = node;
    node->prev = node;
}

/******************************************************************************
* Function : ezmLL_GetListSize
*//**
* \b Description:
*
* Return number of node in a list
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    list_head: (IN)pointer to the head of the list
* @return   number of node
*
*******************************************************************************/
uint16_t ezmLL_GetListSize(struct Node* list_head)
{
    uint16_t size = 0;
    struct Node* it_node = NULL;
 
    EZMLL_FOR_EACH(it_node, list_head)
    {
        size++;
    }
    return size;
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
bool ezmLL_AppendNode(struct Node* new_node, struct Node* node)
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
struct Node* ezmLL_InsertNewHead(struct Node *current_head, struct Node * new_node)
{
    struct Node* new_head = NULL;

    if(current_head != NULL 
        && new_node != NULL
        && ezmLL_AppendNode(new_node, current_head->prev) == true)
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
struct Node * ezmLL_UnlinkCurrentHead(struct Node * head)
{
    struct Node* new_head = head;

    if(head->next != head)
    {
        new_head = head->next;
        EZMLL_UNLINK_NODE(head);
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
bool ezmLL_IsNodeInList(struct Node* head, struct Node* searched_node)
{
    bool is_existing = false;
    struct Node* node;

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

#endif /* CONFIG_HELPER_LINKEDLIST */
/* End of file*/