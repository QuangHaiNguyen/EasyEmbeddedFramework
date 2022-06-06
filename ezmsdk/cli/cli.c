/*******************************************************************************
* Filename:         cli.c
* Author:           Quang Hai Nguyen
* Original Date:    22.05.2022
* Last Update:      22.05.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Quang Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Quang Hai Nguyen 22.05.2022
*
*******************************************************************************/

/** @file  cli.c
 *  @brief This is the source code for a cli
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "cli.h"

#if (CLI == 1U)

#include "ezmDebug/ezmDebug.h"
#include "string.h"

#define VERBOSE                 1U

#define MOD_NAME                "CLI"

#if (MODULE_DEBUG == 1U) && (CLI_DEBUG == 1U)
    #define CLIPRINT(a)                     PRINT_DEBUG(MOD_NAME,a)
    #define CLIPRINT1(a,b)                  PRINT_DEBUG1(MOD_NAME,a,b)
    #define CLIPRINT2(a,b,c)                PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define CLIPRINT3(a,b,c,d)              PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define CLIPRINT4(a,b,c,d,e)            PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#else 
    #define CLIPRINT(a)
    #define CLIPRINT1(a,b)
    #define CLIPRINT2(a,b,c)
    #define CLIPRINT3(a,b,c,d)
    #define CLIPRINT4(a,b,c,d,e)
#endif

#define STR_TERMINATE   '\0'
#define SPACE           ' '
#define ARG_INVALID     NUM_OF_ARG
#define SKIP_NULL_SPACE(pointer, end)   while((*pointer == ' ' || *pointer == '\0') && pointer != end){pointer++;}
#define BEGIN   "$ "

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Metadata of a command
 *  
 */
typedef struct 
{
    const char      *command;                        /**< Pointer to the command */
    const char      *description;                    /**< Pointer to the command description */
    CLI_CALLBACK    callback;                       /**< Pointer to the command callback function */
    uint32_t        *long_arg_list[NUM_OF_ARG];      /**< Pointer to the list of argument in long form */
    uint32_t        *short_arg_list[NUM_OF_ARG];     /**< Pointer to the list of argument in short form */
    uint32_t        *description_list[NUM_OF_ARG];   /**< Pointer to the command description */
    char            *value_list[NUM_OF_ARG];         /**< Pointer to the list of values */
}CommandMetadata;


/** @brief state of the CLI command parser
 *
 */
typedef enum 
{
    STATE_COMMAND,  /**< State parsing the command */
    STATE_ARGUMENT, /**< State parsing the argument */
    STATE_VALUE,    /**< State parsing the value */
    STATE_EXECUTE,  /**< State execute the callback function*/
    STATE_ERROR,    /**< Handle error and clean up */
}ENUM_CLI_STATE;


/**@brief
 *
 */
typedef enum
{
    GET_BYTE,   /**< */
    WAIT,       /**< */
    PROC_CMD,   /**< */
}CLI_RUN_STATE;

/**@brief
 *
 */
struct CliInstance
{ 
    uint8_t         cli_buffer[256];/**< */
    CommandHandle   cli_inst_num;   /**< */
    uint8_t         one_byte;       /**< */
    uint16_t        buff_index;     /**< */
    CLI_RUN_STATE   state;          /**< */
    UartDrvApi*     uart_driver;    /**< */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static CommandMetadata  astMetaData[NUM_OF_CMD] = { 0 }; /**< holding commands metadata */
static ENUM_CLI_STATE   eState = STATE_COMMAND; /**< Holding the current state of the parser */
static const char       cmd_help[] = "help";
static const char       cmd_help_desc[] = "show help";
static const uint8_t    welcome[] = "CLI has been activated, type help for the list of command\n";
/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint8_t  ezmCli_GetFreeInstance          (void);
static bool     ezmCli_ResetMetaData            (uint8_t index);
static bool     ezmCli_IsCommandExist           (const char * command,
                                                    uint8_t * index);
static uint8_t  ezmCli_IsLongFormArgumentExist  (uint8_t command_index,
                                                    const char * long_arg);
static uint8_t  ezmCli_IsShortFormArgumentExist (uint8_t command_index,
                                                    const char * short_arg);
static void     ezmCli_PrintCommandHelp         (uint8_t index);
static bool     ezmCli_IsArgumentShortForm      (char * short_arg);
static bool     ezmCli_IsArgumentLongForm       (char * long_arg);
static void     ezmCli_ResetValueList           (uint8_t index);

static uint8_t  UartCallbackHandle              (uint8_t notify_code,
                                                    void *param1);

static CLI_NOTIFY_CODE  HandleHelpCommand(const char* command, void* value_list);
static struct CliInstance  cli_inst;

/************************** Public function **********************************/

/******************************************************************************
* Function : ezmCli_Init
*//** 
* \b Description:
*
* This function initializes the command line interface. It resets the resource and
* start the low level serial interface driver
*
* PRE-CONDITION: has dependency on UART HAL and deriver interface modules.
* Therefore, they must be initiated before calling the cli module 
*
* POST-CONDITION: None
* 
* @param    uart_driver: driver instance of the uart peripheral
*
* @return   true: success
*           false fail
*
* \b Example Example:
* @code
* ezmCli_Init();
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmCli_Init(UartDrvApi* uart_driver)
{
    bool success = true;

    for (uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if (!ezmCli_ResetMetaData(i))
        {
            CLIPRINT1("Cannot reset metadata: [index = %d]", i);
        }
    }

    if (uart_driver == NULL)
    {
        success = false;
    }

    if (success)
    {
        cli_inst.uart_driver = uart_driver;
        cli_inst.uart_driver->ezmUart_RegisterCallback(UartCallbackHandle);
        cli_inst.buff_index = 0;
        cli_inst.state = GET_BYTE;
        memset(cli_inst.cli_buffer, 0, sizeof(cli_inst.cli_buffer));

        cli_inst.cli_inst_num = ezmCli_RegisterCommand(cmd_help, 
                                                        cmd_help_desc, 
                                                        HandleHelpCommand);

        if (cli_inst.cli_inst_num == 0xFF)
        {
            CLIPRINT("add command error");
            success = false;
        }
        else
        {
            CLIPRINT("CLI has been activated, type help for the list of command");
            cli_inst.uart_driver->ezmUart_Send(welcome, sizeof(welcome));
        }
    }

    return success;
}


/******************************************************************************
* Function : ezmCli_Run
*//**
* \b Description:
*
* This function must be call in a loop/or scheduler to run the CLI
*
* PRE-CONDITION: CLI module must be initialized
*
* POST-CONDITION: None
*
* @param    None
*
* @return   None
*
* \b Example Example:
* @code
* while(1)
* {
*     ezmCli_Run();
* }
* @endcode
*
* @see sum
*
*******************************************************************************/
void ezmCli_Run(void)
{
    switch (cli_inst.state)
    {
    case GET_BYTE:
        //(void)cli_inst.uart_driver->ezmUart_Receive(&cli_inst.one_byte, 1U);
#if(SUPPORTED_CHIP != WIN)
        cli_inst.state = WAIT;
#endif
        break;
    case WAIT:
        break;
    case PROC_CMD:
        (void)ezmCli_CommandReceivedCallback(0, (char*)cli_inst.cli_buffer, sizeof(cli_inst.cli_buffer));
        memset(cli_inst.cli_buffer, 0, sizeof(cli_inst.cli_buffer));
        cli_inst.buff_index = 0U;
        cli_inst.uart_driver->ezmUart_Send((uint8_t*)BEGIN, sizeof(BEGIN));
        cli_inst.state = GET_BYTE;
        break;
    default:
        cli_inst.state = GET_BYTE;
        break;
    }
}

/******************************************************************************
* Function : ezmCli_RegisterCommand
*//** 
* \b Description:
*
* This function register a new command 
*
* PRE-CONDITION: ezmCli_Init() must be called before
*
* POST-CONDITION: None
* 
* @param    *command:     (IN)pointer to the command buffer
* @param    *description: (IN)pointer to the description buffer
* @param    callback:    (IN)pointer to the callback function, where the command is executed
*
* @return   index of the command, or 0xff of fail
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* @endcode
*
* @see sum
*
*******************************************************************************/
CommandHandle ezmCli_RegisterCommand(const char *command,
                                        const char *description,
                                        CLI_CALLBACK callback)
{
    CommandHandle handle = CLI_HANDLE_INVALID;
    
    if (command != NULL
        && callback != NULL
        && description != NULL
        && ezmCli_IsCommandExist(command, NULL) == false)
    {
        handle = ezmCli_GetFreeInstance();
        if (handle < CLI_HANDLE_INVALID)
        {
            astMetaData[handle].callback = callback;
            astMetaData[handle].command = command;
            astMetaData[handle].description = description;

            CLIPRINT2("Add new command: [command = %s] [description = %s]",
                        command,
                        description);
        }
        else
        {
            CLIPRINT("ezmCli_RegisterCommand() FAILED");
        }
    }
    return handle;
}


/******************************************************************************
* Function : ezmCli_AddArgument
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: ezmCli_Init() and ezmCli_RegisterCommand() must be called before
*
* POST-CONDITION: None
* 
* @param    cmd_handle:  (IN)index of the command, which the argument will be added
* @param    *long_arg:   (IN)argument in long form, starting with --
* @param    *short_arg:  (IN)argument in short form, starting with -
* @param    *description:(IN)argument description
*
* @return   True if success, else false
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* ezmCli_AddArgument(u8Index, "--arg1", "-a1", "argument 1");
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmCli_AddArgument (CommandHandle cmd_handle,
                            const char * long_arg,
                            const char * short_arg,
                            const char * description)
{
    bool bResult = false;

    if (long_arg != NULL
        && short_arg != NULL
        && description != NULL
        && (uint32_t)cmd_handle < CLI_HANDLE_INVALID)
    {
        if (ezmCli_IsLongFormArgumentExist(cmd_handle, long_arg) == ARG_INVALID
            && ezmCli_IsShortFormArgumentExist(cmd_handle, short_arg) == ARG_INVALID)
        {
            for (uint8_t i = 0; i < NUM_OF_ARG; i++)
            {
                if (astMetaData[cmd_handle].long_arg_list[i] == NULL)
                {
                    astMetaData[cmd_handle].long_arg_list[i] = (uint32_t*)long_arg;
                    astMetaData[cmd_handle].short_arg_list[i] = (uint32_t*)short_arg;
                    astMetaData[cmd_handle].description_list[i] = (uint32_t*)description;

                    CLIPRINT3("Add new argument: [long = %s] [shord = %s] [description = %s]",
                        (char*)astMetaData[cmd_handle].long_arg_list[i],
                        (char*)astMetaData[cmd_handle].short_arg_list[i],
                        (char*)astMetaData[cmd_handle].description_list[i]);
                    bResult = true;
                    break;
                }
            }
        }
    }
    return bResult;
}

/******************************************************************************
* Function : ezmCli_CommandReceivedCallback
*//** 
* \b Description:
*
* This function resets the contents of the metadata at a specific index
*
* PRE-CONDITION: ezmCli_Init() and ezmCli_RegisterCommand() 
*                ezmCli_AddArgument() must be called before
*
* POST-CONDITION: None
* 
* @param    notify_code:         (IN) notification code
* @param    *command_buffer:     (IN) pointer to the buffer storing the command
* @param    command_buff_size:   (IN) size of the buffer storing the command
*
* @return   True if success, else false
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* ezmCli_AddArgument(u8Index, "--arg1", "-a1", "argument 1");
* ezmCli_CommandReceivedCallback(u8NotifyCode, pu8CommandBuffer,u16Size);
* @endcode
*
*******************************************************************************/
bool ezmCli_CommandReceivedCallback(uint8_t notify_code,
                                    char * command_buffer,
                                    uint16_t command_buff_size)
{
    bool bResult = false;
    bool process_is_done = false;
    uint8_t u8CommandIndex = CLI_HANDLE_INVALID;
    uint8_t u8ValueIndex = CLI_HANDLE_INVALID;
    char* tail;
    eState = STATE_COMMAND;

    char* head = command_buffer;
    SKIP_NULL_SPACE(head, command_buffer + command_buff_size);
    tail = head;

    if (*head == '\n' || *head == '\r')
    {
        process_is_done = true;
    }

    while (head != command_buffer + command_buff_size &&
        process_is_done == false)
    {
        switch (eState)
        {
        case STATE_COMMAND:
            if (*head == SPACE)
            {
                *head = STR_TERMINATE;
                CLIPRINT1("Receive command: [command = %s]", tail);

                if (ezmCli_IsCommandExist((char*)tail, &u8CommandIndex))
                {
                    eState = STATE_ARGUMENT;

                    SKIP_NULL_SPACE(head, command_buffer + command_buff_size);
                    tail = head;
                }
                else
                {
                    eState = STATE_ERROR;
                }
            }
            
            if (*head == '\r' || *head == '\n')
            {
                *head = STR_TERMINATE;
                if(ezmCli_IsCommandExist((char*)tail, &u8CommandIndex) 
                    && NULL == astMetaData[u8CommandIndex].long_arg_list[0])
                {
                    eState = STATE_EXECUTE;
                }
                else
                {
                    eState = STATE_ERROR;
                }
            }

            break;
        case STATE_ARGUMENT:
            if (*head == SPACE)
            {
                *head = STR_TERMINATE;

                CLIPRINT1("Receive argument: [argument = %s]", tail);

                u8ValueIndex = ezmCli_IsShortFormArgumentExist(u8CommandIndex, tail);
                if (u8ValueIndex == ARG_INVALID)
                {
                    u8ValueIndex = ezmCli_IsLongFormArgumentExist(u8CommandIndex, tail);
                }

                if (ARG_INVALID != u8ValueIndex)
                {
                    eState = STATE_VALUE;
                    CLIPRINT1("Value index: [idx = %d]", u8ValueIndex);

                    SKIP_NULL_SPACE(head, command_buffer + command_buff_size);
                    tail = head;
                }
                else
                {
                    eState = STATE_ERROR;
                }
            }
            
            if (*head == '\r' || *head == '\n')
            {
                eState = STATE_ERROR;
            }
            break;

        case STATE_VALUE:
            if (*head == SPACE && *(head + 1) == '-')
            {
                *head = STR_TERMINATE;
                CLIPRINT1("Receive value: [value = %s]", tail);

                astMetaData[u8CommandIndex].value_list[u8ValueIndex] = tail;
                eState = STATE_ARGUMENT;

                SKIP_NULL_SPACE(head, command_buffer + command_buff_size);
                tail = head;

            }
            
            if (*head == '\r' || *head == '\n')
            {
                *head = STR_TERMINATE;
                CLIPRINT1("Receive value: [value = %s]", tail);

                astMetaData[u8CommandIndex].value_list[u8ValueIndex] = tail;
                eState = STATE_EXECUTE;
            }

            break;

        case STATE_EXECUTE:
            /* Execute the callback */
            if (CLI_NC_OK == astMetaData[u8CommandIndex].callback(astMetaData[u8CommandIndex].command,
                                                                    astMetaData[u8CommandIndex].value_list))
            {
                bResult = true;
            }

            ezmCli_ResetValueList(u8CommandIndex);
            process_is_done = true;
            break;

        case STATE_ERROR:
            CLIPRINT("Error");
            ezmCli_PrintCommandHelp(u8CommandIndex);
            process_is_done = true;
            break;
        }
        head++;
    }

    (void)(notify_code);

    return bResult;
}

/*********************** Private function ************************************/

/******************************************************************************
* Function : ezmCli_ResetMetaData
*//** 
* \b Description:
*
* This function resets the contents of the metadata at a specific index
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    index:    (IN) Index of the command metadata
* @return   True:       Metadata is reset
*           False:      Cannot find the index of metadata
*
* \b Example Example:
* @code
* ezmCli_ResetMetaData(0U);
* @endcode
*
*******************************************************************************/
static bool ezmCli_ResetMetaData (uint8_t index)
{
    bool bResult = false;

    if(index < NUM_OF_CMD)
    {
        astMetaData[index].callback = NULL;
        astMetaData[index].command = NULL;
        astMetaData[index].description = NULL;
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            astMetaData[index].description_list[i] = NULL;
            astMetaData[index].long_arg_list[i] = NULL;
            astMetaData[index].short_arg_list[i] = NULL;
            astMetaData[index].value_list[i] =NULL;
        }
        bResult = true; 
    }

    return bResult;
}

/******************************************************************************
* Function : ezmCli_GetFreeInstance
*//** 
* \b Description:
*
* This function gets a free instance of the command data
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
* @return   CLI_HANDLE_INVALID if there is no free instance, otherwise the index
*           of the intsance
*
* \b Example Example:
* @code
* uint8_t u8FreeInstance = ezmCli_GetFreeInstance()
* @endcode
*
*******************************************************************************/
static uint8_t ezmCli_GetFreeInstance (void)
{
    uint8_t u8FreeInstanceIndex = CLI_HANDLE_INVALID;

    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if(astMetaData[i].command == NULL)
        {
            u8FreeInstanceIndex = i;
            CLIPRINT1("Found free instance: [index = %d]", u8FreeInstanceIndex);
            break;
        }
    }

    return u8FreeInstanceIndex;
}

/******************************************************************************
* Function : ezmCli_IsCommandExist
*//** 
* \b Description:
*
* This function checks if the the command exists
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    *command:(IN) pointer to the command string
* @param    *index:(OUT) index of the checked command
* @return   True:       Command is existing
*           False:      Command is not existing
*
* \b Example Example:
* @code
* uint8_t index = CLI_HANDLE_INVALID;
* if(ezmCli_IsCommandExist("TurnOnLED", &index))
* {
*     printf("command exists, index %d", index);
* }
* @endcode
*
*******************************************************************************/
static bool ezmCli_IsCommandExist   (const char *command, uint8_t *index)
{
    bool cmd_exist = false;

    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if(astMetaData[i].command != NULL && 
            strcmp(astMetaData[i].command, command) == 0)
        {
            cmd_exist = true;

            if(index != NULL)
            {
                *index = i;
            }

            CLIPRINT1("Command is existing: [command = %s]", command);
            break;
        }
    }
    return cmd_exist;
}


/******************************************************************************
* Function : ezmCli_PrintCommandHelp
*//**
* \b Description:
*
* This function prints the help of a specific command
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    index:(IN) index of the command
* @return   None
*
*******************************************************************************/
static void ezmCli_PrintCommandHelp (uint8_t index)
{
    if(index < NUM_OF_CMD && astMetaData[index].command != NULL)
    {
        PRINTF_NO_NL("%s\n", "-----------------------------------------");
        PRINTF_NO_NL("%s", "usage: ");
        PRINTF_NO_NL("%s ", astMetaData[index].command);

        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[index].short_arg_list[i] != NULL)
            {
                PRINTF_NO_NL("[%s] ", (char *)astMetaData[index].short_arg_list[i]);
                PRINTF_NO_NL("%s\n", "<VALUE>");
            }
        }

        PRINT1("");
        PRINTF_NO_NL("%s", "description: ");
        PRINTF_NO_NL("\n\t%s", astMetaData[index].description);
        PRINT1("");
        PRINT1("Argument options:");
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[index].short_arg_list[i] != NULL)
            {
                PRINTF_NO_NL("\t%s, ", (char *)astMetaData[index].short_arg_list[i]);
                PRINTF_NO_NL("%s ", (char *)astMetaData[index].long_arg_list[i]);
                PRINTF_NO_NL("\t%s \n", (char *)astMetaData[index].description_list[i]);
            }
        }
        PRINT1("");
    }
}

/******************************************************************************
* Function : ezmCli_PrintMenu
*//**
* \b Description:
*
* This function checks prints the complete help
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezmCli_PrintMenu (void)
{
    PRINT1("\nList of commands:");
    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        ezmCli_PrintCommandHelp(i);
    }
    PRINT1("\n");
}

/******************************************************************************
* Function : ezmCli_IsArgumentShortForm
*//**
* \b Description:
*
* This function checks if the arguments are in short form
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *short_arg:    (IN) pointer to the argument
*
* @return   True:       Argument is in short form
*           False:      Argument is not in short form 
*
*******************************************************************************/
static bool     ezmCli_IsArgumentShortForm(char* short_arg)
{
    bool is_exist = false;
    if(*short_arg == '-')
    {
        if(( *(short_arg + 1) >= 'a' && *(short_arg + 1) <= 'z')
            || ( *(short_arg + 1) >= 'A' && *(short_arg + 1) <= 'Z'))
        {
            is_exist = true;
        }
    } 
    return  is_exist;
}

/******************************************************************************
* Function : ezmCli_IsArgumentLongForm
*//**
* \b Description:
*
* This function checks if the arguments are in long form
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *long_arg:    (IN) pointer to the argument
*
* @return   True:       Argument is in long form
*           False:      Argument is not in long form
*
*******************************************************************************/
static bool ezmCli_IsArgumentLongForm(char * long_arg)
{
    return (*long_arg == '-' && ezmCli_IsArgumentShortForm(long_arg + 1));
}

/******************************************************************************
* Function : ezmCli_IsLongFormArgumentExist
*//**
* \b Description:
*
* This function checks if the long form argument is existing and return its index
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    u8CommandIndex: (IN) Index of the command metadata
* @param    *pu8LongForm:   (IN) Pointer to the buffer storing the argument
*
* @return   Index of the argument
*
*******************************************************************************/
static uint8_t  ezmCli_IsLongFormArgumentExist(uint8_t command_index, 
                                                const char* long_arg)
{
    uint8_t index = ARG_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[command_index].long_arg_list[i] != NULL &&
                strcmp((char *)astMetaData[command_index].long_arg_list[i], long_arg) == 0)
        {
            index = i;
            CLIPRINT1("argument exists: [argument = %s]", long_arg);
            break;
        }
    }
    return index;
}

/******************************************************************************
* Function : ezmCli_IsShortFormArgumentExist
*//**
* \b Description:
*
* This function checks if the short form argument is existing and return its index
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    command_index:  (IN) Index of the command metadata
* @param    *short_arg:     (IN) Pointer to the buffer storing the argument
*
* @return   Index of the argument
*
*******************************************************************************/
static uint8_t  ezmCli_IsShortFormArgumentExist(uint8_t command_index,
                                                    const char* short_arg)
{
    uint8_t index = ARG_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[command_index].short_arg_list[i] != NULL &&
            strcmp((char *)astMetaData[command_index].short_arg_list[i], short_arg) == 0)
        {
            index = i;
            CLIPRINT1("argument exists: [argument = %s]", short_arg);
            break;
        }
    }
    return index;
}

/******************************************************************************
* Function : ezmCli_ResetValueList
*//**
* \b Description:
*
* This function resets the list storing the pointer the the values.
* It is called after the callback function is executed
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    index:    (IN) Index of the command metadata
* @return   None
*
*******************************************************************************/
static void ezmCli_ResetValueList(uint8_t index)
{
    if(index < NUM_OF_CMD)
    {
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            astMetaData[index].value_list[i] = NULL;
        }
        CLIPRINT1("[index = %d]", index);
    }
}

/******************************************************************************
* Function : UartCallbackHandle
*//**
* \b Description:
*
* Callback from the UART peripheral to handle the data. Here we check:
*   * buffer does not have enough size --> termnate
*   * if we get the \n or \r --> terminate and process the command
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    notify_code: notification code
* @param    *param1
* @param    *param2
* @return   None
*
*******************************************************************************/
static uint8_t UartCallbackHandle(uint8_t notify_code, void* param1)
{
    
    switch ((UART_NOTIFY_CODE)notify_code)
    {
    case UART_TX_COMPLT:
        break;
    case UART_RX_COMPLT:
#if(SUPPORTED_CHIP == WIN)
        cli_inst.cli_buffer[cli_inst.buff_index] = *(char*)param1;
        CLIPRINT1("[test = %c]", cli_inst.cli_buffer[cli_inst.buff_index]);
        if (cli_inst.cli_buffer[cli_inst.buff_index] == '\n' || 
            cli_inst.buff_index == sizeof(cli_inst.cli_buffer) || 
            cli_inst.cli_buffer[cli_inst.buff_index] == '\r')
        {
            cli_inst.state = PROC_CMD;
        }
#else
        (void)cli_inst.uart_driver->ezmUart_Receive(&cli_inst.one_byte, 1U);
        
        cli_inst.cli_buffer[cli_inst.buff_index] = cli_inst.one_byte;
        
        CLIPRINT2("[test = %c ], [size = %d]", 
                    cli_inst.cli_buffer[cli_inst.buff_index],
                    *(uint16_t*)param1);

        if (cli_inst.cli_buffer[cli_inst.buff_index] == '\n' || 
            cli_inst.buff_index == sizeof(cli_inst.cli_buffer) || 
            cli_inst.cli_buffer[cli_inst.buff_index] == '\r')
        {
            CLIPRINT("command receeive");
            cli_inst.state = PROC_CMD;
        }
        else
        {
            //(void)cli_inst.uart_driver->ezmUart_Receive(&cli_inst.one_byte, 1U);
        }
#endif
        cli_inst.buff_index++;
        break;
    case UART_BUFF_FULL:
        break;
    case UART_UNSUPPORTED:
        break;
    default:
        break;
    }

    return 0U;
}

/******************************************************************************
* Function : HandleHelpCommand
*//**
* \b Description:
*
* Callback to handle the "help" command, which is a default command to print
* the list of commands
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    *command: pointer to the help command, to cross check if it is correct
* @param    *value_list: values of the args
* @return   return code
*
*******************************************************************************/
static CLI_NOTIFY_CODE  HandleHelpCommand(const char *command, void *value_list)
{
    if (strcmp(command, cmd_help) == 0U)
    {
        ezmCli_PrintMenu();
    }
    else
    {
        CLIPRINT("Unknown command");
    }

    (void)value_list;
    return CLI_NC_OK;
}
#endif /* CLI */

/* End of file*/