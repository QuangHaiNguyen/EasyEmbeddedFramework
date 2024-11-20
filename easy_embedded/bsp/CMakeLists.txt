# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_bsp_lib
# License: This file is published under the license described in LICENSE.md
# Description: PLEASE ADD TEXT HERE
# ----------------------------------------------------------------------------

add_library(ez_bsp_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_bsp_lib library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/easy_embedded)


# Source files ---------------------------------------------------------------
target_sources(ez_bsp_lib
    PRIVATE
        ez_bsp.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_bsp_lib
    PUBLIC
        EZ_BSP_ENABLE=$<BOOL:${ENABLE_EZ_BSP}>
    PRIVATE
        EZ_BUILD_WITH_CMAKE=$<BOOL:${BUILD_WITH_CMAKE}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_bsp_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_bsp_lib
    PUBLIC
        # Please add public libraries
    PRIVATE
        ez_utilities_lib
    INTERFACE
        # Please add interface libraries
)

# End of file
