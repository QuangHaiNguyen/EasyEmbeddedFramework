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

#define VERBOSE                 0U
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

uint8_t         au8CommandBuffer[CLI_BUFF_SIZE] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
uint8_t ezmCli_GetFreeInstance  (void);
bool    ezmCli_ResetMetaData    (uint8_t u8Index);
bool    ezmCli_IsIndexExist     (uint8_t u8Index);
bool    ezmCli_IsCommandExist   (const char * pu8Command, uint8_t * pu8Index);
bool    ezmCli_IsArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm, const char * pu8ShortForm);
void    ezmCli_PrintCommandHelp      (uint8_t u8Index);
static void    ezmCli_PrintMenu (void);
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

    if(pu8LongForm == NULL || pu8ShortForm == NULL || pu8Description == NULL)
    {
        bResult = false;
    }

    if (bResult)
    {
        if(!ezmCli_IsIndexExist(u8CommandIndex))
        {
            bResult = false;
        }
    }

    if(bResult)
    {
        if(ezmCli_IsArgumentExist(u8CommandIndex, pu8LongForm, pu8ShortForm))
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
void ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode, void * pu32Param1, void * pu32Param2)
{
    uint8_t * pu8Buffer = (uint8_t *)pu32Param1;
    uint8_t * pu8Helper = pu8Buffer;
    uint8_t u8CommandIndex = CLI_INDEX_INVALID;
    uint8_t u8ValueIndex = CLI_INDEX_INVALID;
    CLIPRINT2("Receive data: [data = %s]", pu8Buffer);
    

    switch(u8NotifyCode)
    {
    case 0:
        /* OK case */
        /* Search command */

        for(uint16_t i = 0; i < CLI_BUFF_SIZE; i++)
        {

            switch (eState)
            {
            case STATE_COMMAND:
                if(*(pu8Buffer + i) == ' ' || *(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
                {
                    *(pu8Buffer + i) = '\0';
                    CLIPRINT2("Receive command: [command = %s]", pu8Helper);

                    if(ezmCli_IsCommandExist((char *)pu8Helper, &u8CommandIndex))
                    {
                        
                        eState = STATE_ARGUMENT;
                        pu8Helper = (uint8_t *)(pu8Buffer + i + 1);
                    }
                    else
                    {
                        /* command is not exist, print help*/
                        ezmCli_PrintMenu();
                        /* terminate the for loop and state machine */
                        i = CLI_BUFF_SIZE; 
                    }
                }
                break;
            
            case STATE_ARGUMENT:
                if(*(pu8Buffer + i) == ' ' || *(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
                {
                    *(pu8Buffer + i) = '\0';
                    CLIPRINT2("Receive argument: [argument = %s]", pu8Helper);

                    if(ezmCli_IsArgumentLongForm(pu8Helper))
                    {
                        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
                        {
                            if(strcmp((char *)astMetaData[u8CommandIndex].au32LongArgumenList[i], pu8Helper) == 0 && 
                                astMetaData[u8CommandIndex].au32LongArgumenList[i] != NULL)
                            {
                                u8ValueIndex = i;
                                eState = STATE_VALUE;
                                CLIPRINT2("Value index: [idx = %d]", u8ValueIndex);
                                break;
                            }
                        }
                    }
                    else if (ezmCli_IsArgumentShortForm(pu8Helper))
                    {
                        for(uint8_t i = 0; i < NUM_OF_ARG; i++)
                        {
                            if(strcmp((char *)astMetaData[u8CommandIndex].au32ShordArgumenList[i], pu8Helper) == 0 && 
                                astMetaData[u8CommandIndex].au32ShordArgumenList[i] != NULL)
                            {
                                u8ValueIndex = i;
                                eState = STATE_VALUE;
                                CLIPRINT2("Value index: [idx = %d]", u8ValueIndex);
                                break;
                            }
                        }
                    }
                    else
                    {
                        /* Wrong format*/
                        CLIPRINT1("Argument in wrong format");
                        ezmCli_PrintCommandHelp(u8CommandIndex);
                        i = CLI_BUFF_SIZE; 
                    }
                    pu8Helper = (uint8_t *)(pu8Buffer + i + 1);
                }
                break;

            case STATE_VALUE:
                if(*(pu8Buffer + i) == ' ' || *(pu8Buffer + i) == '\r' || *(pu8Buffer + i) == '\n')
                {
                    *(pu8Buffer + i) = '\0';
                    CLIPRINT2("Receive value: [value = %s]", pu8Helper);
                }
                break;
            
            default:
                break;
            }
        }

        break;

    default:
        break;
    }

    (void)(pu32Param2);
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

/******************************************************************************
* Function : ezmCli_IsArgumentExist
*//** 
* \b Description:
*
* This function checks if the argument exists
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u8CommandIndex: (IN) index of the command
* @param    *pu8LongForm:   (IN) point to the argument in long form
* @param    *pu8ShortForm:  (IN) point to the argument in short form
*
* @return   True:   Argument is existing
*           False:  Argument is not existing
*
* \b Example Example:
* @code
* if(ezmCli_IsArgumentExist(1U, "-arg1", "-a1"))
* {
*       printf("argument exists")
* }
* @endcode
*
*******************************************************************************/
bool ezmCli_IsArgumentExist  (uint8_t u8CommandIndex, const char * pu8LongForm, const char * pu8ShortForm)
{
    bool bResult = false;

    for(uint8_t i = 0; i < NUM_OF_ARG; i++)
    {
        if(astMetaData[u8CommandIndex].au32LongArgumenList[i] != NULL && 
                strcmp((char *)astMetaData[u8CommandIndex].au32LongArgumenList[i], pu8LongForm) == 0)
        {
            bResult = true;
            CLIPRINT2("argument exists: [argument = %s]", pu8LongForm);   
            break;
        }

        if( astMetaData[u8CommandIndex].au32ShordArgumenList[i] != NULL &&
            strcmp((char *)astMetaData[u8CommandIndex].au32ShordArgumenList[i], pu8ShortForm) == 0 )
        {
            bResult = true;
            CLIPRINT2("argument exists: [argument = %s]", pu8ShortForm);   
            break;
        }
    }

    return bResult;
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

#endif /* CLI */

/* End of file*/