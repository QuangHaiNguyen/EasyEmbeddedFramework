/*******************************************************************************
* Title                 :   ezmAssert
* Filename              :   ezmAssert.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.05.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.05.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   ezmAssert.h
 *  @brief  Header template for a Assert
 */


#ifndef _EZMASSERT_H
#define _EZMASSERT_H

/*******************************************************************************
* Includes
*******************************************************************************/
/*None*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ASSERT(expr) \
    if (expr) \
        {} \
    else \
        ezmAssert_AssertFailHandler(MODULE_ID, __LINE__);
/**< Assert operation*/

#define REGISTER_ASSERT(module_id) \
    enum { MODULE_ID=module_id }; \
    void _dummy##module_id(void) {}
/**< register your module id*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/*None*/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/*None*/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void ezmAssert_AssertFailHandler(int u32ModuleId, int u32LineNumer); /*Handling when assert fail*/

#endif /* _EZMASSERT_H */

/* End of file*/
