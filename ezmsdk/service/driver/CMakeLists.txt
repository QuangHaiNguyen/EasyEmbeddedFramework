add_library(ez_generic_driver STATIC)

message(STATUS "    Adding Generic Driver service")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}../../../)

target_compile_definitions(ez_generic_driver
PUBLIC 
EZ_GENERIC_DRIVER=$<BOOL:${ENABLE_EZ_GENERIC_DRIVER}>
WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
EZ_TARGET_ECHO=$<BOOL:${ENABLE_EZ_TARGET_ECHO}>
)

target_sources(ez_generic_driver PRIVATE
    
    hal/ez_hal_echo.c
)

target_include_directories(ez_generic_driver PUBLIC
    ${CURRENT_DIR}
    ${CURRENT_DIR}/hal
    ${SDK_ROOT_DIR}
)

target_link_libraries(ez_generic_driver PUBLIC
ez_kernel
ez_utilities
$<$<BOOL:${ENABLE_WINDOWS_TARGET}>:ez_windows_driver>
)