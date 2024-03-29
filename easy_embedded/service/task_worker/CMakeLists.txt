# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_task_worker_lib
# License: This file is published under the license described in LICENSE.md
# Description: PLEASE ADD TEXT HERE
# ----------------------------------------------------------------------------

add_library(ez_task_worker_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_task_worker_lib library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/easy_embedded)


# Source files ---------------------------------------------------------------
target_sources(ez_task_worker_lib
    PRIVATE
        ez_task_worker.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_task_worker_lib
    PUBLIC
        EZ_TASK_WORKER_ENABLE=$<BOOL:${ENABLE_EZ_TASK_WORKER}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_task_worker_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
        # Please add private folders here
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_task_worker_lib
    PUBLIC
        # Please add public libraries
    PRIVATE
        ez_utilities_lib
    INTERFACE
        # Please add interface libraries
)

# End of file
