cmake_minimum_required(VERSION 3.25)

project(EzEmbeddedFramework
    VERSION 1.0
    LANGUAGES C CXX
    DESCRIPTION "Easy Embedded Software Framework"
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(easy_embedded)
add_subdirectory(extern)

add_executable(main
    targets/linux/main.c
)

target_include_directories(main PRIVATE ezmsdk)

target_link_libraries(main PRIVATE sdk)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_custom_target(doxygen
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})