add_library(ez_generic_driver STATIC)

message(STATUS "    Adding Generic Driver app")

target_compile_definitions(ez_generic_driver
PUBLIC 
EZ_GENERIC_DRIVER=$<BOOL:${ENABLE_EZ_GENERIC_DRIVER}>
)

target_sources(ez_generic_driver PRIVATE
"ez_generic_driver.c"
dummy_driver.c)

target_include_directories(ez_generic_driver PUBLIC . ../../)

target_link_libraries(ez_generic_driver PUBLIC
ez_event_notifier
ez_kernel
ez_utilities)