# Now simply link against gtest or gtest_main as needed. Eg
add_executable(unit_test_app unit_test.cpp ../app.c)

#add dependency if nedded
add_dependencies(unit_test_app app)

target_include_directories(unit_test_app PRIVATE ${CMAKE_SOURCE_DIR})

#link the main with app
target_link_libraries(unit_test_app app gtest_main gtest )

add_test(unit_test_app unit_test_app)

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
