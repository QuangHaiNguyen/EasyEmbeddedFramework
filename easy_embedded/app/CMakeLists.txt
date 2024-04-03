# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_app_lib
# License: This file is published under the license described in LICENSE.md
# Description: PLEASE ADD TEXT HERE
# ----------------------------------------------------------------------------

add_library(ez_app_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_app_lib library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/easy_embedded)


if(ENABLE_DATA_MODEL)
    add_subdirectory(data_model)
endif()

if(ENABLE_EZ_CLI)
    add_subdirectory(cli)
endif()

if(ENABLE_EZ_RPC)
    add_subdirectory(rpc)
endif()

if(ENABLE_EZ_IPC)
    add_subdirectory(ipc)
endif()

# Source files ---------------------------------------------------------------
target_sources(ez_app_lib
    PRIVATE
        ez_app.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_app_lib
    PUBLIC
        DATA_MODEL=$<BOOL:${ENABLE_DATA_MODEL}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_app_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_app_lib
    PUBLIC
        $<$<BOOL:${ENABLE_DATA_MODEL}>:ez_data_model_lib>
        $<$<BOOL:${ENABLE_EZ_CLI}>:ez_cli_lib>
        $<$<BOOL:${ENABLE_EZ_RPC}>:ez_rpc_lib>
        $<$<BOOL:${ENABLE_EZ_IPC}>:ez_ipc_lib>
    PRIVATE
        ez_utilities_lib
    INTERFACE
        # Please add interface libraries
)

# End of file
