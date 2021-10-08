/*******************************************************************************
* Title                 :   ezmIpc 
* Filename              :   ezmIpc.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 09.08.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  ezmIpc.c
 *  @brief This is the source template for a ezmIpc
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmIpc.h"
#include "ezmIpc_conf.h"

#if (SMALLOC == 0U)
#error SMALLOC module must be activated
#else
#include "../helper/smalloc/smalloc.h"
#endif

#if (MODULE_DEBUG == 1U) && (IPC_DEBUG == 1U)
    #define IPCPRINT1(a)                    PRINT_DEBUG1(a)               
    #define IPCPRINT2(a,b)                  PRINT_DEBUG2(a,b)             
    #define IPCPRINT3(a,b,c)                PRINT_DEBUG3(a,b,c) 
#else 
    #define IPCPRINT1(a)           
    #define IPCPRINT2(a,b)           
    #define IPCPRINT3(a,b,c)
#endif

#define NUM_OF_MODULE       sizeof(au8RegisteredModule)
#define IPC_ID              0x0BU

REGISTER_ASSERT(IPC_ID)


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static ezmMemList astRegisteredMemList[NUM_OF_MODULE];
/**< Store list of memory lists*/

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool ezmIsModuleRegistered(uint8_t u8ModuleId);
static uint8_t ezmSearchModule(uint8_t u8ModuleId);

/******************************************************************************
* Function : ezmIpc_Init
*//** 
* \b Description:
*
* This function initializes the memory list of each registered moduled.
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
* @return   None
*
* \b Example Example:
* @code
* ezmIpc_Init()
* @endcode
*
* @see sum
*
*******************************************************************************/
void ezmIpc_Init(void)
{
    ASSERT(NUM_OF_MODULE > 0)
    
    for(uint8_t i = 0; i < NUM_OF_MODULE; i++)
    {
        /* Parse module id into the mem list*/
        IPCPRINT2("Init module: %x", au8RegisteredModule[i]);
        ezmSmalloc_InitMemList(&astRegisteredMemList[i], au8RegisteredModule[i]);
    }
}

/******************************************************************************
* Function : ezmIpc_RegisterModule
*//** 
* \b Description:
*
* Register a module into IPC service. Actually, the module is registered in
* compiled time,so it just checks if the module is registered-
*
* PRE-CONDITION: ezmIpc_Init() must be called first
*
* POST-CONDITION: None
* 
* @param    u8ModuleId: (IN)module id
* @return   true if module is registered
*
* \b Example Example:
* @code
* ezmIpc_Init();
* bool bResult = ezmIpc_RegisterModule(0x01U);
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmIpc_RegisterModule(uint8_t u8ModuleId)
{
    bool bReturn = false;

    for(uint8_t i = 0; i < NUM_OF_MODULE; i++)
    {
        if(ezmIsModuleRegistered(u8ModuleId))
        {
            bReturn = true;
            break;
        }
    }

    return bReturn;
}

/******************************************************************************
* Function : ezmIpc_InitMsg
*//** 
* \b Description:
*
* Get a message with size
*
* PRE-CONDITION: IPC module is initialized and the module is registered
*
* POST-CONDITION: None
* 
* @param    u8MsgSize: (IN)size of the payload in byte
* @return   pointer to the memory block, which stores the message
*
* \b Example Example:
* @code
* ezmIpc_Init();
* bool bResult = ezmIpc_RegisterModule(0x01U);
* ezmMemoryBlock * pstMsg = ezmIpc_InitMsg(2);
* pstMsg->pBuffer[0] = 0x00;
* pstMsg->pBuffer[1] = 0x01;
* @endcode
*
* @see sum
*
*******************************************************************************/
ezmMemoryBlock * ezmIpc_InitMsg(uint8_t u8MsgSize)
{
    ezmMemoryBlock * pstMsg;
    pstMsg = ezmSmalloc_GetFreeBlock(u8MsgSize);
    IPCPRINT1(" Init a message");
    return pstMsg;
}

/******************************************************************************
* Function : ezmIpc_DeInitMsg
*//** 
* \b Description:
*
* This function free a message which is not used by the module
*
* PRE-CONDITION: IPC module is initialized and the module is registered
*
* POST-CONDITION: None
* 
* @param    u8ModuleId: (IN)module id
* @param    pstMsg: (IN)pointer to the message
* @return   true if success, else false
*
* \b Example Example:
* @code
* ezmIpc_Init();
* bool bResult = ezmIpc_RegisterModule(0x02U);
* ezmMemoryBlock * pstMsg = ezmIpc_ReceiveMsg(0x02U);
* if(pstMsg != NULL)
* {
*     printf("Yay have a message!");
*     ezmIpc_DeInitMsg(0x02U, pstMsg);   
* }
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmIpc_DeInitMsg(uint8_t u8ModuleId, ezmMemoryBlock* pstMsg)
{
    bool bReturn = false;
    uint8_t u8ModuleIndex = ezmSearchModule(u8ModuleId);

    if(u8ModuleIndex != 0xFF)
    {
        ezmSmalloc_ReturnMemBlock(&astRegisteredMemList[u8ModuleIndex], pstMsg);
        bReturn = true;
    }

    return bReturn;
}


/******************************************************************************
* Function : ezmIpc_SendMsg
*//** 
* \b Description:
*
* This function send the message from one module to another
*
* PRE-CONDITION: IPC module is initialized, the module is registered, and a 
*                message is ready to send
*
* POST-CONDITION: None
* 
* @param    u8FromModuleId: (IN)send module id
* @param    u8ToModuleId:   (IN)receive module id
* @param    pstMsg:         (IN)pointer to the message
* @return   None
*
* \b Example Example:
* @code
* ezmIpc_Init();
* bool bResult = ezmIpc_RegisterModule(0x01U);
* ezmMemoryBlock * pstMsg = ezmIpc_InitMsg(2);
* pstMsg->pBuffer[0] = 0x00;
* pstMsg->pBuffer[1] = 0x01;
* ezmIpc_SendMsg(0x01, 0x02, pstMsg);
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmIpc_SendMsg (uint8_t u8FromModuleId, uint8_t u8ToModuleId, ezmMemoryBlock* pstMsg)
{
    bool bReturn = false;
    uint8_t u8IndexFrom = ezmSearchModule(u8FromModuleId);
    uint8_t u8IndexTo = ezmSearchModule(u8ToModuleId);

    if(u8IndexFrom != 0xFF && u8IndexTo != 0xFF)
    {
        ezmSmalloc_ApendBlockToList(pstMsg, &astRegisteredMemList[u8IndexTo]);
        IPCPRINT3("Send message from %x to %x", u8FromModuleId, u8ToModuleId);
        bReturn = true;
    }
    return bReturn;    
}


/******************************************************************************
* Function : ezmIpc_ReceiveMsg
*//** 
* \b Description:
*
* This function receives the message sent by other module
*
* PRE-CONDITION:  IPC module is initialized, the module is registered
*
* POST-CONDITION: None
* 
* @param    u8ModuleId: (IN)pointer to the ring buffer
* @return   pointer to the message
*
* \b Example Example:
* @code
* ezmIpc_Init();
* bool bResult = ezmIpc_RegisterModule(0x02U);
* ezmMemoryBlock * pstMsg = ezmIpc_ReceiveMsg(0x02U);
* if(pstMsg != NULL)
* {
*     printf("Yay have a message!");
*     ezmIpc_DeInitMsg(0x02U, pstMsg);   
* }
* @endcode
*
* @see sum
*
*******************************************************************************/
ezmMemoryBlock * ezmIpc_ReceiveMsg (uint8_t u8ModuleId)
{
    ezmMemoryBlock * pstMsg = NULL;
    uint8_t u8ModuleIndex = ezmSearchModule(u8ModuleId);

    if(u8ModuleIndex != 0xFF)
    {
        IPCPRINT1("Get a message!");
        pstMsg = astRegisteredMemList[u8ModuleIndex].pstHead;
    }
    return pstMsg;
}


/******************************************************************************
* Function : ezmIsModuleRegistered
*//** 
* \b Description:
*
* Check if a module is registered
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u8ModuleId: (IN)module id
* @return   true if registered, otherwise false
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static bool ezmIsModuleRegistered(uint8_t u8ModuleId)
{
    bool bReturn = false;

    for(uint8_t i = 0; i < NUM_OF_MODULE; i++)
    {
        if(u8ModuleId == au8RegisteredModule[i])
        {
            IPCPRINT1("module found! Registered!");
            bReturn = true;
            break;
        }
    }
    return bReturn;
}


/******************************************************************************
* Function : ezmSearchModule
*//** 
* \b Description:
*
* This function searches for the index of the mem list of a module id
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u8ModuleId: (IN)module id
* @return   module id if found otherwise oxff
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static uint8_t ezmSearchModule(uint8_t u8ModuleId)
{
    uint8_t u8Index = 0xFF;

    for(uint8_t i = 0; i < NUM_OF_MODULE; i++)
    {
        if(astRegisteredMemList[i].u8ModuleId == u8ModuleId)
        {
            u8Index = i;
            IPCPRINT2("module found at index: %d", u8Index);
            break;
        }
    }
    return u8Index;
}


/* End of file*/