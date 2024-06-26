# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: data_model_lib
# License: This file is published under the license described in LICENSE.md
# Description: CMake file for data model component
# ----------------------------------------------------------------------------

add_library(ez_data_model_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating data_model library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/ezmsdk)


# Source files ---------------------------------------------------------------
target_sources(ez_data_model_lib
    PRIVATE
        ez_data_model.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_data_model_lib
    PUBLIC
        DATA_MODEL=$<BOOL:${ENABLE_DATA_MODEL}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_data_model_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        # Unused
    INTERFACE
        # Unused
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_data_model_lib
    PUBLIC
        ez_event_notifier_lib
    PRIVATE
        # Unused
    INTERFACE
        # Unused
)

# End of file
