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
#include "app/app_config.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/*@brief: iterate thru a link list starting from head
 *
 */
#define EZMLL_FOR_EACH(node,head)               for(node = (head)->next; node != head; node = (node)->next)

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
#define OFFSET(type, member) ((char*)&(((type*)0)->member) - (char*)((type*)0))

/* This one is easy, see OFFSET */
#define EZMLL_GET_PARENT_OF(ptr,member,type) (type*)((char*)ptr - OFFSET(type, member))

/*@brief advance a node to next node
 *
 */
#define EZMLL_TO_NEXT_NODE(node) node = node->next

/*@brief Insert a node to head
 *
 */
#define EZMLL_ADD_HEAD(list_head,node) ezmLL_AppendNode(node, (list_head)->next)

/*@brief Insert a node to tail
  *
 */
#define EZMLL_ADD_TAIL(list_head,node) ezmLL_AppendNode(node, (list_head)->prev)

/*@brief unlink a node
 *
 */
#define EZMLL_UNLINK_NODE(node) (node)->prev->next = (node)->next;(node)->next->prev = (node)->prev;ezmLL_InitNode(node);

/*@brief check if a list is empty
 *
 */
#define IS_LIST_EMPTY(list_head) ((list_head)->next == (list_head))

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Meta data of a node
 *      Contain the pointer to the next node and pointer to the buffer 
 */
struct Node
{
    struct Node * next;  /**< pointer to the next node in a linked list*/
    struct Node * prev;  /**< pointer to the previous node in a linked list*/
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmLL_Initialization        (void);
void        ezmLL_InitNode              (struct Node* node);
uint16_t    ezmLL_GetListSize           (struct Node* list_head);
bool        ezmLL_AppendNode            (struct Node *new_node, struct Node *appended_node);
struct Node *ezmLL_InsertNewHead        (struct Node * current_head, struct Node *new_node);
struct Node *ezmLL_UnlinkCurrentHead    (struct Node *head);
bool        ezmLL_IsNodeInList          (struct Node *head, struct Node *searched_node);

#endif /* _LINKEDLIST_H */

/* End of file*/
