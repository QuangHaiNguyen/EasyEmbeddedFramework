/*******************************************************************************
* Title                 :   ezmIpc 
* Filename              :   ezmIpc.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   03.02.2022
* Version               :   1.0.0
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

#define FREE_INSTANCE       0xFFU

#define NUM_OF_MODULE       sizeof(au8RegisteredModule)


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**@brief structure define an IPC instance
 *
 */
typedef struct
{
    uint8_t instance_owner_id;          /**< Store the id of the owner of the instance */
    ezmIpc_MessageCallback fnCallback;  /**< Callback function */
    ezmMemList memory_list;             /**< Memory list to manage the buffer of the ipc instance*/
}IpcInstance;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static IpcInstance instance_pool[NUM_OF_IPC_INSTANCE] = {0};    /**< pool of ipc intance*/

/******************************************************************************
* Function Definitions
*******************************************************************************/

/**Function to manipulate the buffer of the ipc extern is used because these 
 * functions are not meant to be shared with the user
 */
extern MemHdr*  ezmStcMem_ReserveMemoryBlock(LinkedList* free_list, uint16_t block_size_byte);
extern bool     ezmStcMem_MoveHeader        (MemHdr* header, LinkedList* from_list, LinkedList* to_list);

static void         ezmIpc_ResetInstance    (uint8_t instance_index);
static IpcInstance  *ezmIpc_GetFreeInstance (void);
static IpcInstance  *ezmIpc_SearchInstance  (uint8_t owner_id);
static bool         ezmIpc_IsInstanceExist  (uint8_t owner_id);

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
* @param    owner_id:       id of the owner of this IPC instance. MUST BE UNIQUE
* @param    *ipc_buffer:    pointer to the providing buffer for the instance
* @param    buffer_size:    size of the buffer in byte
* @param    fnCallback:     callback function, tell the owner that it receives a message
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmIpc_GetInstance(uint8_t owner_id, uint8_t *ipc_buffer, uint16_t buffer_size, ezmIpc_MessageCallback fnCallback)
{
    bool is_success = true;
    IpcInstance *new_instance = NULL;

    if (FREE_INSTANCE == owner_id || NULL == ipc_buffer || 0 == buffer_size)
    {
        is_success = false;
    }

    if (ezmIpc_IsInstanceExist(owner_id))
    {
        is_success = false;
    }

    if (is_success)
    {
        new_instance = ezmIpc_GetFreeInstance();
        if (NULL == new_instance)
        {
            is_success = false;
            PRINT_ERR("Dont have enough instance!!!");
        }
    }

    if (is_success)
    {
        new_instance->instance_owner_id = owner_id;
        new_instance->fnCallback = fnCallback;
        is_success = is_success & ezmStcMem_InitMemList(&new_instance->memory_list, ipc_buffer, buffer_size);
    }

    return is_success;
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
* @param    send_to_id:     the id of the module which we want to send the message to
* @param    size_in_byte    size of the message in byte
*
* @return   address of the buffer
*           NULL error
*
*******************************************************************************/
void* ezmIpc_InitMessage(uint8_t send_to_id, uint16_t size_in_byte)
{
    bool        is_success = true;
    void        *buffer_address = NULL;
    IpcInstance *send_to_instance = NULL;
    MemHdr      *reserved_header = NULL;

    if (0U == size_in_byte)
    {
        is_success = false;
    }

    if (is_success)
    {
        send_to_instance = ezmIpc_SearchInstance(send_to_id);

        if (NULL == send_to_instance)
        {
            is_success = false;
            PRINT_ERR1("[module = %x] does not exist", send_to_id);
        }
    }

    if (is_success)
    {
        reserved_header = ezmStcMem_ReserveMemoryBlock(&send_to_instance->memory_list.free_list, size_in_byte);
        if (NULL != reserved_header)
        {
            buffer_address = reserved_header->pBuffer;
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
* @param    send_to_id: id of the module which a message is sent to
* @param    *message:   pointer the message
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmIpc_SendMessage(uint8_t send_to_id, void *message)
{
    bool        is_success = true;
    IpcInstance *send_to_instance = NULL;
    MemHdr      *next_header = NULL;

    if (NULL == message)
    {
        is_success = false;
    }

    if (is_success)
    {
        send_to_instance = ezmIpc_SearchInstance(send_to_id);

        if (NULL == send_to_instance)
        {
            is_success = false;
        }
    }

    if (is_success)
    {
        next_header = send_to_instance->memory_list.free_list.pstHead;
        while (NULL != next_header)
        {
            if (next_header->pBuffer == message)
            {
                ezmStcMem_MoveHeader(next_header, &send_to_instance->memory_list.free_list, &send_to_instance->memory_list.alloc_list);
                IPCPRINT("message sent");
                break;
            }
            else
            {
                next_header = next_header->pstNextNode;
            }
        }
    }

    if (NULL == next_header)
    {
        is_success = false;
        PRINT_ERR("Cannot find message");
    }
    else
    {
        if (NULL != send_to_instance->fnCallback)
        {
            send_to_instance->fnCallback();
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
* @param    owner_id:       id of the owner of the instance
* @param    *message_size:  size of the message
*
* @return   address of the message if there is one
*
*******************************************************************************/
void* ezmIpc_ReceiveMessage(uint8_t owner_id, uint16_t *message_size)
{
    bool        is_success = true;
    void        *buffer_address = NULL;
    IpcInstance *instance = NULL;

    instance = ezmIpc_SearchInstance(owner_id);

    if (NULL == instance)
    {
        is_success = false;
    }

    if (is_success)
    {
        buffer_address = instance->memory_list.alloc_list.pstHead->pBuffer;
        *message_size = instance->memory_list.alloc_list.pstHead->u16BufferSize;
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
* @param    owner_id: owner id of the ipc instance
* @param    *message:   message to be free
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmIpc_ReleaseMessage(uint8_t owner_id, void* message)
{
    bool        is_success = true;
    void        *buffer_address = NULL;
    IpcInstance *instance = NULL;

    instance = ezmIpc_SearchInstance(owner_id);

    if (NULL == instance || NULL == message )
    {
        is_success = false;
    }

    if (is_success)
    {
        is_success = is_success & ezmStcMem_Free(&instance->memory_list, message);
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
        instance_pool[instance_index].instance_owner_id = FREE_INSTANCE;
        instance_pool[instance_index].fnCallback = NULL;
        memset(&instance_pool[instance_index].memory_list, 0, sizeof(instance_pool[instance_index].memory_list));
    }
}

/******************************************************************************
* Function : ezmIpc_GetFreeInstance
*//**
* \b Description:
*
* Return a free instance from the pool
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   pointer to free instance or NULL
*
*******************************************************************************/
static IpcInstance *ezmIpc_GetFreeInstance(void)
{
    IpcInstance *free_instance = NULL;
    for (uint8_t i = 0; i < NUM_OF_IPC_INSTANCE; i++)
    {
        if (FREE_INSTANCE == instance_pool[i].instance_owner_id)
        {
            free_instance = &instance_pool[i];
            break;
        }
    }
    return free_instance;
}

/******************************************************************************
* Function : ezmIpc_SearchInstance
*//**
* \b Description:
*
* Search a intance in the pool according to the owner id
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    owner_id: owner id of the instance
* @return   pointer to the instance or null
*
*******************************************************************************/
static IpcInstance* ezmIpc_SearchInstance(uint8_t owner_id)
{
    IpcInstance *searched_instance = NULL;
    for (uint8_t i = 0; i < NUM_OF_IPC_INSTANCE; i++)
    {
        if (instance_pool[i].instance_owner_id == owner_id)
        {
            searched_instance = &instance_pool[i];
            break;
        }
    }
    return searched_instance;
}

static bool ezmIpc_IsInstanceExist(uint8_t owner_id)
{
    bool is_existing = false;
    for (uint8_t i = 0; i < NUM_OF_IPC_INSTANCE; i++)
    {
        if (instance_pool[i].instance_owner_id == owner_id)
        {
            is_existing = true;
            break;
        }
    }

    return is_existing;
}
#endif /* IPC */

/* End of file*/