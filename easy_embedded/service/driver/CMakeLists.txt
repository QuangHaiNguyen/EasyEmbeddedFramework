# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_driver_lib
# License: This file is published under the license described in LICENSE.md
# Description: Cmake file for driver component
# ----------------------------------------------------------------------------

add_library(ez_driver_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_driver_lib library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/easy_embedded)


# Sub directory --------------------------------------------------------------


# Source files ---------------------------------------------------------------
target_sources(ez_driver_lib
    PRIVATE
        $<$<BOOL:${ENABLE_EZ_UART}>:uart/ez_uart.c>
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_driver_lib
    PUBLIC
        EZ_DRIVER_ENABLE=$<BOOL:${ENABLE_EZ_HAL_DRIVER}>
        EZ_UART_ENABLE=$<BOOL:${ENABLE_EZ_UART}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_driver_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        uart
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_driver_lib
    PUBLIC
        # Unused
    PRIVATE
        ez_utilities_lib
    INTERFACE
        # Please add interface libraries
)

# End of file
