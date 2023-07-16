#build the complete sdk as static library
add_library(sdk STATIC)

include(features.cmake)

add_subdirectory(targets)
add_subdirectory(utilities)
add_subdirectory(service)
add_subdirectory(ezApp)

target_sources(sdk PRIVATE
    ezApp/ezSdk.c
)

target_compile_definitions(sdk
    PRIVATE 
    WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
    LINUX_TARGET=$<BOOL:${ENABLE_LINUX_TARGET}>
    EMBEDDED_TARGET=$<BOOL:${ENABLE_EMBEDDED_TARGET}>
)

target_link_libraries(sdk PUBLIC
    ez_utilities
    ez_event_notifier
    ez_app
    ez_kernel
    ez_hal_driver
    littlefs
)

#include directory
target_include_directories(sdk PUBLIC .)
