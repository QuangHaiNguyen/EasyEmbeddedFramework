/*******************************************************************************
* Title                 :   link_list 
* Filename              :   link_list.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  link_list.c
 *  @brief This is the source template for a link_list
 */

/******************************************************************************
* Includes
*******************************************************************************/

#include "../../app/app_config.h"

#if (HELPER_LINKEDLIST == 1U)

#include "linked_list.h"
#include "stdlib.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MOD_NAME        "LL"

#if (MODULE_DEBUG == 1U) && (HELPER_LINKEDLIST_DEBUG == 1U)
    #define LLPRINT(a)                  PRINT_DEBUG(MOD_NAME,a)
    #define LLPRINT1(a,b)               PRINT_DEBUG1(MOD_NAME,a,b)
    #define LLPRINT2(a,b,c)             PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define LLPRINT3(a,b,c,d)           PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define LLPRINT4(a,b,c,d,e)         PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#else 
    #define LLPRINT(a)
    #define LLPRINT1(a,b)
    #define LLPRINT2(a,b,c)
    #define LLPRINT3(a,b,c,d)
    #define LLPRINT4(a,b,c,d,e)
#endif


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/

#if HELPER_LINKEDLIST_DEBUG == 1U
void LinkedList_PrintLinkedListMetaData(LinkedList * pstList);      /*Print linked list's metadata*/
void LinkedList_PrintNodeMetaData(Node * pstNode);                  /*Print node's metadata*/
void LinkedList_PrintListForward(LinkedList * pstList);             /*Print list from head -> tail*/
void LinkedList_PrintListBackward(LinkedList * pstList);            /*Print List from tail -> head*/
#endif

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
* Function : LinkedList_AddToHead
*//** 
* \b Description:
*
* Add a new node to the head of the linked list
*
* PRE-CONDITION: Linked list must be initialized
*
* POST-CONDITION: None
* 
* @param    pstList:    (IN)pointer to the linked list
* @param    pstNewNode: (IN)pointer to the new node
* @return   None
*
* \b Example Example:
* @code
* LinkedList stMyList;
* LinkedList_AddToHead(&stMyList, &stNewNode);
* @endcode
*
* @see sum
*
*******************************************************************************/
void LinkedList_InsertToHead(LinkedList * pstList, Node * pstNewNode)
{
    LLPRINT("LinkedList_InsertToHead");
    if(pstList != NULL && pstNewNode != NULL)
    {
        if(pstList->pstHead == NULL)
        {
            /** Has no head
             * it also implies that, list is new, aka it also has no tail
             */
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            pstNewNode->pstPrevNode = NULL; 
            pstNewNode->pstNextNode = NULL;
        }
        else
        {
            pstNewNode->pstNextNode = pstList->pstHead;    /*new node next node point to head*/
            pstNewNode->pstPrevNode = NULL;                /*new node prev node point to NULL*/
            pstList->pstHead->pstPrevNode = pstNewNode;    /*prev node of current head point to new node*/
            pstList->pstHead = pstNewNode;                 /*Update the head to new node*/
        }

        pstList->u16Size++;
    }

    PRINT_LIST_METADATA(pstList);

    return;
}

Node * LinkedList_RemoveFromHead(LinkedList * pstList)
{
    LLPRINT("LinkedList_RemoveFromHead");
    Node * pstRemovedNode;
    pstRemovedNode = pstList->pstHead;

    if(pstList != NULL)
    {
        if(pstRemovedNode != NULL)
        {
            PRINT_NODE_METADATA(pstRemovedNode);
            pstList->pstHead = pstList->pstHead->pstNextNode;
            
            /*Linked list is not empty*/
            if(pstList->pstHead != NULL)
            {
                pstList->pstHead->pstPrevNode = NULL;
            }
            else
            {
                pstList->pstTail = NULL;
            }
            pstList->u16Size--;
        }
    }

    PRINT_LIST_METADATA(pstList);

    return pstRemovedNode;
}

void LinkedList_InsertToTail(LinkedList * pstList, Node * pstNewNode)
{
    LLPRINT("LinkedList_InsertToTail");
    /*Only perform action when list and node are not Null*/
    if(pstList != NULL && pstNewNode != NULL)
    {
        if(pstList->pstTail == NULL)
        {
            /** Has no tail
             * it also implies that, list is new, aka it also has no head
             */
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            pstNewNode->pstPrevNode = NULL; 
            pstNewNode->pstNextNode = NULL;
        }
        else
        {
            pstNewNode->pstPrevNode = pstList->pstTail;    /*new node prev node point to tail*/
            pstNewNode->pstNextNode = NULL;                /*new node next node point to NULL*/
            pstList->pstTail->pstNextNode = pstNewNode;    /*Next node of current tail point to new node*/
            pstList->pstTail = pstNewNode;                 /*Update the tail to new node*/
        }

        pstList->u16Size++;
    }

    PRINT_LIST_METADATA(pstList);

    return;
}

Node * LinkedList_RemoveFromTail(LinkedList * pstList)
{
    LLPRINT("LinkedList_RemoveFromTail");
    Node * pstRemovedNode;
    pstRemovedNode = pstList->pstTail;

    if(pstList != NULL)
    {
        if(pstRemovedNode != NULL)
        {
            PRINT_NODE_METADATA(pstRemovedNode);
            pstList->pstTail = pstList->pstTail->pstPrevNode;
            
            /*Linked list is not empty*/
            if(pstList->pstTail != NULL)
            {
                pstList->pstTail->pstNextNode = NULL;
            }
            else
            {
                pstList->pstHead = NULL;
            }
            pstList->u16Size--;
        }
    }

    PRINT_LIST_METADATA(pstList);

    return pstRemovedNode;
}

Node * LinkedList_FindNodeAtIndex(LinkedList * pstList, uint16_t u16Index)
{
    LLPRINT("LinkedList_FindNodeAtIndex");
    Node * pstFoundNode = NULL;

    if(pstList != NULL)
    {
        if (u16Index < pstList->u16Size)
        {
            if(u16Index == 0U)
            {
                pstFoundNode = pstList->pstHead;
            }
            else if(u16Index == pstList->u16Size - 1U)
            {
                pstFoundNode = pstList->pstTail;
            }
            else if (u16Index > pstList->u16Size/2)
            {
                pstFoundNode = pstList->pstTail;
                for(uint16_t i = 0; i < pstList->u16Size - 1U - u16Index; i++)
                {
                    pstFoundNode = pstFoundNode->pstPrevNode;
                }
            }
            else
            {
                pstFoundNode = pstList->pstHead;
                for(uint16_t i = 0; i < u16Index; i++)
                {
                    pstFoundNode = pstFoundNode->pstNextNode;
                }
            }
        }
    }

    PRINT_NODE_METADATA(pstFoundNode);

    return pstFoundNode;
}


void LinkedList_InsertNodeAtIndex(LinkedList * pstList, Node * pstInsertedNode, uint16_t u16Index)
{
    LLPRINT("LinkedList_InsertNodeAtIndex");
    Node * pstFoundNode;

    if(u16Index == 0U)
    {
        LinkedList_InsertToHead(pstList, pstInsertedNode);
    }
    else if(u16Index == pstList->u16Size - 1U)
    {
        LinkedList_InsertToTail(pstList, pstInsertedNode);
    }
    else
    {
        /*Looking for the node in that index*/
        pstFoundNode = LinkedList_FindNodeAtIndex(pstList, u16Index);
        if(pstFoundNode != NULL)
        {
            /*Insert the node before the node at given index*/
            pstFoundNode->pstPrevNode->pstNextNode = pstInsertedNode;
            pstInsertedNode->pstPrevNode = pstFoundNode->pstPrevNode;

            pstFoundNode->pstPrevNode = pstInsertedNode;
            pstInsertedNode->pstNextNode = pstFoundNode;

            pstList->u16Size++;
        }
    }
    
    PRINT_LIST_METADATA(pstList);
    return;
}

Node * LinkedList_RemoveNodeAtIndex(LinkedList * pstList, uint16_t u16Index)
{
    LLPRINT("LinkedList_RemoveNodeAtIndex");
    Node * pstRemovedNode = NULL;

    if(u16Index == 0U)
    {
        pstRemovedNode = LinkedList_RemoveFromHead(pstList);
    }
    else if(u16Index == pstList->u16Size - 1U)
    {
        pstRemovedNode = LinkedList_RemoveFromTail(pstList);
    }
    else
    {
        /*Looking for the node in that index*/
        pstRemovedNode = LinkedList_FindNodeAtIndex(pstList, u16Index);
        if(pstRemovedNode != NULL)
        {
            /*unlink the remove node*/
            pstRemovedNode->pstPrevNode->pstNextNode = pstRemovedNode->pstNextNode;
            pstRemovedNode->pstNextNode->pstPrevNode =  pstRemovedNode->pstPrevNode;

            pstList->u16Size--;
        }
    }
    
    PRINT_LIST_METADATA(pstList);
    return pstRemovedNode;
}

Node * LinkedList_SearchNode(LinkedList * pstList, Node * pstSearchNode)
{
    Node * pstNode;
    pstNode = pstList->pstHead;

    while(pstNode != NULL)
    {
        if(pstNode == pstSearchNode)
        {
            break;
        }
        else
        {
            pstNode = pstNode->pstNextNode;
        }   
    }

    return pstNode;
}

bool LinkedList_InsertNewNodeAfterANode(LinkedList * pstList, Node * pstCurrentNode, Node * pstNewNode)
{
    bool bSuccess = false;
    Node * pstSearchNode = LinkedList_SearchNode(pstList, pstCurrentNode);

    if(pstSearchNode != NULL)
    {
        pstNewNode->pstPrevNode = pstSearchNode;
        pstNewNode-> pstNextNode = pstSearchNode->pstNextNode;
        pstSearchNode->pstNextNode = pstNewNode;
        
        if (pstNewNode->pstNextNode == NULL)
        {
            /* new node is at tail so we have to update the tail*/
            pstList->pstTail = pstNewNode;
        }
        

        pstList->u16Size++;
        bSuccess = true;
    }     

    return bSuccess;
}

bool LinkedList_RemoveNode(LinkedList * pstList, Node * pstRemovedNode)
{
    bool bSuccess = false;

    if(pstRemovedNode == pstList->pstHead)
    {
        LinkedList_RemoveFromHead(pstList);
        bSuccess = true;
    }
    else if (pstRemovedNode == pstList->pstTail)
    {
        LinkedList_RemoveFromTail(pstList);
        bSuccess = true;
    }
    else
    {
        Node * pstSearchNode = LinkedList_SearchNode(pstList, pstRemovedNode);
        if(pstSearchNode != NULL)
        {
            pstSearchNode->pstPrevNode->pstNextNode = pstSearchNode->pstNextNode;
            pstSearchNode->pstNextNode->pstPrevNode = pstSearchNode->pstPrevNode;
            pstList->u16Size--;
            bSuccess = true;
        }      
    }

    return bSuccess;
}

#if HELPER_LINKEDLIST_DEBUG == 1U
void LinkedList_PrintLinkedListMetaData(LinkedList * pstList)
{
    if(pstList != NULL)
    {
        PRINT2("list head: %p", (void*)pstList->pstHead);
        PRINT2("list tail: %p", (void*)pstList->pstTail);
        PRINT2("list size: %u", pstList->u16Size);    
    }
}

void LinkedList_PrintNodeMetaData(Node * pstNode)
{
    if(pstNode != NULL)
    {
        PRINT2("node @: %p", (void*)pstNode);
        PRINT2("node Id: %u", pstNode->u16NodeIndex);
        PRINT2("next node: %p", (void*)pstNode->pstNextNode);
        PRINT2("prev node: %p", (void*)pstNode->pstPrevNode);
    }
}

void LinkedList_PrintListForward(LinkedList * pstList)
{
    PRINT1("LinkedList_PrintListForward");
    Node * pstCurrentNode;
    pstCurrentNode = pstList->pstHead;

    while(pstCurrentNode != NULL)
    {
        PRINT_NODE_METADATA(pstCurrentNode);
        pstCurrentNode = pstCurrentNode->pstNextNode;
    }
}

void LinkedList_PrintListBackward(LinkedList * pstList)
{
    PRINT1("LinkedList_PrintListBackward");
    Node * pstCurrentNode;
    pstCurrentNode = pstList->pstTail;

    while(pstCurrentNode != NULL)
    {
        PRINT_NODE_METADATA(pstCurrentNode);
        pstCurrentNode = pstCurrentNode->pstPrevNode;
    }
}
#endif /* HELPER_LINKEDLIST_DEBUG */
#endif /* HELPER_LINKEDLIST */
/* End of file*/