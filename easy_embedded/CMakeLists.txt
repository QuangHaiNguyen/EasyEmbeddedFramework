#build the complete sdk as static library
add_library(sdk STATIC)

include(features.cmake)

add_subdirectory(targets)
add_subdirectory(utilities)
add_subdirectory(service)
add_subdirectory(app)

target_sources(sdk PRIVATE
    app/ezSdk.c
)

target_compile_definitions(sdk
    PRIVATE 
    WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
    LINUX_TARGET=$<BOOL:${ENABLE_LINUX_TARGET}>
    EMBEDDED_TARGET=$<BOOL:${ENABLE_EMBEDDED_TARGET}>
)

target_link_libraries(sdk PUBLIC
    ez_utilities_lib
    $<$<BOOL:${ENABLE_EZ_EVENT_NOTIFIER}>:ez_event_notifier_lib>
    $<$<BOOL:${ENABLE_EZ_KERNEL}>:ez_kernel>
    $<$<BOOL:${ENABLE_EZ_HAL_DRIVER}>:ez_driver>
    $<$<BOOL:${ENABLE_DATA_MODEL}>:ez_data_model_lib>
    $<$<BOOL:${ENABLE_LITTLE_FS}>:littlefs>
    ez_app
)

#include directory
target_include_directories(sdk PUBLIC .)
