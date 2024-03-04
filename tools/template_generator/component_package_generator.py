__author__ =        "Hai Nguyen"
__credits__ =       "Hai Nguyen"
__license__ =       "This file is published under the license described in LICENSE.md"
__maintainer__ =    "Hai Nguyen"
__email__ =         "hainguyen.eeit@gmail.com"

import logging
import argparse
import os
import cmake_template_generator as cmake_gen
import component_template_generator as comp_gen
import unity_test_template_generator as test_gen
import shutil


# create logger
logger = logging.getLogger('COMPONENT_PCKG_GENERATOR')
logger.setLevel(logging.INFO)
logger.propagate = False

ch = logging.StreamHandler()
formatter = logging.Formatter('%(name)s::%(funcName)s::%(levelname)s::%(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)


# Create the parser
my_parser = argparse.ArgumentParser(prog = 'Module Source header template generator',
                                    description='Create the header and source file')

def main():
    """main, entry point of the application
    """
    # read arguments
    my_parser.add_argument( '-c',
                            '--component', 
                            action='store', 
                            type=str, 
                            help='name of the component')

    my_parser.add_argument( '-a',
                            '--author', 
                            action='store', 
                            type=str, 
                            help='name of the author of the component')

    my_parser.add_argument( '-d',
                            '--destination', 
                            action='store', 
                            type=str, 
                            help='where the component will be saved')

    args = my_parser.parse_args()

    component_name = "ez_" + args.component
    path_src = os.path.join(args.destination, args.component)
    path_test = os.path.join(args.destination, args.component, "unit_test")

    if os.path.exists(path_src) == True:
        logger.warning("folder exists. The existing folder will be overwritten")
        shutil.rmtree(path_src)
    os.mkdir(path_src)
    os.mkdir(path_test)

    cmake_gen.generate_cmake_lib(args.author, component_name, path_src)
    comp_gen.generate_header_file(path_src,
                                  component_name,
                                  args.author)
    comp_gen.generate_source_file(path_src,
                                  component_name,
                                  args.author)
    test_gen.generate_test_file(path_test,
                                component_name,
                                args.author)
    cmake_gen.generate_cmake_test(args.author, component_name, path_test)

if __name__ == "__main__":
    main()