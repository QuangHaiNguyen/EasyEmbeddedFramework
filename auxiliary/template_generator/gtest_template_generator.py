__author__ = "Quang Hai Nguyen"
__copyright__ = "Copyright 2022, Easy Embedded"
__credits__ = ""
__license__ = "TBD"
__version__ = "1.0.1"
__maintainer__ = "Quang Hai Nguyen"
__email__ = "hainguyen.ezm@gmail.com"
__status__ = "Production"

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
my_parser = argparse.ArgumentParser(prog = 'gtest template generator',
                                    description='Create a template for gtest')

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

_test_header =\
"""
#ifndef UNIT_TEST_{0}
#define UNIT_TEST_{0}

#include <limits.h>
#include "gtest/gtest.h"

extern "C" {{
}}
"""

_test_footer =\
"""
#endif /* UNIT_TEST_{0} */
"""

_test_body =\
"""
namespace 
{
    TEST(test_case_name, test_name) 
    {
    }
}
"""

def _generate_test_file(file_name : str, args):
    """Generate gtest file. It is a header file

    Args:
        file_name (str): file handle for writing data
        args (_type_): list of arguments
    """
    logger.info("generating test file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    with open(file_name, "a") as header:
        header.write(_file_header.format(args.filename + ".h", 
                                         args.author, dt_string))
        header.write(_test_header.format(args.filename.upper()))
        header.write(_test_body)
        header.write(_test_footer.format(args.filename.upper()))
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
    testFile = args.destination + "/" + args.filename + ".h"
    
    if os.path.exists(testFile):
        logger.warning("file exists, do you want to overide it?")
        logger.warning("type Yes to overwrite:")
        _user_input = input("")
        
        if _user_input == "Yes":
            logger.debug("user type yes, continue to generate file")
            
            if os.path.exists(testFile):
                os.remove(testFile)

        else:
            logger.info("terminated")
            quit()
            
    logger.info("author: {}".format(args.author))
    logger.info("filename: {}".format(args.filename))
    logger.info("destination: {}".format(args.destination))
    
    _generate_test_file(testFile, args)
    
if __name__ == "__main__":
    main()