add_library(ez_app STATIC)

set(SDK_PATH "..")
set(APP_FRAMEWORK_PATH ".")

message(STATUS "Building application framework")

if(ENABLE_DATA_MODEL)
add_subdirectory(data_model)
endif()

target_compile_definitions(ez_app
PUBLIC 
DATA_MODEL=$<BOOL:${ENABLE_DATA_MODEL}>
)

target_sources(ez_app PRIVATE ezSdk.c)

target_include_directories(ez_app PUBLIC
${SDK_PATH}
${APP_FRAMEWORK_PATH})

target_link_libraries(ez_app PRIVATE
ez_utilities
$<$<BOOL:${ENABLE_DATA_MODEL}>:ez_data_model>)
