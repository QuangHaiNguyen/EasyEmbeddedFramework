cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(FetchContent)

#Adding Unity test framework -------------------------------------------------
if(BUILD_TESTING)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY    https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG           v2.5.2
        #SOURCE_DIR        ${CMAKE_SOURCE_DIR}/extern/unity
    )

    FetchContent_GetProperties(unity)
    if(NOT unity_POPULATED)
        message(STATUS "Unity framework is not available, downloading...")
        
        FetchContent_Populate(unity)
        SET(UNITY_EXTENSION_FIXTURE ON CACHE BOOL "Build Unity with extension features")
        add_subdirectory(
            ${unity_SOURCE_DIR}
            ${unity_BINARY_DIR}
            EXCLUDE_FROM_ALL)
        
        message(STATUS "Download completed")
    endif()
endif()

#Adding littlefs file system -------------------------------------------------
if(ENABLE_LITTLE_FS)
    FetchContent_Declare(
        littlefs
        GIT_REPOSITORY    https://github.com/littlefs-project/littlefs.git
        GIT_TAG           v2.7.0
        #SOURCE_DIR        ${CMAKE_SOURCE_DIR}/extern/littlefs
    )

    FetchContent_GetProperties(littlefs)
    if(NOT littlefs_POPULATED)
        message(STATUS "littlefs is not available, downloading...")
        FetchContent_Populate(littlefs)
        message(STATUS "Download completed")
    endif()

    add_subdirectory(littlefs)
endif()

#Adding threadx rtos ---------------------------------------------------------
if(ENABLE_THREADX)
    FetchContent_Declare(
        threadx
        GIT_REPOSITORY    https://github.com/eclipse-threadx/threadx.git
        GIT_TAG           v6.4.1_rel
        SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/threadx
    )

    FetchContent_GetProperties(threadx)
    if(NOT threadx)
        message(STATUS "threadx is not available, downloading...")
        FetchContent_Populate(threadx)
        message(STATUS "Download completed")
    endif()

    add_subdirectory(threadx)
endif()

#Adding FreeRTOS -------------------------------------------------------------
if(ENABLE_FREERTOS)
    FetchContent_Declare(
        freertos_kernel
        GIT_REPOSITORY    https://github.com/FreeRTOS/FreeRTOS-Kernel.git
        GIT_TAG           V11.0.1
        SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/freertos
    )

    FetchContent_GetProperties(freertos_kernel)
    if(NOT freertos_kernel)
        message(STATUS "freertos is not available, downloading...")
        FetchContent_Populate(freertos_kernel)
        message(STATUS "freertos completed")
    endif()

    add_library(freertos_config INTERFACE)

    target_include_directories(freertos_config SYSTEM
    INTERFACE
        ${CMAKE_SOURCE_DIR}/targets/linux_freertos/
    )

    target_compile_definitions(freertos_config
    INTERFACE
        projCOVERAGE_TEST=0
)

    add_subdirectory(freertos)
endif()

