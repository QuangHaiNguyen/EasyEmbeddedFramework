# Now simply link against gtest or gtest_main as needed. Eg
add_executable(unit_test unit_test.cpp)

#add dependency if nedded
add_dependencies(unit_test smalloc)

target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with smalloc
target_link_libraries(unit_test smalloc gtest_main gtest )

add_test(Unit_test unit_test)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
