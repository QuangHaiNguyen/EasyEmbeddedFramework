#build the unit tests
add_library(test_event_notifier STATIC)

# Service module
target_sources(test_event_notifier PRIVATE
Test_ezEventNotifier.c
Test_ezEventNotifier_Runner.c)

#include directory
target_include_directories(test_event_notifier PUBLIC .)

target_link_libraries(unit_test unity test_event_notifier)