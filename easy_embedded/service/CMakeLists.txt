message(STATUS "************************************")
message(STATUS "* Building Service framework")
message(STATUS "************************************")

if(ENABLE_EZ_HAL_DRIVER)
    message(STATUS "HAL Driver service is activated")
    add_subdirectory(driver)
endif()

if(ENABLE_EZ_EVENT_NOTIFIER)
    add_subdirectory(event_notifier)
endif()

if(ENABLE_EZ_KERNEL)
    add_subdirectory(kernel)
endif()

if(ENABLE_EZ_TASK_WORKER)
    add_subdirectory(task_worker)
endif()

