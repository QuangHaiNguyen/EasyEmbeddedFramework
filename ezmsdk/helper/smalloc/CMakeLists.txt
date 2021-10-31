cmake_minimum_required( VERSION 2.8 )

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )

## C compiler options
set( CMAKE_CXX_FLAGS "-Wall -Wextra" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

set (DEPENDENCIES ${CMAKE_SOURCE_DIR}/helper/linked_list/linked_list.c ${CMAKE_SOURCE_DIR}/helper/hexdump/hexdump.c)


# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

########################################################################
# Project section 
########################################################################
## Specify a project name
project(smalloc_project)

## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" 
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()

add_library(smalloc smalloc.c ${DEPENDENCIES})

target_include_directories(smalloc PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with smalloc

#include smalloc
add_subdirectory(${PROJECT_SOURCE_DIR}/unit_test)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()


########################################################################
# cpp check 
########################################################################
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
  message("Path: ${CPPCHECK}")

  set(CPPCHECK_TEMPLATE "cppcheck:{file}:{line}: {severity}:{message}")

  message (${CPPCHECK})
  add_custom_target(cppcheck ALL COMMAND ${CPPCHECK}
      --enable=all
      --inconclusive
      --force
      --inline-suppr
      --template=${CPPCHECK_TEMPLATE}
      --verbose
      --output-file=cppcheck_output.txt
      -i ${PROJECT_SOURCE_DIR}/build
      -i ${PROJECT_SOURCE_DIR}/unit_test
      ${PROJECT_SOURCE_DIR}
    COMMENT "Checking ..."
    VERBATIM
  )
  message("debug value : ${CMAKE_CXX_CPPCHECK}")
else()
  message(SEND_ERROR "cppcheck requested but executable not found")
endif()