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

#ifndef _MODULE_H
#define _MODULE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "../../ezmDebug/ezmDebug.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO             1
/**< a macro*/
 
#if (MODULE_DEBUG == 1U) && (HELPER_LINKEDLIST_DEBUG == 1U)
    #define PRINT_LIST_METADATA(x)      LinkedList_PrintLinkedListMetaData(x)
    #define PRINT_NODE_METADATA(x)      LinkedList_PrintNodeMetaData(x)
    #define PRINT_LIST_FORWARD(x)       LinkedList_PrintListForward(x)
    #define PRINT_LIST_BACKWARD(x)      LinkedList_PrintListBackward(x)
#else
    #define PRINT_LIST_METADATA(x)
    #define PRINT_NODE_METADATA(x)
    #define PRINT_LIST_FORWARD(x)
    #define PRINT_LIST_BACKWARD(x)
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Meta data of a node
 *      Contain the pointer to the next node and pointer to the buffer 
 */
typedef struct Node
{
    struct Node * pstNextNode;
    /**< pointer to the next node in a linked list*/
    struct Node * pstPrevNode;
    /**< pointer to the previous node in a linked list*/
    uint8_t * pBuffer;
    /**< pointer to the buffer storing data*/
    uint16_t u16BufferSize;
    /**< size of buffer*/
#if (HELPER_LINKEDLIST_DEBUG == 1)
    uint16_t u8NodeId;
    /**< Store the node ID, only for debug mode*/
#endif
}Node;

/** @brief Meta data of the linked list
 *      Contain the size and pointer to the first node
 */
typedef struct LinkedList
{
    Node * pstHead;
    /**< pointer to the head of linked list*/
    Node * pstTail;
    /**< pointer to the tail of linked list*/
    uint16_t u16Size;
    /**< store the size of the linked list*/
    uint8_t u8ModuleId;
    /**< store the module id, used in block linked list*/
}LinkedList;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/

void LinkedList_InsertToHead(LinkedList * pstList, Node * pstNewNode);                                  /*Add a node to the head*/
Node * LinkedList_RemoveFromHead(LinkedList * pstList);                                                 /*Remove the head node*/
void LinkedList_InsertToTail(LinkedList * pstList, Node * pstNewNode);                                  /*Add a node to the tail*/
Node * LinkedList_RemoveFromTail(LinkedList * pstList);                                                 /*Remove the tail node*/
Node * LinkedList_FindNodeAtIndex(LinkedList * pstList, uint16_t u16Index);                             /*Find node at given index*/
void LinkedList_InsertNodeAtIndex(LinkedList * pstList, Node * pstInsertedNode, uint16_t u16Index);     /*Insert node at given index*/
Node * LinkedList_RemoveNodeAtIndex(LinkedList * pstList, uint16_t u16Index);
bool LinkedList_InsertNewNodeAfterANode(LinkedList * pstList, Node * pstCurrentNode, Node * pstNewNode);
Node * LinkedList_SearchNode(LinkedList * pstList, Node * pstSearchNode);
bool LinkedList_RemoveNode(LinkedList * pstList, Node * pstRemovedNode);

#if SMALLOC_DEBUG == 1U
void LinkedList_PrintLinkedListMetaData(LinkedList * pstList);      /*Print linked list's metadata*/
void LinkedList_PrintNodeMetaData(Node * pstNode);                  /*Print node's metadata*/
void LinkedList_PrintListForward(LinkedList * pstList);             /*Print list from head -> tail*/
void LinkedList_PrintListBackward(LinkedList * pstList);            /*Print List from tail -> head*/
#endif

#endif /* _MODULE_H */

/* End of file*/
