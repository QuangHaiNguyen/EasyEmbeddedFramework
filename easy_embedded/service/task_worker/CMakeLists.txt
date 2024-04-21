# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# Name: ez_task_worker_lib
# License: This file is published under the license described in LICENSE.md
# Description: CMake file for task worker component
# ----------------------------------------------------------------------------

add_library(ez_task_worker_lib STATIC)

message(STATUS "**********************************************************")
message(STATUS "* Generating ez_task_worker_lib library build files")
message(STATUS "**********************************************************")


set(FRAMEWORK_ROOT_DIR ${CMAKE_SOURCE_DIR}/easy_embedded)


# Source files ---------------------------------------------------------------
target_sources(ez_task_worker_lib
    PRIVATE
        $<$<BOOL:${ENABLE_THREADX}>:threadx_port/ez_threadx_port.c>
        $<$<BOOL:${ENABLE_FREERTOS}>:freertos_port/ez_freertos_port.c>
        ez_task_worker.c
)


# Definitions ----------------------------------------------------------------
target_compile_definitions(ez_task_worker_lib
    PUBLIC
        EZ_TASK_WORKER_ENABLE=$<BOOL:${ENABLE_EZ_TASK_WORKER}>
        EZ_THREADX_PORT_ENABLE=$<BOOL:${ENABLE_THREADX}>
        EZ_FREERTOS_PORT_ENABLE=$<BOOL:${ENABLE_FREERTOS}>
)


# Include directory -----------------------------------------------------------
target_include_directories(ez_task_worker_lib
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        $<$<BOOL:${ENABLE_THREADX}>:${CMAKE_CURRENT_LIST_DIR}/threadx_port>
        $<$<BOOL:${ENABLE_FREERTOS}>:${CMAKE_CURRENT_LIST_DIR}/freertos_port>
    PRIVATE
        #
    INTERFACE
        # Please add interface folders here
)


# Link libraries -------------------------------------------------------------
target_link_libraries(ez_task_worker_lib
    PUBLIC
        # Please add public libraries
    PRIVATE
        ez_utilities_lib
        $<$<BOOL:${ENABLE_THREADX}>:threadx>
        $<$<BOOL:${ENABLE_FREERTOS}>:freertos_kernel>
    INTERFACE
        # Please add interface libraries
)

# End of file
