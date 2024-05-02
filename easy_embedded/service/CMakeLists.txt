message(STATUS "************************************")
message(STATUS "* Building Service framework")
message(STATUS "************************************")

if(ENABLE_EZ_HAL_DRIVER)
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

if(ENABLE_EZ_STATE_MACHINE)
    add_subdirectory(state_machine)
endif()

