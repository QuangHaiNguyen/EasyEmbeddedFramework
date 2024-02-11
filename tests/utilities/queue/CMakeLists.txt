#build the unit tests
add_library(ez_unit_test_queue)

#include directory
target_include_directories(ez_unit_test_queue PRIVATE
.
../../../ezmsdk)

target_link_libraries(ez_unit_test_queue PRIVATE unity sdk)

target_compile_definitions(ez_unit_test_queue
PRIVATE 
EZ_QUEUE=$<BOOL:${ENABLE_EZ_QUEUE}>
)

# Service module
target_sources(ez_unit_test_queue PRIVATE
Test_ezQueue.c
Test_ezQueue_Runner.c)
