# ----------------------------------------------------------------------------
# Author: Hai Nguyen
# License: This file is published under the license described in LICENSE.md
# Description: CMake file to build unit tests
# ----------------------------------------------------------------------------

message(STATUS "************************************")
message(STATUS "* Building unit tests")
message(STATUS "************************************")


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

# End of file