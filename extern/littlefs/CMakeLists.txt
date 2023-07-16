add_library(littlefs STATIC)

message(STATUS "Adding littlefs")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_SOURCE_DIR}/ezmsdk/)

#shut down all of debug output
add_compile_definitions(LFS_NO_DEBUG=0)
add_compile_definitions(LFS_NO_WARN=0)
add_compile_definitions(LFS_NO_ERROR=0)

target_sources(littlefs PRIVATE
    ${littlefs_SOURCE_DIR}/lfs.c
    ${littlefs_SOURCE_DIR}/lfs_util.c
)

target_include_directories(littlefs PUBLIC
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}
)