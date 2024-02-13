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
logger = logging.getLogger('CMAKE_TEMPLATE_GENERATOR')
logger.setLevel(logging.INFO)
logger.propagate = False

ch = logging.StreamHandler()
formatter = logging.Formatter('%(name)s::%(funcName)s::%(levelname)s::%(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

cmake_header="""\
# ----------------------------------------------------------------------------
# Author: {0}
# Name: {1}_lib
# License: This file is published under the license described in LICENSE.md
# Description: PLEASE ADD TEXT HERE
# ----------------------------------------------------------------------------
"""

cmake_lib="""
add_library({0}_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating {0} library build files")
message(STATUS "**********************************************************")

"""

cmake_body="""
# Source files ---------------------------------------------------------------
target_sources({0}_{1}
    PRIVATE
        # Please add source files here
)


# Definitions ----------------------------------------------------------------
target_compile_definitions({0}_{1}
    PUBLIC
        # Please add definitions here
)


# Include directory -----------------------------------------------------------
target_include_directories({0}_{1}
    PUBLIC
        # Please add public folders here
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries({0}_{1}
    PUBLIC
        # Please add public libraries
    PRIVATE
        # Please add private libraries
    INTERFACE
        # Please add interface libraries
)


# End of file
"""

# Create the parser
my_parser = argparse.ArgumentParser(prog = 'cmake template generator',
                                    description='Create a template for cmake file')

def is_tartget_name_valid(name:str) -> bool:
    """check if the target contain space or undercore. Do not check for
    special characters and co because it assumes that the users must know
    about cmake and special characters are not allowed.

    Args:
        name (str): target name

    Returns:
        bool: True if name is valid, else False
    """
    if " " in name or "-" in name:
        return False
    return True

def write_text_to_cmake_lib(author:str, name:str, file_handle):
    """write text to the cmake file

    Args:
        author (str): author's name
        name (str): target's name
        file_handle (_type_): file handle to write text data
    """
    file_handle.write(cmake_header.format(author, name))
    file_handle.write(cmake_lib.format(name))
    file_handle.write(cmake_body.format(name, "lib"))

def generate_cmake_lib(author:str, name:str, path:str) -> bool:
    """Generate a cmake file for a library

    Args:
        author (str): author's name
        name (str): target's name
        path (str): path of the result CMake

    Returns:
        bool: True if success, else false
    """
    if is_tartget_name_valid(name) == True:
        with open(path, "+a") as file:
            write_text_to_cmake_lib(author, name, file)
            return True
    else:
        logger.error("invalid targer name")
    return False

def main():
    """main, entry point of the application
    """
    # read arguments
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

    my_parser.add_argument("-l",
                            "--library",
                            action="store",
                            type=bool,
                            help="flag to indicate this cmake is a library or an executable")
    
    my_parser.add_argument( "-n",
                            "--name",
                            action="store",
                            type=str,
                            help="name of the library or executable")
    args = my_parser.parse_args()

    logger.info("author: {}".format(args.author))
    logger.info("destination: {}".format(args.destination))
    logger.info("name: {}".format(args.name))

    # Create the file path
    cmake_path = args.destination + "/CMakeLists.txt"
    
    if os.path.exists(cmake_path):
        logger.warning("file exists, do you want to overide it?")
        logger.warning("type Yes to overwrite:")
        _user_input = input("")
        
        if _user_input == "Yes":
            logger.debug("user type yes, continue to generate file")
            os.remove(cmake_path)
        else:
            logger.info("terminated")
            quit()

    if args.library == True:
        logger.info("cmake for library")
        generate_cmake_lib(args.author, args.name, cmake_path)
    else:
        logger.warning("not supported yet")

if __name__ == "__main__":
    main()