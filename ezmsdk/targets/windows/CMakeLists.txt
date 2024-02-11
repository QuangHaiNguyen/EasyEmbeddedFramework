add_library(ez_windows_target STATIC)

message(STATUS "************************************")
message(STATUS "* Building target module")
message(STATUS "************************************")


set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_SOURCE_DIR}/ezmsdk/)

target_sources(ez_windows_target PRIVATE
    echo_driver/ez_windows_echo.c
    uart_driver/ez_windows_uart.c
)

target_include_directories(ez_windows_target PUBLIC
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}
)

target_compile_definitions(ez_windows_target
    PUBLIC
    WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
    EZ_HAL_ECHO=$<BOOL:${ENABLE_EZ_HAL_ECHO}>
    EZ_HAL_UART=$<BOOL:${ENABLE_EZ_HAL_UART}>
)

target_link_libraries(ez_windows_target PRIVATE
    ez_utilities
)