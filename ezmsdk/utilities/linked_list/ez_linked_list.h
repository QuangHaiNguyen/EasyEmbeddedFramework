/*****************************************************************************
* Filename:         ez_linked_list.h
* Author:           Hai Nguyen
* Original Date:    19.02.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_linked_list.h
 *  @author Hai Nguyen
 *  @date   19.02.2024
 *  @brief  Public functions of the linked list component
 *
 *  @details Linked list data structure. This implementation is inspired by
 *  the one in the linux kernel.
 * 
 */

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "ezSdk_config.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/

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

/*@brief Get the parent of the data structure, where the linked list is embedded
 *
 */
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

/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief Meta data of a node. Containing the pointer to the next node and
 *  previous node
 */
struct Node
{
    struct Node * next;  /**< pointer to the next node in a linked list*/
    struct Node * prev;  /**< pointer to the previous node in a linked list*/
};


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function : ezmLL_Initialization
*//** 
* @brief Inititialize the linked list component
*
* @details This function has empty body (aka, there is no implementation)
*
* @return   None
*
* @pre None
* @post None
*
* @code
* ezmLL_Initialization();
* @endcode
*
*****************************************************************************/
void ezmLL_Initialization(void);


/*****************************************************************************
* Function : ezmLL_InitNode
*//** 
* @brief Initialize a new node
*
* @details The indication for new node is that, both head and tail point to
*          itself
*
* @param    node: (IN)pointer to node
* @return   None
*
* @pre None
* @post Node is intialized.
*
* @code
* struct Node new_node;
* ezmLL_InitNode(&new_node);
* @endcode
*
*
*****************************************************************************/
void ezmLL_InitNode (struct Node* node);


/*****************************************************************************
* Function : ezmLL_GetListSize
*//** 
* @brief Return number of node in a list
*
* @details This function transverses the linked list and count the number
* of nodes
*
* @param    list_head: (IN)pointer to the head of the list
* @return   number of node
*
* @pre None
* @post None
*
* @code
* uint16_t size = ezmLL_GetListSize(head);
* @endcode
*
*****************************************************************************/
uint16_t ezmLL_GetListSize(struct Node* list_head);


/*****************************************************************************
* Function : ezmLL_AppendNode
*//** 
* @brief Append a node after a node
*
* @details Detail description
*
* @param    new_node: (IN)pointer to the new node
* @param    appended_node: (IN)pointer to the node, which new node will be
                           appended to
* @return   true if success, else false
*
* @pre None
* @post None
*
* @code
* struct Node new_node;
* struct Node appended_node;
* ezmLL_InitNode(&new_node);
* ezmLL_InitNode(&appended_node);
* ezmLL_AppendNode(&new_node, &appended_node);
* @endcode
*
* @see ezmLL_InitNode
*
*****************************************************************************/
bool ezmLL_AppendNode(struct Node *new_node, struct Node *appended_node);


/*****************************************************************************
* Function : ezmLL_InsertNewHead
*//** 
* @brief Add a new node to the current head position
*
* @details New node will be the head of the linked list
*
* @param    current_head: (IN)pointer to the current head
* @param    new_node: (IN)pointer to the new node to be inserted
* @return   pointer to the new head or NULL if operation fails
*
* @pre None
* @post None
*
* @code
* struct Node new_node;
* ezmLL_InitNode(&new_node);
* struct Node *new_head = ezmLL_InsertNewHead(&new_node, current_head);
* @endcode
*
* @see ezmLL_InitNode
*
*****************************************************************************/
struct Node *ezmLL_InsertNewHead (struct Node * current_head,
                                  struct Node *new_node);


/*****************************************************************************
* Function : ezmLL_UnlinkCurrentHead
*//** 
* @brief Unlink the current head of the linked list
*
* @details After the operation, the new head position will be changed to
* head->next
*
* @param    head: (IN)pointer to the current head
* @return   pointer to the unlinked head or NULL
*
* @pre None
* @post None
*
* @code
* struct Node *unlinked_head = ezmLL_UnlinkCurrentHead(head);
* @endcode
*
*****************************************************************************/
struct Node *ezmLL_UnlinkCurrentHead(struct Node *head);


/*****************************************************************************
* Function : ezmLL_IsNodeInList
*//** 
* @brief Check if a node is in a list
*
* @details
*
* @param    head: (IN)pointer to head of the list
* @param    searched_node: (IN)node to be checked
* @return   true if existing, else false
*
* @pre None
* @post none
*
* @code
* bool exist = ezmLL_IsNodeInList(list, &checked_node);
* @endcode
*
*****************************************************************************/
bool ezmLL_IsNodeInList(struct Node *head, struct Node *searched_node);

#endif /* _LINKEDLIST_H */

/* End of file*/
