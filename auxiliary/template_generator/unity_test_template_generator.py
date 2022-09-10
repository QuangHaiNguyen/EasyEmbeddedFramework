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
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
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

_doxygen_source_body=\
"""
/******************************************************************************
* Includes
*******************************************************************************/

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "{0}"       /**< module name */
#include "utilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"

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
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
/* None */


/******************************************************************************
* Internal functions
*******************************************************************************/
"""


_test_file_body=\
"""

TEST_GROUP({0});


TEST_SETUP({0})
{{
}}


TEST_TEAR_DOWN({0})
{{
}}


TEST({0}, TestTempPlate)
{{
    TEST_ASSERT_EQUAL(false, false);
}}

/* End of file*/

"""

_test_runner_file_body=\
"""

TEST_GROUP_RUNNER({0})
{{
    RUN_TEST_CASE({0}, TestTempPlate);
}}

"""

def _generate_file(file_path : str, author : str, module : str, generated_date : str):
    with open(file_path, "a") as header:
        header.write(_file_header.format(module + ".c",author, generated_date))
        header.write(_doxygen_file_header.format(module + ".c", author, generated_date))
        header.write(_doxygen_source_body.format(module))
        header.write(_test_file_body.format(module))
    logger.info("complete")
        

def _generate_test_file(file_name : str, args):
    """generate the source file

    Args:
        file_name (str): file name with path, it will be used by the open() function
        args (_type_): arguments from the CLI
    """
    logger.info("generating source file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    module = args.module 
    _generate_file(file_name, args.author, module, dt_string)


def _generate_test_runner_file(file_name : str, args):
    """_generate_test_runner_file

    Args:
        file_name (str): file name with path, it will be used by the open() function
        args (_type_): arguments from the CLI
    """
    logger.info("generating source file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    module = args.module + "_runner"
    _generate_file(file_name, args.author, module, dt_string)

def main():
    """main, entry point of the application
    """
    # read arguments
    my_parser.add_argument( '-m',
                        '--module', 
                        action='store', 
                        type=str, 
                        help='name of the module, which the test will be crated for')

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
    test_runner_file = args.destination + "/Test_" + args.module + "_Runner.c"
    test_file = args.destination + "/Test_" + args.module + ".c"
    
    if os.path.exists(test_runner_file) or os.path.exists(test_file):
        logger.warning("file exists, do you want to overide it?")
        logger.warning("type Yes to overwrite:")
        _user_input = input("")
        
        if _user_input == "Yes":
            logger.debug("user type yes, continue to generate file")
            
            if os.path.exists(test_runner_file):
                os.remove(test_runner_file)

            if os.path.exists(test_file):
                os.remove(test_file)
        else:
            logger.info("terminated")
            quit()
            
    logger.info("author: {}".format(args.author))
    logger.info("filename: {}".format(args.module))
    logger.info("destination: {}".format(args.destination))

    _generate_test_runner_file(test_runner_file, args)
    _generate_test_file(test_file, args)

if __name__ == "__main__":
    main()