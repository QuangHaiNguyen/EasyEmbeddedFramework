/*******************************************************************************
* Title                 :   linked_list
* Filename              :   linked_list.h
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
/** @file   linked_list.h
 *  @brief  Header of the linked_list
 */

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "../../app/app_config.h"

#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/*@brief: iterate thru a link list starting from head
 *
 */
#define EZMLL_FOR_EACH(node,head)               for(node = head->next; node != head; node = node->next)

/*@brief: initialize a node
 *
 */
#define EZMLL_INIT_NODE(name)                   {&(name), &(name)}

/* It deserves a whole story. Generally speaking, it use the same concept 
 * as in the linked list of the linux kernel
 * (type*)0 cast address 0 (zero) to data type "type"
 * (type*)0)->member access the member of type "type"
 * ((char*)&(((type*)0)->member) - (char*)((type*)0)): minus the address of the member and the addres 0
 */
#define OFFSET(type, member)    ((char*)&(((type*)0)->member) - (char*)((type*)0))

/* This one is easy, see OFFSET */
#define EZMLL_GET_PARENT_OF(ptr,member,type)   (type*)((char*)ptr - OFFSET(type, member))


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Meta data of a node
 *      Contain the pointer to the next node and pointer to the buffer 
 */
typedef struct Node
{
    //uint16_t u16NodeIndex;      /**< Store the node index*/
    struct Node * next;  /**< pointer to the next node in a linked list*/
    struct Node * prev;  /**< pointer to the previous node in a linked list*/
}Node;

#if 0
/** @brief Meta data of the linked list
 *      Contain the size and pointer to the first node
 */
typedef struct LinkedList
{
    Node * pstHead;     /**< pointer to the head of linked list*/
    uint16_t offset;
}LinkedList;
#endif

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmLL_Initialization        (void);
bool        ezmLL_AppendNode            (Node *new_node, Node *appended_node);
Node        *ezmLL_InsertNewHead        (Node * current_head, Node *new_node);
Node        *ezmLL_UnlinkCurrentHead    (Node *head);
bool        ezmLL_IsNodeInList          (Node *head, Node *searched_node);
void        ezmLL_UnlinkNode            (Node *unlinked_node);
//Node        *ezmLL_GetFreeNode                  (void);
//void        ezmLL_ResetNode                     (Node *node);

#endif /* _LINKEDLIST_H */

/* End of file*/
