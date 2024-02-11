# CMake file for building the utilities library
add_library(ez_utilities STATIC)

message(STATUS "Building Utilities library")

set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

# utilities module
target_sources(ez_utilities PRIVATE
assert/ez_assert.c
hexdump/ez_hexdump.c
linked_list/ez_linked_list.c
logging/ez_logging.c
ring_buffer/ez_ring_buffer.c
static_alloc/ez_static_alloc.c
system_error/ez_system_error.c
queue/ez_queue.c)

target_compile_definitions(ez_utilities
PUBLIC
EZ_LOGGING=$<BOOL:${ENABLE_EZ_LOGGING}>
EZ_LOGGING_COLOR=$<BOOL:${ENABLE_EZ_LOGGING_COLOR}>
EZ_LINKEDLIST=$<BOOL:${ENABLE_EZ_LINKEDLIST}>
EZ_HEXDUMP=$<BOOL:${ENABLE_EZ_HEXDUMP}>
EZ_RING_BUFFER=$<BOOL:${ENABLE_EZ_RING_BUFFER}>
EZ_ASSERT=$<BOOL:${ENABLE_EZ_ASSERT}>
EZ_STATIC_ALLOC=$<BOOL:${ENABLE_EZ_STATIC_ALLOC}>
EZ_SYS_ERROR=$<BOOL:${ENABLE_EZ_SYS_ERROR}>
EZ_QUEUE=$<BOOL:${ENABLE_EZ_QUEUE}>
)

#include directory
target_include_directories(ez_utilities PUBLIC
${CMAKE_CURRENT_LIST_DIR}
${SDK_ROOT_DIR})