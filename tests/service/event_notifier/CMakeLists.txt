#build the unit tests
add_library(ez_unit_test_event_notifier)

#include directory
target_include_directories(ez_unit_test_event_notifier PRIVATE
.
../../../ezmsdk)

target_link_libraries(ez_unit_test_event_notifier PRIVATE unity easy_embedded_lib)

target_compile_definitions(ez_unit_test_event_notifier
PRIVATE 
EZ_EVENT_NOTIFIER=$<BOOL:${ENABLE_EZ_EVENT_NOTIFIER}>
)


# Service module
target_sources(ez_unit_test_event_notifier PRIVATE
Test_ezEventNotifier.c
Test_ezEventNotifier_Runner.c)
