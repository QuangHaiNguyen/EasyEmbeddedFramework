cmake_minimum_required( VERSION 2.8 )

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )

## C compiler options
set( CMAKE_CXX_FLAGS "-Wall -Wextra" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

set (DEPENDENCIES ${CMAKE_SOURCE_DIR}/helper/smalloc/smalloc.c ${CMAKE_SOURCE_DIR}/helper/linked_list/linked_list.c ${CMAKE_SOURCE_DIR}/helper/ezmAssert/ezmAssert.c)

########################################################################
# Project section 
########################################################################
## Specify a project name
project(binCmdParser_project)


## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" 
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()

add_library(binCmdParser binCmdParser.c ${DEPENDENCIES})

#add dependency if nedded
#add_dependencies(main my_math)

target_include_directories(binCmdParser PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with binCmdParser
#target_link_libraries(main my_math)

#include binCmdParser
#add_subdirectory(${PROJECT_SOURCE_DIR}/math)
add_subdirectory(${PROJECT_SOURCE_DIR}/unit_test)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()
