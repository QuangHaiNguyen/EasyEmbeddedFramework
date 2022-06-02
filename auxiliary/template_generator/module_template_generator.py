__author__ = "Quang Hai Nguyen"
__copyright__ = "Copyright 2022, Easy Embedded"
__credits__ = ""
__license__ = "TBD"
__version__ = "1.0.1"
__maintainer__ = "Quang Hai Nguyen"
__email__ = "hainguyen.ezm@gmail.com"
__status__ = "Production"


from distutils.log import debug
import logging
import argparse
import os
from datetime import datetime

# create logger
logger = logging.getLogger('MODULE_TEMPLATE_GENERATOR')
logger.setLevel(logging.INFO)
logger.propagate = False

ch = logging.StreamHandler()
formatter = logging.Formatter('%(name)s::%(funcName)s::%(levelname)s::%(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)


# Create the parser
my_parser = argparse.ArgumentParser(prog = 'Module Source header template generator',
                                    description='Create the header and source file')

_file_header =\
"""
/*******************************************************************************
* Filename:         {0}
* Author:           {1}
* Original Date:    {2}
* Last Update:      {2}
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
* Copyright {1} - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by {1} {2}
*
*******************************************************************************/
"""

_doxygen_file_header=\
"""
/** @file   {0}
 *  @author {1}
 *  @date   {2}
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */
"""

_doxygen_header_body=\
"""
#ifndef _{0}_H
#define _{0}_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */
typedef struct
{{
    int a;  /**< an integer */
    int b;  /**< an integer */
}}aType;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
int sum(int a, int b); /**< Short description of function */

#endif /* _{0}_H */

/* End of file */

"""

_doxygen_source_body=\
"""
/******************************************************************************
* Includes
*******************************************************************************/
#include "{0}.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "{0}"       /**< module name */
#include "utilities/logging/logging.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : sum
*//** 
* @Description:
*
* This function initializes the ring buffer
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
int sum(int a, int b)
{{
    return a + b;
}}

/******************************************************************************
* Internal functions
*******************************************************************************/

/* End of file*/

"""

def _generate_header_file(file_name : str, args):
    """generate the header file

    Args:
        file_name (str): file name with path, it will be used by the open() function
        args (_type_): arguments from the CLI
    """
    logger.info("generating header file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    with open(file_name, "a") as header:
        header.write(_file_header.format(args.filename + ".h", args.author, dt_string))
        header.write(_doxygen_file_header.format(args.filename + ".h", args.author, dt_string))
        header.write(_doxygen_header_body.format(args.filename.upper()))
    logger.info("complete")
        
def _generate_source_file(file_name : str, args):
    """generate the source file

    Args:
        file_name (str): file name with path, it will be used by the open() function
        args (_type_): arguments from the CLI
    """
    logger.info("generating source file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    with open(file_name, "a") as header:
        header.write(_file_header.format(args.filename + ".c", args.author, dt_string))
        header.write(_doxygen_file_header.format(args.filename + ".c", args.author, dt_string))
        header.write(_doxygen_source_body.format(args.filename))
    logger.info("complete")

def main():
    """main, entry point of the application
    """
    # read arguments
    my_parser.add_argument( '-f',
                        '--filename', 
                        action='store', 
                        type=str, 
                        help='name of the source and header file without .c or .h')

    my_parser.add_argument( '-a',
                            '--author', 
                            action='store', 
                            type=str, 
                            help='name of the author of the module')

    my_parser.add_argument( '-d',
                            '--destination', 
                            action='store', 
                            type=str, 
                            help='where the files will be saved')
    args = my_parser.parse_args()

    # Create the file path
    headerFile = args.destination + "/" + args.filename + ".h"
    sourceFile = args.destination + "/" + args.filename + ".c"
    
    if os.path.exists(headerFile) or os.path.exists(sourceFile):
        logger.warning("file exists, do you want to overide it?")
        logger.warning("type Yes to overwrite:")
        _user_input = input("")
        
        if _user_input == "Yes":
            logger.debug("user type yes, continue to generate file")
            
            if os.path.exists(headerFile):
                os.remove(headerFile)

            if os.path.exists(sourceFile):
                os.remove(sourceFile)
        else:
            logger.info("terminated")
            quit()
            
    logger.info("author: {}".format(args.author))
    logger.info("filename: {}".format(args.filename))
    logger.info("destination: {}".format(args.destination))

    _generate_header_file(headerFile, args)
    _generate_source_file(sourceFile, args)

if __name__ == "__main__":
    main()