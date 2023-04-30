cmake_minimum_required(VERSION 3.16)

project(EasyEmbeddedSdk
    VERSION 1.0
    LANGUAGES C CXX
    DESCRIPTION "Easy Embedded Software Development Kit"
)

add_subdirectory(ezmsdk)
add_subdirectory(extern)
#add_subdirectory(tests)

add_executable(main ezmsdk/main.c)
target_include_directories(main PRIVATE ezmsdk)
target_link_libraries(main PRIVATE unity sdk)

#target_link_libraries(main PRIVATE unity sdk unit_test)