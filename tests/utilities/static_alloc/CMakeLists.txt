# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_static_alloc_test
# License: This file is published under the license described in LICENSE.md
# Description: Cmake file for static_alloc component
# ----------------------------------------------------------------------------

add_executable(ez_static_alloc_test)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_static_alloc_test build files")
message(STATUS "**********************************************************")


# Source files ---------------------------------------------------------------
target_sources(ez_static_alloc_test
    PRIVATE
        unittest_ez_static_alloc.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_static_alloc_test
    PUBLIC
        # Please add definitions here
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_static_alloc_test
    PUBLIC
        # Please add private folders here
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_static_alloc_test
    PUBLIC
        # Please add public libraries
    PRIVATE
        unity
        easy_embedded_lib
    INTERFACE
        # Please add interface libraries
)

add_test(NAME ez_static_alloc_test
    COMMAND ez_static_alloc_test
)

# End of file
