# CMake file for building the utilities library
add_library(ez_utilities STATIC)

message(STATUS "Building Utilities library")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

# utilities module
target_sources(ez_utilities PRIVATE
ezmAssert/ezmAssert.c
hexdump/hexdump.c
linked_list/linked_list.c
logging/logging.c
ring_buffer/ring_buffer.c
smalloc/smalloc.c
stcmem/stcmem.c
system_error/system_error.c)

#include directory
target_include_directories(ez_utilities PUBLIC
${CURRENT_DIR}
${SDK_ROOT_DIR})