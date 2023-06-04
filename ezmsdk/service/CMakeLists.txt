message(STATUS "************************************")
message(STATUS "* Building Service framework")
message(STATUS "************************************")

if(ENABLE_EZ_EVENT_NOTIFIER)
    message(STATUS "Event notifier module is enable")
    add_subdirectory(event_notifier)
endif()

if(ENABLE_EZ_KERNEL)
    message(STATUS "Kernel service is activated")
    add_subdirectory(kernel)
endif()
