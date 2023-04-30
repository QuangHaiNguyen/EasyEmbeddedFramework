#build the unit tests
add_library(unit_test STATIC)

# Service module
target_sources(unit_test PRIVATE
service/event_notifier/Test_ezEventNotifier.c
service/event_notifier/Test_ezEventNotifier_Runner.c)

#include directory
target_include_directories(unit_test PUBLIC
.
../ezmsdk)

target_link_libraries(unit_test unity sdk)