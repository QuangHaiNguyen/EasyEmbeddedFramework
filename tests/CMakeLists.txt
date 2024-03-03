#build the unit tests
add_executable(ez_unit_test)

message(STATUS "************************************")
message(STATUS "* Building unit tests")
message(STATUS "************************************")

#include directory
target_include_directories(ez_unit_test PRIVATE
    .
    ../ezmsdk
)

target_link_libraries(ez_unit_test PUBLIC unity easy_embedded_lib)

# activate test depend on the activated module
if(ENABLE_EZ_EVENT_NOTIFIER)
    message(STATUS "Test for event notifier is activated")
    add_subdirectory(service/event_notifier)
endif()

if(ENABLE_EZ_KERNEL)
    message(STATUS "Test for kernel is activated")
    add_subdirectory(service/kernel)
endif()

if(ENABLE_EZ_QUEUE)
    message(STATUS "Test for queue is activated")
    add_subdirectory(utilities/queue)
endif()

if(ENABLE_EZ_HAL_ECHO)
    message(STATUS "Test for hal echo is activated")
    add_subdirectory(service/driver/hal_echo)
endif()

if(ENABLE_EZ_HAL_UART)
    message(STATUS "Test for hal uart is activated")
    add_subdirectory(service/driver/hal_uart)
endif()

# Service module
target_sources(ez_unit_test PRIVATE
    main.c
 )

target_link_libraries(ez_unit_test PRIVATE
    $<$<BOOL:${ENABLE_EZ_EVENT_NOTIFIER}>:ez_unit_test_event_notifier>
    $<$<BOOL:${ENABLE_EZ_QUEUE}>:ez_unit_test_queue>
    $<$<BOOL:${ENABLE_EZ_KERNEL}>:ez_unit_test_kernel>
    $<$<BOOL:${ENABLE_EZ_HAL_ECHO}>:ez_unit_test_hal_echo>
    $<$<BOOL:${ENABLE_EZ_HAL_UART}>:ez_unit_test_hal_uart>
)

add_test(NAME ez_unit_test
    COMMAND ez_unit_test
)