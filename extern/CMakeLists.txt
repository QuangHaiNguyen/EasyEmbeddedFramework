cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(FetchContent)    

FetchContent_Declare(
    unity
    GIT_REPOSITORY    https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG           v2.5.2
)

FetchContent_GetProperties(unity)
if(NOT unity_POPULATED)
  message(STATUS "Unity framework is not available, downloading...")
  
  FetchContent_Populate(unity)
  SET(UNITY_EXTENSION_FIXTURE ON CACHE BOOL "Build Unity with extension features")
  add_subdirectory(
    ${unity_SOURCE_DIR}
    ${unity_BINARY_DIR}
    EXCLUDE_FROM_ALL)
  
  message(STATUS "Download completed")
endif()