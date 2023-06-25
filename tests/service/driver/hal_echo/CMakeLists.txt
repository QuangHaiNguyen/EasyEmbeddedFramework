#build the unit tests
add_library(ez_unit_test_hal_echo)

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}../../../../../)

#include directory
target_include_directories(ez_unit_test_hal_echo PRIVATE
    ${CURRENT_DIR}
    ${SDK_ROOT_DIR}/ezmsdk
)

target_link_libraries(ez_unit_test_hal_echo PRIVATE unity sdk)

# Service module
target_sources(ez_unit_test_hal_echo PRIVATE
    Test_EchoHal.c
    Test_EchoHal_Runner.c
)
