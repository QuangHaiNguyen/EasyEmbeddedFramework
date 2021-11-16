cmake_minimum_required( VERSION 2.8 )

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )

## C compiler options
set( CMAKE_CXX_FLAGS "-Wall -Wextra" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

#set (DEPENDENCIES ${CMAKE_SOURCE_DIR}/../../helper/linked_list/linked_list.c ${CMAKE_SOURCE_DIR}/../../smalloc/smalloc.c)

########################################################################
# Download and unpack googletest at configure time 
########################################################################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

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
project(uart_project)


## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" 
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()

add_library(uart uart.c)

#add dependency if nedded
#add_dependencies(main my_math)

target_include_directories(uart PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with uart
#target_link_libraries(main my_math)

#include uart
#add_subdirectory(${PROJECT_SOURCE_DIR}/math)
add_subdirectory(${PROJECT_SOURCE_DIR}/unit_test)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()

########################################################################
# Doxygen 
########################################################################
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    # first we can indicate the documentation build as an option and set it to ON by default
    option(BUILD_DOC "Build documentation" ON)

    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
      message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()

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