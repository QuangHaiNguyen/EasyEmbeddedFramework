cmake_minimum_required(VERSION 3.16)

project(EasyEmbeddedSdk
    VERSION 1.0
    LANGUAGES C CXX
    DESCRIPTION "Easy Embedded Software Develpoment Kit")

add_subdirectory(ezmsdk)

add_executable(main ezmsdk/app/main.c)
target_include_directories(main PRIVATE ezmsdk)
target_link_libraries(main PRIVATE sdk)


    