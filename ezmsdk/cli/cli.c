/*******************************************************************************
* Title                 :   cli 
* Filename              :   cli.c
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

/** @file  cli.c
 *  @brief This is the source template for a cli
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "cli.h"
#include "../ezmDebug/ezmDebug.h"
#include "../helper/smalloc/smalloc.h"
#include "string.h"

#if (CLI == 1U)

#define VERBOSE                 1U
#define CLI_INDEX_INVALID       0xffU

#define CLI_BUFF_SIZE       512U

#if (MODULE_DEBUG == 1U) && (CLI_DEBUG == 1U)
    #define CLIPRINT1(a)                    PRINT_DEBUG1(a)               
    #define CLIPRINT2(a,b)                  PRINT_DEBUG2(a,b)             
    #define CLIPRINT3(a,b,c)                PRINT_DEBUG3(a,b,c)
    #define CLIPRINT4(a,b,c,d)              PRINT_DEBUG4(a,b,c,d)  
#else 
    #define CLIPRINT1(a)           
    #define CLIPRINT2(a,b)           
    #define CLIPRINT3(a,b,c)
    #define CLIPRINT4(a,b,c,d)
#endif



/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */
typedef struct 
{
    const char *    pu8Command;                         /**< */
    const char *    pu8Description;                     /**< */
    CLI_CALLBACK    pfnCallback;                        /**< */
    uint32_t *      au32LongArgumenList[NUM_OF_ARG];    /**< */
    uint32_t *      au32ShordArgumenList[NUM_OF_ARG];   /**< */
    uint32_t *      au32DescriptionList[NUM_OF_ARG];    /**< */
    uint32_t *      au32ValueList[NUM_OF_ARG];          /**< */
}CommandMetadata;

typedef enum 
{
    STATE_COMMAND,
    STATE_ARGUMENT,
    STATE_VALUE,
}ENUM_CLI_STATE;
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static CommandMetadata astMetaData[NUM_OF_CMD];
static ENUM_CLI_STATE  eState = STATE_COMMAND;

/******************************************************************************
* Function Definitions
*******************************************************************************/
uint8_t     ezmCli_GetFreeInstance  (void);
bool        ezmCli_ResetMetaData    (uint8_t u8Index);
bool        ezmCli_IsIndexExist     (uint8_t u8Index);
bool        ezmCli_IsCommandExist   (const char * pu8Command, uint8_t * pu8Index);
uint8_t     ezmCli_IsLongFormArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm);
uint8_t     ezmCli_IsShortFormArgumentExist  (uint8_t u8CommandIndex,const char * pu8ShortForm);
void        ezmCli_PrintCommandHelp      (uint8_t u8Index);
static void ezmCli_PrintMenu (void);
static bool ezmCli_IsArgumentShortForm(char * pu8ShortFormArg);
static bool ezmCli_IsArgumentLongForm(char * pu8LongFormArg);
/* Public function ***********************************************************/

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
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
uint8_t ezmCli_RegisterCommand(const char * pu8Command, const char *  pu8Description, CLI_CALLBACK pfnCallback)
{
    bool bSuccess = true;
    uint8_t u8Index = CLI_INDEX_INVALID;
    
    if(pu8Command == NULL || pfnCallback == NULL || pu8Description == NULL)
    {
        bSuccess = false;
        CLIPRINT1("Input parameter is NULL");
    }

    if(ezmCli_IsCommandExist(pu8Command, NULL))
    {
        bSuccess = false;
    }

    if(bSuccess)
    {
        u8Index = ezmCli_GetFreeInstance();
        if(u8Index == CLI_INDEX_INVALID)
        {
            bSuccess = false;
            CLIPRINT1("Could not find free instance");
        }
    }

    if(bSuccess)
    {
        astMetaData[u8Index].pfnCallback = pfnCallback;
        astMetaData[u8Index].pu8Command = pu8Command;
        astMetaData[u8Index].pu8Description = pu8Description;
        CLIPRINT3("Add new command: [command = %s] [description = %s]", pu8Command, pu8Description);
    }
    else
    {
        CLIPRINT1("ezmCli_RegisterCommand() FAILED");
    }

    return u8Index;
}


/******************************************************************************
* Function : sum
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezmCli_AddArgument ( uint8_t u8CommandIndex, 
                            const char * pu8LongForm, 
                            const char * pu8ShortForm, 
                            const char * pu8Description)
{
    bool bResult = true;

    if (bResult)
    {
        if(!ezmCli_IsIndexExist(u8CommandIndex))
        {
            bResult = false;
        }
    }

    if(pu8LongForm == NULL || pu8ShortForm == NULL || pu8Description == NULL)
    {
        bResult = false;
    }
    
    
    if(bResult)
    {
        if(ezmCli_IsLongFormArgumentExist(u8CommandIndex, pu8LongForm) != CLI_INDEX_INVALID || 
            ezmCli_IsShortFormArgumentExist(u8CommandIndex, pu8ShortForm) != CLI_INDEX_INVALID)
        {
            bResult = false;
        }
    }
    
    if(bResult)
    {
        bResult = false;
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[u8CommandIndex].au32LongArgumenList[i] == NULL)
            {
                astMetaData[u8CommandIndex].au32LongArgumenList[i] = (uint32_t *)pu8LongForm;
                astMetaData[u8CommandIndex].au32ShordArgumenList[i] = (uint32_t *)pu8ShortForm;
                astMetaData[u8CommandIndex].au32DescriptionList[i] = (uint32_t *)pu8Description;

                CLIPRINT4("Add new argument: [long = %s] [shord = %s] [description = %s]",
                            (char *)astMetaData[u8CommandIndex].au32LongArgumenList[i],
                            (char *)astMetaData[u8CommandIndex].au32ShordArgumenList[i],
                            (char *)astMetaData[u8CommandIndex].au32DescriptionList[i]);
                bResult = true;
                break;
            }
        }   
    }
    return bResult;
}

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
* @param    u8NotifyCode:   (IN) notification code
* @param    *pu32Param1:    (IN) pointer to the parameter 1
* @param    *pu32Param2:    (IN) pointer to the parameter 2
*
* @return   None
*
*******************************************************************************/
bool ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode, void * pu32Param1, void * pu32Param2)
{
    char * pu8Buffer = (char *)pu32Param1;
    char * pu8Helper = pu8Buffer;
    uint8_t u8CommandIndex = CLI_INDEX_INVALID;
    uint8_t u8ValueIndex = CLI_INDEX_INVALID;
    uint16_t u16Count = 0;
    bool bResult = true;

    eState = STATE_COMMAND;

    CLIPRINT2("Receive data: [data = %s]", pu8Buffer);

    for(uint16_t i = 0; i < CLI_BUFF_SIZE - 1; i++)
    {
        if(*(pu8Buffer + i) == ' ')
        {
            u16Count = u16Count + 1;
        }
        else
        {
            break;
        }
    }
    pu8Helper = pu8Buffer + u16Count;

    /* Search command */
    for(uint16_t i = u16Count; i < CLI_BUFF_SIZE; i++)
    {
        if(bResult == false)
        {
            break;
        }

        switch (eState)
        {
        case STATE_COMMAND:
            if(*(pu8Buffer + i) == ' ')
            {
                *(pu8Buffer + i) = '\0';
                CLIPRINT2("Receive command: [command = %s]", pu8Helper);

                if(ezmCli_IsCommandExist((char *)pu8Helper, &u8CommandIndex))
                {
                    eState = STATE_ARGUMENT;
                    pu8Helper = (pu8Buffer + i + 1);

                    /* remove next white space*/
                    while(i < CLI_BUFF_SIZE - 1 && *(pu8Buffer + i + 1) == ' ')
                    {
                        i = i + 1;
                    }
                    pu8Helper = (pu8Buffer + i + 1);
                }
                else
                {
                    /* command is not exist, print help*/
                    ezmCli_PrintMenu();
                    bResult = false; 
                }
            }
            else if (*(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
            {
                bResult = false;
                if(i == u16Count)
                {
                    /* missing command*/
                    CLIPRINT1("seriously, where is your command???");
                    ezmCli_PrintMenu();
                }
                else
                {
                    /* missing argument*/
                    CLIPRINT1("Missing argument");
                    ezmCli_PrintCommandHelp(u8CommandIndex);
                }
            }
            else
            {
                /* Do nothing */
            }

            break;
        
        case STATE_ARGUMENT:
            if(*(pu8Buffer + i) == ' ')
            {
                *(pu8Buffer + i) = '\0';
                CLIPRINT2("Receive argument: [argument = %s]", pu8Helper);

                if(ezmCli_IsArgumentLongForm(pu8Helper))
                {
                    u8ValueIndex = ezmCli_IsLongFormArgumentExist(u8CommandIndex, pu8Helper);
                    if(CLI_INDEX_INVALID != u8ValueIndex)
                    {
                        eState = STATE_VALUE;
                        CLIPRINT2("Value index: [idx = %d]", u8ValueIndex);
                    }
                }
                else if (ezmCli_IsArgumentShortForm(pu8Helper))
                {
                    u8ValueIndex = ezmCli_IsShortFormArgumentExist(u8CommandIndex, pu8Helper);
                    if(CLI_INDEX_INVALID != u8ValueIndex)
                    {
                        eState = STATE_VALUE;
                        CLIPRINT2("Value index: [idx = %d]", u8ValueIndex);
                    }
                }
                else
                {
                    /* Wrong format*/
                    CLIPRINT1("Argument in wrong format");
                    ezmCli_PrintCommandHelp(u8CommandIndex);
                    bResult = false;  
                }

                /* remove white space*/
                while(i < CLI_BUFF_SIZE - 1 && *(pu8Buffer + i + 1) == ' ')
                {
                    i = i + 1;
                }
                pu8Helper = (pu8Buffer + i + 1);
            }
            else if (*(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
            {
                /* missing value*/
                CLIPRINT1("Missing value");
                ezmCli_PrintCommandHelp(u8CommandIndex);
                bResult = false;  
            }
            else
            {
                /* Do nothing */
            }
            
            break;

        case STATE_VALUE:
            if(*(pu8Buffer + i) == ' ')
            {
                *(pu8Buffer + i) = '\0';
                CLIPRINT2("Receive value: [value = %s]", pu8Helper);
                astMetaData[u8CommandIndex].au32ValueList[u8ValueIndex] = (uint32_t * )pu8Helper;
                eState = STATE_ARGUMENT;
            }
            else if (*(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
            {
                *(pu8Buffer + i) = '\0';
                CLIPRINT2("Receive value: [value = %s]", pu8Helper);
                astMetaData[u8CommandIndex].au32ValueList[u8ValueIndex] = (uint32_t * )pu8Helper;
                
                /* Execute the callback */
                astMetaData[u8CommandIndex].pfnCallback(astMetaData[u8CommandIndex].pu8Command, astMetaData[u8CommandIndex].au32ValueList);

                /* End of command, use next line to terminate the loop*/
                i = CLI_BUFF_SIZE; 
            }
            else
            {
                /* do nothing */
            }

            /* remove white space*/
            while(i < CLI_BUFF_SIZE - 1 && *(pu8Buffer + i + 1) == ' ')
            {
                i = i + 1;
            }
            pu8Helper = (pu8Buffer + i + 1);
            break;
        
        default:
            break;
        }
    }

    (void)(pu32Param2);
    (void)(u8NotifyCode);

    return bResult;
}

/* Private function **********************************************************/

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
bool ezmCli_ResetMetaData (uint8_t u8Index)
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
            astMetaData[u8Index].au32ValueList[i] =NULL;
        }
        bResult = true;
        CLIPRINT2("Reset metadata: [index = %d]", u8Index); 
    }

    return bResult;
}

/******************************************************************************
* Function : ezmCli_IsIndexExist
*//** 
* \b Description:
*
* This function checks if the index of the command is available
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u8Index:    (IN) index of the command
* @return   True:       Command is existing
*           False:      Command is not existing
*
* \b Example Example:
* @code
* if(ezmCli_IsIndexExist(0U))
* {
*       printf("command exists")
* }
* @endcode
*
*******************************************************************************/
bool ezmCli_IsIndexExist (uint8_t u8Index)
{
    bool bResult = false;

    for (uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if (i == u8Index)
        {
            bResult = true;
            break;
        }
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
* @return   CLI_INDEX_INVALID if there is no free instance, otherwise the index
*           of the intsance
*
* \b Example Example:
* @code
* uint8_t u8FreeInstance = ezmCli_GetFreeInstance()
* @endcode
*
*******************************************************************************/
uint8_t ezmCli_GetFreeInstance (void)
{
    uint8_t u8FreeInstanceIndex = CLI_INDEX_INVALID;

    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        if(astMetaData[i].pu8Command == NULL)
        {
            u8FreeInstanceIndex = i;
            CLIPRINT2("Found free instance: [index = %d]", u8FreeInstanceIndex);
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
bool ezmCli_IsCommandExist   (const char * pu8Command, uint8_t * pu8Index)
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
            
            CLIPRINT2("Command is existing: [command = %s]", pu8Command);
            break;
        }
    }
    return bIsExist;
}


void ezmCli_PrintCommandHelp (uint8_t u8Index)
{
    if(u8Index < NUM_OF_CMD && astMetaData[u8Index].pu8Command != NULL)
    {
        PRINTF_NO_NL("%s", "usage: ");
        PRINTF_NO_NL("%s ", astMetaData[u8Index].pu8Command);

        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[u8Index].au32ShordArgumenList[i] != NULL)
            {
                PRINTF_NO_NL("[%s] ", (char *)astMetaData[u8Index].au32ShordArgumenList[i]);
            }
        }
        PRINT1("\n");
        PRINT1("Argument options:");
        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
        {
            if(astMetaData[u8Index].au32ShordArgumenList[i] != NULL)
            {
                PRINTF_NO_NL("\t%s, ", (char *)astMetaData[u8Index].au32ShordArgumenList[i]);
                PRINTF_NO_NL("%s ", (char *)astMetaData[u8Index].au32LongArgumenList[i]);
                PRINTF_NO_NL("\t\t%s \n", (char *)astMetaData[u8Index].au32DescriptionList[i]);
            }
        }

    }
}

static void ezmCli_PrintMenu (void)
{
    PRINT1("\nList of commands:");
    for(uint8_t i = 0; i < NUM_OF_CMD; i++)
    {
        ezmCli_PrintCommandHelp(i);
    }
    PRINT1("\n");
}

static bool ezmCli_IsArgumentShortForm(char * pu8ShortFormArg)
{
    bool bResult = false;
    if(*pu8ShortFormArg == '-')
    {
        if(( *(pu8ShortFormArg + 1) >= 'a' && *(pu8ShortFormArg + 1) <= 'z') || 
            ( *(pu8ShortFormArg + 1) >= 'A' && *(pu8ShortFormArg + 1) <= 'Z'))
        {
            bResult = true;
        }
    } 
    return  bResult;
}
static bool ezmCli_IsArgumentLongForm(char * pu8LongFormArg)
{
    bool bResult = false;
    if(*pu8LongFormArg == '-' && *(pu8LongFormArg + 1) == '-')
    {
        if(( *(pu8LongFormArg + 2) >= 'a' && *(pu8LongFormArg + 2) <= 'z') || 
            ( *(pu8LongFormArg + 2) >= 'A' && *(pu8LongFormArg + 2) <= 'Z'))
        {
            bResult = true;
        }
    }
    return  bResult;
}

uint8_t ezmCli_IsLongFormArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm)
{
    uint8_t u8Index = CLI_INDEX_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[u8CommandIndex].au32LongArgumenList[i] != NULL && 
                strcmp((char *)astMetaData[u8CommandIndex].au32LongArgumenList[i], pu8LongForm) == 0)
        {
            u8Index = i;
            CLIPRINT2("argument exists: [argument = %s]", pu8LongForm);   
            break;
        }
    }
    return u8Index;
}

uint8_t ezmCli_IsShortFormArgumentExist  (uint8_t u8CommandIndex,const char * pu8ShortForm)
{
    uint8_t u8Index = CLI_INDEX_INVALID;
    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[u8CommandIndex].au32ShordArgumenList[i] != NULL && 
                strcmp((char *)astMetaData[u8CommandIndex].au32ShordArgumenList[i], pu8ShortForm) == 0)
        {
            u8Index = i;
            CLIPRINT2("argument exists: [argument = %s]", pu8ShortForm);   
            break;
        }
    }
    return u8Index;
}
#endif /* CLI */

/* End of file*/