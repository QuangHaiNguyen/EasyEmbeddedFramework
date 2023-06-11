cmake_minimum_required(VERSION 3.16)

project(EasyEmbeddedSdk
    VERSION 1.0
    LANGUAGES C CXX
    DESCRIPTION "Easy Embedded Software Development Kit"
)

# Only use testing if we're not somebody's subproject!
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(ezmsdk)
add_subdirectory(extern)
#add_subdirectory(tests)

add_executable(main targets/windows/main.c)
target_include_directories(main PRIVATE ezmsdk)
target_link_libraries(main PRIVATE unity sdk)

#target_link_libraries(main PRIVATE unity sdk unit_test)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()