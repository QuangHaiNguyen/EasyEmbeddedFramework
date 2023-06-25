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

target_compile_definitions(ez_unit_test_hal_echo
PRIVATE 
EZ_KERNEL=$<BOOL:${ENABLE_EZ_KERNEL}>
)

# Service module
target_sources(ez_unit_test_hal_echo PRIVATE
    Test_EchoHal.c
    Test_EchoHal_Runner.c
)
