add_library(ez_hal_driver STATIC)

message(STATUS "    Adding HAL Driver service")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_SOURCE_DIR}/ezmsdk/)

if(ENABLE_EZ_HAL_ECHO)
    message(STATUS "    HAL Echo driver is activated")
    if(ENABLE_WINDOWS_TARGET)
        message(STATUS "    + Link to Windows Echo driver")
        target_link_libraries(ez_hal_driver PUBLIC
            $<$<BOOL:${ENABLE_WINDOWS_TARGET}>:ez_windows_driver>
        )
    endif()
endif()


target_compile_definitions(ez_hal_driver
    PUBLIC 
    EZ_HAL_DRIVER=$<BOOL:${ENABLE_EZ_HAL_DRIVER}>
    WINDOWS_TARGET=$<BOOL:${ENABLE_WINDOWS_TARGET}>
    EZ_TARGET_ECHO=$<BOOL:${ENABLE_EZ_TARGET_ECHO}>
)

target_sources(ez_hal_driver PRIVATE
    $<$<BOOL:${ENABLE_EZ_HAL_ECHO}>:hal_echo/ez_hal_echo.c>
)

target_include_directories(ez_hal_driver PUBLIC
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}
    $<$<BOOL:${ENABLE_EZ_HAL_ECHO}>:${CURRENT_DIR}/hal>
)

target_link_libraries(ez_hal_driver PUBLIC
ez_utilities
)