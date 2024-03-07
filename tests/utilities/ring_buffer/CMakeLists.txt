# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_ring_buffer_test
# License: This file is published under the license described in LICENSE.md
# Description: CMake file for ring buffer unit test
# ----------------------------------------------------------------------------

add_executable(ez_ring_buffer_test)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_ring_buffer_test build files")
message(STATUS "**********************************************************")


# Source files ---------------------------------------------------------------
target_sources(ez_ring_buffer_test
    PRIVATE
        unittest_ez_ring_buffer.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_ring_buffer_test
    PUBLIC
        # Please add definitions here
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_ring_buffer_test
    PUBLIC
        # Please add private folders here
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_ring_buffer_test
    PUBLIC
        # Please add public libraries
    PRIVATE
        unity
        easy_embedded_lib
    INTERFACE
        # Please add interface libraries
)

add_test(NAME ez_ring_buffer_test
    COMMAND ez_ring_buffer_test
)

# End of file
