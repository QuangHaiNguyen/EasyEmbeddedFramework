# Now simply link against gtest or gtest_main as needed. Eg
add_executable(unit_test_assert unit_test.cpp ../ezmAssert.c)

#add dependency if nedded
add_dependencies(unit_test_assert ezmAssert)

target_include_directories(unit_test_assert PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with ezmAssert
target_link_libraries(unit_test_assert ezmAssert gtest_main gtest )

add_test(unit_test_assert unit_test_assert)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
