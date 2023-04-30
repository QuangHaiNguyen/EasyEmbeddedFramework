cmake_minimum_required(VERSION 3.16)

project(EasyEmbeddedSdk
    VERSION 1.0
    LANGUAGES C CXX
    DESCRIPTION "Easy Embedded Software Development Kit"
)

include(FetchContent)    

message(STATUS "Download Unity test framework")
FetchContent_Declare(
    unity
    GIT_REPOSITORY    https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG           v2.5.2
)

SET(UNITY_EXTENSION_FIXTURE ON CACHE BOOL "Build Unity with extension features")

FetchContent_MakeAvailable(unity)

add_subdirectory(ezmsdk)
add_subdirectory(tests)

add_executable(main ezmsdk/main.c)
target_include_directories(main PRIVATE ezmsdk)
target_link_libraries(main PRIVATE unity sdk unit_test)