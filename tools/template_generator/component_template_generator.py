__author__ =        "Hai Nguyen"
__credits__ =       "Hai Nguyen"
__license__ =       "This file is published under the license described in LICENSE.md"
__maintainer__ =    "Hai Nguyen"
__email__ =         "hainguyen.eeit@gmail.com"


from distutils.log import debug
import logging
import argparse
import os
from datetime import datetime

# create logger
logger = logging.getLogger('COMPONENT_TEMPLATE_GENERATOR')
logger.setLevel(logging.INFO)
logger.propagate = False

ch = logging.StreamHandler()
formatter = logging.Formatter('%(name)s::%(funcName)s::%(levelname)s::%(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)


# Create the parser
my_parser = argparse.ArgumentParser(prog = 'Component Source header template generator',
                                    description='Create the header and source file')

_file_header ="""\
/*****************************************************************************
* Filename:         {0}
* Author:           {1}
* Original Date:    {2}
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/
"""

_doxygen_file_header=\
"""
/** @file   {0}
 *  @author {1}
 *  @date   {2}
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */
"""

_doxygen_header_body=\
"""
#ifndef _{0}_H
#define _{0}_H

/*****************************************************************************
* Includes
*****************************************************************************/
/* INCLUDE HEADER HERE */

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief definition of a new type
 *
 */
typedef struct
{{
    int a;  /**< an integer */
    int b;  /**< an integer */
}}aType;


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function : sum
*//** 
* @brief one line description
*
* @details Detail description
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @pre Precondition
* @post Postcondition
*
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*****************************************************************************/
int sum(int a, int b);

#endif /* _{0}_H */


/* End of file */
"""

_doxygen_source_body=\
"""
/*****************************************************************************
* Includes
*****************************************************************************/
#include "{0}.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "{0}"       /**< module name */
#include "ez_logging.h"

/*the rest of include go here*/

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Public functions
*****************************************************************************/
int sum(int a, int b)
{{
    return a + b;
}}

/*****************************************************************************
* Local functions
*****************************************************************************/

/* End of file*/
"""

def generate_header_file(path : str, filename :str, author :str):
    """generate header file

    Args:
        path (str): path where the header is stored
        filename (str): name of the header file
        author (str): author
    """
    logger.info("generating header file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    path = os.path.join(path, filename + ".h")
    with open(path, "a") as header:
        header.write(_file_header.format(filename + ".h", author, dt_string))
        header.write(_doxygen_file_header.format(filename + ".h", author, dt_string))
        header.write(_doxygen_header_body.format(filename.upper()))
    logger.info("complete")
        
def generate_source_file(path:str, filename:str, author:str):
    """generate source file

    Args:
        path (str): path where the header is stored
        filename (str): name of the header file
        author (str): author
    """
    logger.info("generating source file")
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y")
    path = os.path.join(path, filename + ".c")
    with open(path, "a") as header:
        header.write(_file_header.format(filename + ".c", author, dt_string))
        header.write(_doxygen_file_header.format(filename + ".c", author, dt_string))
        header.write(_doxygen_source_body.format(filename))
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

    logger.info("author: {}".format(args.author))
    logger.info("filename: {}".format(args.filename))
    logger.info("destination: {}".format(args.destination))

    generate_header_file(args.destination, args.filename, args.author)
    generate_source_file(args.destination, args.filename, args.author)

if __name__ == "__main__":
    main()