add_library(ez_app STATIC)

set(SDK_PATH "..")
set(APP_FRAMEWORK_PATH ".")

message(STATUS "************************************")
message(STATUS "* Building application framework")
message(STATUS "************************************")

if(ENABLE_DATA_MODEL)
message("Data model application is activated")
add_subdirectory(data_model)
endif()

if(ENABLE_EZ_GENERIC_DRIVER)
message("Generic Driver application is activated")
add_subdirectory(generic_driver)
endif()

target_compile_definitions(ez_app
PUBLIC 
DATA_MODEL=$<BOOL:${ENABLE_DATA_MODEL}>
EZ_GENERIC_DRIVER=$<BOOL:${ENABLE_EZ_GENERIC_DRIVER}>
)

target_sources(ez_app PRIVATE ezSdk.c)

target_include_directories(ez_app PUBLIC
${SDK_PATH}
${APP_FRAMEWORK_PATH})

target_link_libraries(ez_app PUBLIC
$<$<BOOL:${ENABLE_DATA_MODEL}>:ez_data_model>
$<$<BOOL:${ENABLE_EZ_GENERIC_DRIVER}>:ez_generic_driver>
)
