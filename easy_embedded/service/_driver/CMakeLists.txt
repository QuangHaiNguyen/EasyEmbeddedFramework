add_library(ez_driver STATIC)

message(STATUS "    Adding HAL Driver service")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_SOURCE_DIR}/ezmsdk/)
set(HAL_DRV_PATH "hal")

if(ENABLE_EZ_HAL_ECHO)
    message(STATUS "    HAL Echo driver is activated")
endif()

if(ENABLE_EZ_HAL_UART)
    message(STATUS "    HAL Uart driver is activated")
endif()


target_compile_definitions(ez_driver
    PUBLIC
    EZ_HAL_DRIVER=$<BOOL:${ENABLE_EZ_HAL_DRIVER}>
    WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
    EZ_HAL_ECHO=$<BOOL:${ENABLE_EZ_HAL_ECHO}>
    EZ_HAL_UART=$<BOOL:${ENABLE_EZ_HAL_UART}>
)

target_sources(ez_driver PRIVATE
    ez_driver_common.c
    $<$<BOOL:${ENABLE_EZ_HAL_ECHO}>:${HAL_DRV_PATH}/hal_echo/ez_hal_echo.c>
    $<$<BOOL:${ENABLE_EZ_HAL_UART}>:${HAL_DRV_PATH}/hal_uart/ez_hal_uart.c>
)

target_include_directories(ez_driver PUBLIC
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}
    ${HAL_DRV_PATH}
)

target_link_libraries(ez_driver PUBLIC
    ez_utilities
    $<$<BOOL:${ENABLE_WINDOWS_TARGET}>:ez_windows_target>
)