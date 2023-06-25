add_library(ez_windows_driver STATIC)

message(STATUS "    Adding drivers for windows")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}../../../)

target_compile_definitions(ez_windows_driver
PUBLIC 
EZ_TARGET_ECHO=$<BOOL:${ENABLE_EZ_TARGET_ECHO}>
)

target_sources(ez_windows_driver PRIVATE
    echo_driver/ez_windows_echo.c
)

target_include_directories(ez_windows_driver PUBLIC
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}
)

target_link_libraries(ez_windows_driver PRIVATE
    ez_utilities
)