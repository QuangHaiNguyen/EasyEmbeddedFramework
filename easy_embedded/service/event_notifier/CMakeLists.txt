# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_event_notifier_lib
# License: This file is published under the license described in LICENSE.md
# Description: CMake file for event notifier component
# ----------------------------------------------------------------------------

add_library(ez_event_notifier_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_event_notifier library build files")
message(STATUS "**********************************************************")


# Source files ---------------------------------------------------------------
target_sources(ez_event_notifier_lib
    PRIVATE
        ez_event_notifier.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_event_notifier_lib
    PUBLIC
        EZ_EVENT_NOTIFIER=$<BOOL:${ENABLE_EZ_EVENT_NOTIFIER}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_event_notifier_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        # Unused
    INTERFACE
        # Unused
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_event_notifier_lib
    PUBLIC
        ez_utilities_lib
    PRIVATE
        # Unused
    INTERFACE
        # Unused
)


# End of file
