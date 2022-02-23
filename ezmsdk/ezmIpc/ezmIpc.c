/*******************************************************************************
* Title                 :   ezmIpc 
* Filename              :   ezmIpc.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   03.02.2022
*
*******************************************************************************/

/** @file  ezmIpc.c
 *  @brief This is the source for the ezmIpc module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmIpc.h"

#if (IPC == 1U)
#include "../helper/linked_list/linked_list.h"
#include "../helper/stcmem/stcmem.h"
#include "ezmIpc_conf.h"
#include "string.h"

#define MOD_NAME        "IPC"

#if (MODULE_DEBUG == 1U) && (IPC_DEBUG == 1U)
    #define IPCPRINT(a)                     PRINT_DEBUG(MOD_NAME,a)
    #define IPCPRINT1(a,b)                  PRINT_DEBUG1(MOD_NAME,a,b)
    #define IPCPRINT2(a,b,c)                PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define IPCPRINT3(a,b,c,d)              PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define IPCPRINT4(a,b,c,d,e)            PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#else 
    #define IPCPRINT(a)
    #define IPCPRINT1(a,b)
    #define IPCPRINT2(a,b,c)
    #define IPCPRINT3(a,b,c,d)
    #define IPCPRINT4(a,b,c,d,e)
#endif

/**@brief Get the MemBlock structure from the node
 *
 */
#define GET_BLOCK(node_ptr) (EZMLL_GET_PARENT_OF(node_ptr, node, struct MemBlock))

/**@brief Get the instance from the ezmIpc type
 *
 */
#define GET_INSTANCE(ipc) ((ipc<NUM_OF_IPC_INSTANCE) ? &instance_pool[ipc] : NULL)
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**@brief structure define an IPC instance
 *
 */
typedef struct
{
    bool        is_busy;                /**< Store the id of the owner of the instance */
    ezmMemList  memory_list;            /**< Memory list to manage the buffer of the ipc instance*/
    struct Node pending_list_head;      /**< list contains message pending to be sent*/
    ezmIpc_MessageCallback fnCallback;  /**< Callback function */
}IpcInstance;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static IpcInstance instance_pool[NUM_OF_IPC_INSTANCE] = {0};    /**< pool of ipc intance*/

/******************************************************************************
* Function Definitions
*******************************************************************************/

/**Function to manipulate the buffer of the ipc. Keyword extern is used because these 
 * functions are not meant to be shared with the user
 */
extern inline struct Node*      ezmStcMem_ReserveMemoryBlock(struct Node* free_list_head, uint16_t block_size_byte);
extern inline bool              ezmStcMem_MoveBlock(struct Node* move_node, struct Node* from_list_head, struct Node* to_list_head);
extern inline struct MemBlock*  GetFreeBlock(void);
extern inline void              ReleaseBlock(struct MemBlock* block);
static void                     ezmIpc_ResetInstance    (uint8_t instance_index);

/******************************************************************************
* Function : ezmIpc_InitModule
*//** 
* \b Description:
*
* This function initializes the IPC module. It reset all of ipc instances in the pools
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
* @return   None
*
*******************************************************************************/
void ezmIpc_InitModule(void)
{
    for (uint8_t i = 0; i < NUM_OF_IPC_INSTANCE; i++)
    {
        ezmIpc_ResetInstance(i);
    }
}

/******************************************************************************
* Function : ezmIpc_GetInstance
*//**
* \b Description:
*
* Get a free instance from the Ipc pool and init it according to the parameters
*
* PRE-CONDITION: IPC module must be init
*
* POST-CONDITION: None
*
* @param    *ipc_buffer:    pointer to the providing buffer for the instance
* @param    buffer_size:    size of the buffer in byte
* @param    fnCallback:     callback function, tell the owner that it receives a message
*
* @return   handle to the ipc instance
*
*******************************************************************************/
ezmIpc ezmIpc_GetInstance(uint8_t* ipc_buffer, uint16_t buffer_size, ezmIpc_MessageCallback fnCallback)
{
    ezmIpc free_instance = NUM_OF_IPC_INSTANCE;
    for (uint8_t i = 0; i < NUM_OF_IPC_INSTANCE; i++)
    {
        if (instance_pool[i].is_busy == false)
        {
            instance_pool[i].is_busy = true;
            instance_pool[i].fnCallback = fnCallback;
            instance_pool[i].memory_list.buff = ipc_buffer;
            instance_pool[i].memory_list.buff_size = buffer_size;
            (void)ezmStcMem_InitMemList(&instance_pool[i].memory_list, ipc_buffer, buffer_size);
            free_instance = (ezmIpc)i;
            break;
        }
    }
    return free_instance;
}

/******************************************************************************
* Function : ezmIpc_InitMessage
*//**
* \b Description:
*
* Init a message and return the address of the buffer for usage. it reseves a memory
* block in the buffer of the ipc instance and return it to the users so they can write
* data into the block.
*
* PRE-CONDITION: IPC instance must be exsisting
*
* POST-CONDITION: None
*
* @param    send_to:        ipc handle, which the message will be sent to
* @param    size_in_byte    size of the message in byte
*
* @return   address of the buffer
*           NULL error
*
*******************************************************************************/
void* ezmIpc_InitMessage(ezmIpc send_to, uint16_t size_in_byte)
{
    void            *buffer_address = NULL;
    IpcInstance     *send_to_instance = NULL;
    struct Node     *pending_node = NULL;

    if (size_in_byte > 0 && send_to < NUM_OF_IPC_INSTANCE)
    {
        send_to_instance = GET_INSTANCE(send_to);
        pending_node = ezmStcMem_ReserveMemoryBlock(&send_to_instance->memory_list.free_list_head, size_in_byte);
        if (NULL != pending_node)
        {
            buffer_address = GET_BLOCK(pending_node)->buff;
            EZMLL_UNLINK_NODE(pending_node);
            EZMLL_ADD_TAIL(&send_to_instance->pending_list_head, pending_node);
        }
    }

    return buffer_address;
}

/******************************************************************************
* Function : ezmIpc_SendMessage
*//**
* \b Description:
*
* "Send" the message to the module. Send action means in move the memory header from
* free list to the allocated list. If a callback is set, it will trigger the owner that
* a message is sent.
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    send_to:    ipc handle, which the message will be sent to
* @param    *message:   pointer the message
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmIpc_SendMessage(ezmIpc send_to, void *message)
{
    bool        is_success = false;
    IpcInstance *send_to_instance = NULL;
    struct Node *it_node = NULL;

    if (NULL != message && send_to < NUM_OF_IPC_INSTANCE)
    {
        send_to_instance = GET_INSTANCE(send_to);
        EZMLL_FOR_EACH(it_node, &send_to_instance->pending_list_head)
        {
            if (GET_BLOCK(it_node)->buff == message)
            {
                ezmStcMem_MoveBlock(it_node, &send_to_instance->pending_list_head, &send_to_instance->memory_list.alloc_list_head);
                IPCPRINT("message sent");
                if (NULL != send_to_instance->fnCallback)
                {
                    send_to_instance->fnCallback();
                }
                is_success = true;
                break;
            }
        }
    }

    return is_success;
}

/******************************************************************************
* Function : ezmIpc_ReceiveMessage
*//**
* \b Description:
*
* This function check the buffer and return the message if there is one. Note calling
* this function only return the message. After working with the message, ezmIpc_ReleaseMessage must
* be called to actually free the message from the buffer
*
* PRE-CONDITION: instance must be exist
*
* POST-CONDITION: None
*
* @param    receive_from:   the handle, which message will be read out
* @param    *message_size:  size of the message
*
* @return   address of the message if there is one
*
*******************************************************************************/
void* ezmIpc_ReceiveMessage(ezmIpc receive_from, uint16_t *message_size)
{
    void        *buffer_address = NULL;
    IpcInstance *instance = NULL;

    if (receive_from < NUM_OF_IPC_INSTANCE)
    {
        instance = GET_INSTANCE(receive_from);
   
        if (!IS_LIST_EMPTY(&instance->memory_list.alloc_list_head))
        {
            buffer_address = GET_BLOCK(instance->memory_list.alloc_list_head.next)->buff;
            *message_size = GET_BLOCK(instance->memory_list.alloc_list_head.next)->buff_size;
        }
    }
    return buffer_address;
}

/******************************************************************************
* Function : ezmIpc_ReleaseMessage
*//**
* \b Description:
*
* Free the message in the buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    receive_from: the handle, which message will be read out
* @param    *message:   message to be free
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmIpc_ReleaseMessage(ezmIpc receive_from, void* message)
{
    bool        is_success = true;
    void        *buffer_address = NULL;
    IpcInstance *instance = NULL;

    if (message != NULL && receive_from < NUM_OF_IPC_INSTANCE)
    {
        instance = GET_INSTANCE(receive_from);
        is_success = is_success && ezmStcMem_Free(&instance->memory_list, message);
    }

    return is_success;
}

/******************************************************************************
* Function : ezmIpc_ResetInstance
*//**
* \b Description:
*
* Reset a ipc instance to init state
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    instance_index; index of the instance
* @return   None
*
*******************************************************************************/
static void ezmIpc_ResetInstance(uint8_t instance_index)
{
    if (instance_index < NUM_OF_IPC_INSTANCE)
    {
        instance_pool[instance_index].is_busy = false;
        instance_pool[instance_index].fnCallback = NULL;
 
        instance_pool[instance_index].pending_list_head.next = &instance_pool[instance_index].pending_list_head;
        instance_pool[instance_index].pending_list_head.prev = &instance_pool[instance_index].pending_list_head;

        memset(&instance_pool[instance_index].memory_list, 0, sizeof(instance_pool[instance_index].memory_list));
    }
}

#endif /* IPC */

/* End of file*/