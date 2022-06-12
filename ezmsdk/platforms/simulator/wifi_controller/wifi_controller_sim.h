
/*******************************************************************************
* Filename:         wifi_controller_sim.h
* Author:           Hai Nguyen
* Original Date:    12.06.2022
* Last Update:      12.06.2022
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
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 12.06.2022
*
*******************************************************************************/

/** @file   wifi_controller_sim.h
 *  @author Hai Nguyen
 *  @date   12.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _WIFI_CONTROLLER_SIM_H
#define _WIFI_CONTROLLER_SIM_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

#include "app/app_config.h"
#include "hal/wifi_controller/wifi_controller.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool wifiSim_Initialization(void);
bool wifiSim_BindingDriverApi(void * api);
bool wifiSim_RegisterInterruptCallback(INTERRUPT_CALLBACK callback);

#endif /* _WIFI_CONTROLLER_SIM_H */

/* End of file */

