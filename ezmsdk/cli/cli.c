/*******************************************************************************
* Title                 :   cli 
* Filename              :   cli.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.12.2021
* Version               :   1.0.0
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

#include "../ezmDebug/ezmDebug.h"
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
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Metadata of a command
 *  
 */
typedef struct 
{
    const char *    pu8Command;                         /**< Pointer to the command */
    const char *    pu8Description;                     /**< Pointer to the command description */
    CLI_CALLBACK    pfnCallback;                        /**< Pointer to the command callback function */
    uint32_t *      au32LongArgumenList[NUM_OF_ARG];    /**< Pointer to the list of argument in long form */
    uint32_t *      au32ShordArgumenList[NUM_OF_ARG];   /**< Pointer to the list of argument in short form */
    uint32_t *      au32DescriptionList[NUM_OF_ARG];    /**< Pointer to the command description */
    char *          au8ValueList[NUM_OF_ARG];           /**< Pointer to the list of values */
}CommandMetadata;

/** @brief state of the command parser
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

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static CommandMetadata astMetaData[NUM_OF_CMD];     /**< Array holding commands metadata */
static ENUM_CLI_STATE  eState = STATE_COMMAND;      /**< Holding the current state of the parser*/

/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint8_t  ezmCli_GetFreeInstance          (void);
static bool     ezmCli_ResetMetaData            (uint8_t u8Index);
static bool     ezmCli_IsCommandExist           (const char * pu8Command, uint8_t * pu8Index);
static uint8_t  ezmCli_IsLongFormArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm);
static uint8_t  ezmCli_IsShortFormArgumentExist (uint8_t u8CommandIndex,const char * pu8ShortForm);
static void     ezmCli_PrintCommandHelp         (uint8_t u8Index);
static bool     ezmCli_IsArgumentShortForm      (char * pu8ShortFormArg);
static bool     ezmCli_IsArgumentLongForm       (char * pu8LongFormArg);
static void     ezmCli_ResetValueList           (uint8_t u8Index);

/************************** Public function **********************************/

/******************************************************************************
* Function : ezmCli_Init
*//** 
* \b Description:
*
* This function initializes the command line interface. It resets the resource and
* start the low level serial interface driver
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
*
* @return   None
*
* \b Example Example:
* @code
* ezmCli_Init();
* @endcode
*
* @see sum
*
*******************************************************************************/
void ezmCli_Init(void)
{
    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if(!ezmCli_ResetMetaData(i))
        {
            PRINT2("Cannot reset metadata: [index = %d]", i);
        }
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
* @param    pu8Command:     (IN)pointer to the command buffer
* @param    pu8Description: (IN)pointer to the description buffer
* @param    pfnCallback:    (IN)pointer to the callback function, where the command is executed
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
CommandHandle ezmCli_RegisterCommand(const char * pu8Command, const char *  pu8Description, CLI_CALLBACK pfnCallback)
{
    CommandHandle handle = CLI_HANDLE_INVALID;
    
    if (pu8Command != NULL 
        && pfnCallback != NULL 
        && pu8Description != NULL 
        && ezmCli_IsCommandExist(pu8Command, NULL) == false)
    {
        handle = ezmCli_GetFreeInstance();
        if (handle < CLI_HANDLE_INVALID)
        {
            astMetaData[handle].pfnCallback = pfnCallback;
            astMetaData[handle].pu8Command = pu8Command;
            astMetaData[handle].pu8Description = pu8Description;
            CLIPRINT2("Add new command: [command = %s] [description = %s]", pu8Command, pu8Description);
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
* @param    u8CommandIndex: (IN)index of the command, which the argument will be added
* @param    *pu8LongForm:   (IN)argument in long form, starting with --
* @param    *pu8ShortForm:  (IN)argument in short form, starting with -
* @param    *pu8Description:(IN)argument description
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
bool ezmCli_AddArgument (CommandHandle u8CommandIndex,
                            const char * pu8LongForm, 
                            const char * pu8ShortForm, 
                            const char * pu8Description)
{
    bool bResult = false;

    if (pu8LongForm != NULL
        && pu8ShortForm != NULL
        && pu8Description != NULL
        && (uint32_t)u8CommandIndex < CLI_HANDLE_INVALID)
    {
        if (ezmCli_IsLongFormArgumentExist(u8CommandIndex, pu8LongForm) == ARG_INVALID
            && ezmCli_IsShortFormArgumentExist(u8CommandIndex, pu8ShortForm) == ARG_INVALID)
        {
            for (uint8_t i = 0; i < NUM_OF_ARG; i++)
            {
                if (astMetaData[u8CommandIndex].au32LongArgumenList[i] == NULL)
                {
                    astMetaData[u8CommandIndex].au32LongArgumenList[i] = (uint32_t*)pu8LongForm;
                    astMetaData[u8CommandIndex].au32ShordArgumenList[i] = (uint32_t*)pu8ShortForm;
                    astMetaData[u8CommandIndex].au32DescriptionList[i] = (uint32_t*)pu8Description;

                    CLIPRINT3("Add new argument: [long = %s] [shord = %s] [description = %s]",
                        (char*)astMetaData[u8CommandIndex].au32LongArgumenList[i],
                        (char*)astMetaData[u8CommandIndex].au32ShordArgumenList[i],
                        (char*)astMetaData[u8CommandIndex].au32DescriptionList[i]);
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
* @param    u8NotifyCode:           (IN) notification code
* @param    *pu8CommandBuffer:      (IN) pointer to the buffer storing the command
* @param    u16CommandBufferSize:   (IN) size of the buffer storing the command
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
bool ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode, char * pu8CommandBuffer, uint16_t u16CommandBufferSize)
{
    bool bResult = false;
    bool process_is_done = false;
    uint8_t u8CommandIndex = CLI_HANDLE_INVALID;
    uint8_t u8ValueIndex = CLI_HANDLE_INVALID;
    char* tail;
    eState = STATE_COMMAND;

    char* head = pu8CommandBuffer;
    SKIP_NULL_SPACE(head, pu8CommandBuffer + u16CommandBufferSize);
    tail = head;

    if (*head == '\n' || *head == '\r')
    {
        process_is_done = true;
    }

    while (head != pu8CommandBuffer + u16CommandBufferSize &&
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

                    SKIP_NULL_SPACE(head, pu8CommandBuffer + u16CommandBufferSize);
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
                    && NULL == astMetaData[u8CommandIndex].au32LongArgumenList[0])
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

                    SKIP_NULL_SPACE(head, pu8CommandBuffer + u16CommandBufferSize);
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

                astMetaData[u8CommandIndex].au8ValueList[u8ValueIndex] = tail;
                eState = STATE_ARGUMENT;

                SKIP_NULL_SPACE(head, pu8CommandBuffer + u16CommandBufferSize);
                tail = head;

            }
            
            if (*head == '\r' || *head == '\n')
            {
                *head = STR_TERMINATE;
                CLIPRINT1("Receive value: [value = %s]", tail);

                astMetaData[u8CommandIndex].au8ValueList[u8ValueIndex] = tail;
                eState = STATE_EXECUTE;
            }

            break;

        case STATE_EXECUTE:
            /* Execute the callback */
            if (CLI_NC_OK == astMetaData[u8CommandIndex].pfnCallback(astMetaData[u8CommandIndex].pu8Command, astMetaData[u8CommandIndex].au8ValueList))
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

    (void)(u8NotifyCode);

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
* @param    u8Index:    (IN) Index of the command metadata
* @return   True:       Metadata is reset
*           False:      Cannot find the index of metadata
*
* \b Example Example:
* @code
* ezmCli_ResetMetaData(0U);
* @endcode
*
*******************************************************************************/
static bool ezmCli_ResetMetaData (uint8_t u8Index)
{
    bool bResult = false;

    if(u8Index < NUM_OF_CMD)
    {
        astMetaData[u8Index].pfnCallback = NULL;
        astMetaData[u8Index].pu8Command = NULL;
        astMetaData[u8Index].pu8Description = NULL;
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            astMetaData[u8Index].au32DescriptionList[i] = NULL;
            astMetaData[u8Index].au32LongArgumenList[i] = NULL;
            astMetaData[u8Index].au32ShordArgumenList[i] = NULL;
            astMetaData[u8Index].au8ValueList[i] =NULL;
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
        if(astMetaData[i].pu8Command == NULL)
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
* @param    *pu8Command:(IN) pointer to the command string
* @return   True:       Command is existing
*           False:      Command is not existing
*
* \b Example Example:
* @code
* if(ezmCli_IsCommandExist("TurnOnLED"))
* {
*       printf("command exists")
* }
* @endcode
*
*******************************************************************************/
static bool ezmCli_IsCommandExist   (const char * pu8Command, uint8_t * pu8Index)
{
    bool bIsExist = false;

    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if(astMetaData[i].pu8Command != NULL && strcmp(astMetaData[i].pu8Command, pu8Command) == 0)
        {
            bIsExist = true;

            if(pu8Index != NULL)
            {
                *pu8Index = i;
            }
            
            CLIPRINT1("Command is existing: [command = %s]", pu8Command);
            break;
        }
    }
    return bIsExist;
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
* @param    u8Index:(IN) index of the command
* @return   None
*
*******************************************************************************/
static void ezmCli_PrintCommandHelp (uint8_t u8Index)
{
    if(u8Index < NUM_OF_CMD && astMetaData[u8Index].pu8Command != NULL)
    {
        PRINTF_NO_NL("%s\n", "-----------------------------------------");
        PRINTF_NO_NL("%s", "usage: ");
        PRINTF_NO_NL("%s ", astMetaData[u8Index].pu8Command);

        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[u8Index].au32ShordArgumenList[i] != NULL)
            {
                PRINTF_NO_NL("[%s] ", (char *)astMetaData[u8Index].au32ShordArgumenList[i]);
                PRINTF_NO_NL("%s\n", "<VALUE>");
            }
        }

        PRINT1("");
        PRINTF_NO_NL("%s", "description: ");
        PRINTF_NO_NL("\n\t%s", astMetaData[u8Index].pu8Description);
        PRINT1("");
        PRINT1("Argument options:");
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[u8Index].au32ShordArgumenList[i] != NULL)
            {
                PRINTF_NO_NL("\t%s, ", (char *)astMetaData[u8Index].au32ShordArgumenList[i]);
                PRINTF_NO_NL("%s ", (char *)astMetaData[u8Index].au32LongArgumenList[i]);
                PRINTF_NO_NL("\t%s \n", (char *)astMetaData[u8Index].au32DescriptionList[i]);
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
* @param    *pu8ShortFormArg:    (IN) pointer to the argument
*
* @return   True:       Argument is in short form
*           False:      Argument is not in short form 
*
*******************************************************************************/
static bool ezmCli_IsArgumentShortForm(char * pu8ShortFormArg)
{
    bool bResult = false;
    if(*pu8ShortFormArg == '-')
    {
        if(( *(pu8ShortFormArg + 1) >= 'a' && *(pu8ShortFormArg + 1) <= 'z') 
            || ( *(pu8ShortFormArg + 1) >= 'A' && *(pu8ShortFormArg + 1) <= 'Z'))
        {
            bResult = true;
        }
    } 
    return  bResult;
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
* @param    *pu8LongFormArg:    (IN) pointer to the argument
*
* @return   True:       Argument is in long form
*           False:      Argument is not in long form
*
*******************************************************************************/
static bool ezmCli_IsArgumentLongForm(char * pu8LongFormArg)
{
    return (*pu8LongFormArg == '-' && ezmCli_IsArgumentShortForm(pu8LongFormArg + 1));
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
uint8_t ezmCli_IsLongFormArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm)
{
    uint8_t u8Index = ARG_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[u8CommandIndex].au32LongArgumenList[i] != NULL && 
                strcmp((char *)astMetaData[u8CommandIndex].au32LongArgumenList[i], pu8LongForm) == 0)
        {
            u8Index = i;
            CLIPRINT1("argument exists: [argument = %s]", pu8LongForm);   
            break;
        }
    }
    return u8Index;
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
* @param    u8CommandIndex: (IN) Index of the command metadata
* @param    *pu8LongForm:   (IN) Pointer to the buffer storing the argument
*
* @return   Index of the argument
*
*******************************************************************************/
uint8_t ezmCli_IsShortFormArgumentExist  (uint8_t u8CommandIndex,const char * pu8ShortForm)
{
    uint8_t u8Index = ARG_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[u8CommandIndex].au32ShordArgumenList[i] != NULL && 
                strcmp((char *)astMetaData[u8CommandIndex].au32ShordArgumenList[i], pu8ShortForm) == 0)
        {
            u8Index = i;
            CLIPRINT1("argument exists: [argument = %s]", pu8ShortForm);
            break;
        }
    }
    return u8Index;
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
* @param    u8Index:    (IN) Index of the command metadata
* @return   None
*
*******************************************************************************/
static void ezmCli_ResetValueList(uint8_t u8Index)
{
    if(u8Index < NUM_OF_CMD)
    {
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            astMetaData[u8Index].au8ValueList[i] = NULL;
        }
        CLIPRINT1("[index = %d]", u8Index); 
    }
}

#endif /* CLI */

/* End of file*/