message(STATUS "************************************")
message(STATUS "* Building target modules")
message(STATUS "************************************")

if(ENABLE_WINDOWS_TARGET)
    message(STATUS "Windows target activated")
    add_subdirectory(windows)
endif()

if(ENABLE_LINUX_TARGET)
    message(STATUS "Linux target activated")
    add_subdirectory(linux)
endif()

if(ENABLE_EMBEDDED_TARGET)
    message(STATUS "Embedded target activated")
endif()

