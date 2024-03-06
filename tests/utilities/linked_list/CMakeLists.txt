# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_linked_list_test
# License: This file is published under the license described in LICENSE.md
# Description: PLEASE ADD TEXT HERE
# ----------------------------------------------------------------------------

add_executable(ez_linked_list_test)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_linked_list_test build files")
message(STATUS "**********************************************************")


# Source files ---------------------------------------------------------------
target_sources(ez_linked_list_test
    PRIVATE
        unittest_ez_linked_list.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_linked_list_test
    PUBLIC
        # Please add definitions here
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_linked_list_test
    PUBLIC
        # Please add private folders here
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_linked_list_test
    PUBLIC
        # Please add public libraries
    PRIVATE
        unity
        easy_embedded_lib
    INTERFACE
        # Please add interface libraries
)

add_test(NAME ez_linked_list_test
    COMMAND ez_linked_list_test
)

# End of file
