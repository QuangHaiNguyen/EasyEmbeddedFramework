#include "provisioning.h"

/** @brief definition TODO
 *
 */
typedef enum
{
	NOEVENT,
	/**< TODO comment*/

	SENDFRAME,
	/**< TODO comment*/

	GETFRAME,
	/**< TODO comment*/

	VERIFYFRAME,
	/**< TODO comment*/

	NUM_OF_EVENT,
}ENUM_EVENT;

/** @brief definition TODO
 *
 */
typedef enum
{
	INITIALIZATION,
	/**< TODO comment*/

	GETCM4ID,
	/**< TODO comment*/

	SENDSAMID,
	/**< TODO comment*/

	GETRANDCOMMAND,
	/**< TODO comment*/

	SENDRND,
	/**< TODO comment*/

	GETHELLOSAM,
	/**< TODO comment*/

	SENDHELLOCM4,
	/**< TODO comment*/

	GETFINACK,
	/**< TODO comment*/

	STATE_ERROR,
	NUM_OF_STATE,
}ENUM_STATE;

static EzmStateMachine stProvisioning;

/* State Handling section *****************************************************/
static uint8_t HandleInitialization (uint8_t eEvent, void * pstData);
static void HandleInitializationEntry (void);
static void HandleInitializationExit (void);

static uint8_t HandleGetCm4Id (uint8_t eEvent, void * pstData);
static void HandleGetCm4IdEntry (void);
static void HandleGetCm4IdExit (void);

static uint8_t HandleSendSamId (uint8_t eEvent, void * pstData);
static void HandleSendSamIdEntry (void);
static void HandleSendSamIdExit (void);

static uint8_t HandleGetRandCommand (uint8_t eEvent, void * pstData);
static void HandleGetRandCommandEntry (void);
static void HandleGetRandCommandExit (void);

static uint8_t HandleSendRnd (uint8_t eEvent, void * pstData);
static void HandleSendRndEntry (void);
static void HandleSendRndExit (void);

static uint8_t HandleGetHelloSam (uint8_t eEvent, void * pstData);
static void HandleGetHelloSamEntry (void);
static void HandleGetHelloSamExit (void);

static uint8_t HandleSendHelloCm4 (uint8_t eEvent, void * pstData);
static void HandleSendHelloCm4Entry (void);
static void HandleSendHelloCm4Exit (void);

static uint8_t HandleGetFinAck (uint8_t eEvent, void * pstData);
static void HandleGetFinAckEntry (void);
static void HandleGetFinAckExit (void);

static void HandleStateMachineError(void);
static uint8_t GetExternalEvent (void);

static EzmState astState[NUM_OF_STATE] = 
{
	{ "Initialization", &HandleInitialization, &HandleInitializationEntry, &HandleInitializationExit},
	{ "GetCm4Id", &HandleGetCm4Id, &HandleGetCm4IdEntry, &HandleGetCm4IdExit},
	{ "SendSamId", &HandleSendSamId, &HandleSendSamIdEntry, &HandleSendSamIdExit},
	{ "GetRandCommand", &HandleGetRandCommand, &HandleGetRandCommandEntry, &HandleGetRandCommandExit},
	{ "SendRnd", &HandleSendRnd, &HandleSendRndEntry, &HandleSendRndExit},
	{ "GetHelloSam", &HandleGetHelloSam, &HandleGetHelloSamEntry, &HandleGetHelloSamExit},
	{ "SendHelloCm4", &HandleSendHelloCm4, &HandleSendHelloCm4Entry, &HandleSendHelloCm4Exit},
	{ "GetFinAck", &HandleGetFinAck, &HandleGetFinAckEntry, &HandleGetFinAckExit},
};

void InitProvisioning(void)
{
	ezmStateMachine_Init(&stProvisioning,
							NOEVENT,
							INITIALIZATION,
							NUM_OF_EVENT,
							NUM_OF_STATE,
							astState,
							&HandleStateMachineError,
							&GetExternalEvent,
							NULL);
};

void RunProvisioning(void)
{
	ezmStateMachine_Execution(&stProvisioning);
};

/******************************************************************************
* Function : HandleStateMachineError
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleStateMachineError(void)
{
}

/******************************************************************************
* Function : GetExternalEvent
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static uint8_t GetExternalEvent (void)
{
}

/******************************************************************************
* Function : HandleInitialization
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLEINITIALIZATION section *****************************************************/

static uint8_t HandleInitialization (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = GETCM4ID;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = INITIALIZATION;
		break;
	}

	case GETFRAME:
	{
		u8NextState = INITIALIZATION;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = INITIALIZATION;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleInitializationEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleInitializationEntry(void)
{
}

/******************************************************************************
* Function : HandleInitializationExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleInitializationExit(void)
{
}

/******************************************************************************
* Function : HandleGetCm4Id
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLEGETCM4ID section *****************************************************/

static uint8_t HandleGetCm4Id (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = GETCM4ID;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = GETCM4ID;
		break;
	}

	case GETFRAME:
	{
		u8NextState = GETCM4ID;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = SENDSAMID;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleGetCm4IdEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetCm4IdEntry(void)
{
}

/******************************************************************************
* Function : HandleGetCm4IdExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetCm4IdExit(void)
{
}

/******************************************************************************
* Function : HandleSendSamId
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLESENDSAMID section *****************************************************/

static uint8_t HandleSendSamId (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = SENDSAMID;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = SENDSAMID;
		break;
	}

	case GETFRAME:
	{
		u8NextState = SENDSAMID;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = GETRANDCOMMAND;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleSendSamIdEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendSamIdEntry(void)
{
}

/******************************************************************************
* Function : HandleSendSamIdExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendSamIdExit(void)
{
}

/******************************************************************************
* Function : HandleGetRandCommand
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLEGETRANDCOMMAND section *****************************************************/

static uint8_t HandleGetRandCommand (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = GETRANDCOMMAND;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = GETRANDCOMMAND;
		break;
	}

	case GETFRAME:
	{
		u8NextState = GETRANDCOMMAND;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = SENDRND;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleGetRandCommandEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetRandCommandEntry(void)
{
}

/******************************************************************************
* Function : HandleGetRandCommandExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetRandCommandExit(void)
{
}

/******************************************************************************
* Function : HandleSendRnd
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLESENDRND section *****************************************************/

static uint8_t HandleSendRnd (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = SENDRND;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = SENDRND;
		break;
	}

	case GETFRAME:
	{
		u8NextState = SENDRND;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = GETHELLOSAM;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleSendRndEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendRndEntry(void)
{
}

/******************************************************************************
* Function : HandleSendRndExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendRndExit(void)
{
}

/******************************************************************************
* Function : HandleGetHelloSam
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLEGETHELLOSAM section *****************************************************/

static uint8_t HandleGetHelloSam (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = GETHELLOSAM;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = GETHELLOSAM;
		break;
	}

	case GETFRAME:
	{
		u8NextState = GETHELLOSAM;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = SENDHELLOCM4;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleGetHelloSamEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetHelloSamEntry(void)
{
}

/******************************************************************************
* Function : HandleGetHelloSamExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetHelloSamExit(void)
{
}

/******************************************************************************
* Function : HandleSendHelloCm4
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLESENDHELLOCM4 section *****************************************************/

static uint8_t HandleSendHelloCm4 (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = SENDHELLOCM4;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = SENDHELLOCM4;
		break;
	}

	case GETFRAME:
	{
		u8NextState = SENDHELLOCM4;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = GETFINACK;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleSendHelloCm4Entry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendHelloCm4Entry(void)
{
}

/******************************************************************************
* Function : HandleSendHelloCm4Exit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleSendHelloCm4Exit(void)
{
}

/******************************************************************************
* Function : HandleGetFinAck
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
/* HANDLEGETFINACK section *****************************************************/

static uint8_t HandleGetFinAck (uint8_t eEvent, void * pstData)
{
	uint8_t u8NextState = STATE_ERROR;

	switch(eEvent)
	{
	case NOEVENT:
	{
		u8NextState = GETFINACK;
		break;
	}

	case SENDFRAME:
	{
		u8NextState = GETFINACK;
		break;
	}

	case GETFRAME:
	{
		u8NextState = GETFINACK;
		break;
	}

	case VERIFYFRAME:
	{
		u8NextState = GETFINACK;
		break;
	}

	default:
	{
		break;
	}

	}
	return u8NextState;
};

/******************************************************************************
* Function : HandleGetFinAckEntry
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetFinAckEntry(void)
{
}

/******************************************************************************
* Function : HandleGetFinAckExit
*//** 
* \b Description:
*
* This function TODO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    a: (IN)TODO
*
* @return   None
*
*******************************************************************************/
static void HandleGetFinAckExit(void)
{
}

