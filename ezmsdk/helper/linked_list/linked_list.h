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
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Meta data of a node
 *      Contain the pointer to the next node and pointer to the buffer 
 */
typedef struct Node
{
    uint8_t* pBuffer;          /**< pointer to the buffer storing data*/
    uint16_t u16BufferSize;     /**< size of buffer*/
    uint16_t u16NodeIndex;      /**< Store the node index*/
    struct Node * pstNextNode;  /**< pointer to the next node in a linked list*/
    struct Node * pstPrevNode;  /**< pointer to the previous node in a linked list*/
}Node;

/** @brief Meta data of the linked list
 *      Contain the size and pointer to the first node
 */
typedef struct LinkedList
{
    Node * pstHead;     /**< pointer to the head of linked list*/
    Node * pstTail;     /**< pointer to the tail of linked list*/
    uint16_t u16Size;   /**< store the size of the linked list*/
}LinkedList;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void        ezmLL_Initialization                (void);
void        LinkedList_InsertToHead             (LinkedList *pstList, Node *pstNewNode);                          /*Add a node to the head*/
Node       *LinkedList_RemoveFromHead           (LinkedList *pstList);                                             /*Remove the head node*/
void        LinkedList_InsertToTail             (LinkedList *pstList, Node *pstNewNode);                          /*Add a node to the tail*/
Node       *LinkedList_RemoveFromTail           (LinkedList *pstList);                                             /*Remove the tail node*/
bool        LinkedList_InsertNewNodeAfterANode  (LinkedList *pstList, Node *pstCurrentNode, Node *pstNewNode);
Node       *LinkedList_SearchNode               (LinkedList *pstList, Node *pstSearchNode);
bool        LinkedList_RemoveNode               (LinkedList *pstList, Node *pstRemovedNode);
Node        *ezmLL_GetFreeNode                  (void);
void        ezmLL_ResetNode                     (Node *node);

#endif /* _LINKEDLIST_H */

/* End of file*/
