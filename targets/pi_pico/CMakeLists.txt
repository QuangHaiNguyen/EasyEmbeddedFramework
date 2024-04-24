# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_target
# License: This file is published under the license described in LICENSE.md
# Description: CMake file for building linux target
# ----------------------------------------------------------------------------

add_executable(ez_target)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_target build files")
message(STATUS "**********************************************************")


# Include --------------------------------------------------------------------
include(features.cmake)

include(${CMAKE_SOURCE_DIR}/extern/pico-sdk/external/pico_sdk_import.cmake)
pico_sdk_init()


# Specify the cross compiler.
set(CMAKE_C_COMPILER  "/usr/bin/arm/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/arm/arm-none-eabi-cpp")
set(CMAKE_C_OUTPUT_EXTENSION .o)
set(CMAKE_ASM_COMPILER "/usr/bin/arm/arm-none-eabi-as")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>   -c <SOURCE>")
set(CMAKE_INCLUDE_FLAG_ASM "-I")

# Source files ---------------------------------------------------------------
target_sources(ez_target
    PRIVATE
        main.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_target
    PUBLIC
        # Please add definitions here
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_target
    PUBLIC
        # Please add private folders here
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_target
    PUBLIC
        # Please add public libraries
    PRIVATE
        easy_embedded_lib
        pico_stdlib
    INTERFACE
        # Please add interface libraries
)


# End of file
