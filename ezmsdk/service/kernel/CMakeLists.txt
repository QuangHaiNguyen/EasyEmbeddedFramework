add_library(ez_kernel STATIC)

message(STATUS "    Adding Kernel service")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}../../../)

target_compile_definitions(ez_kernel
PUBLIC 
EZ_KERNEL=$<BOOL:${ENABLE_EZ_KERNEL}>
)

target_sources(ez_kernel PRIVATE "ez_kernel.c")

target_include_directories(ez_kernel PUBLIC
${CURRENT_DIR} 
${SDK_ROOT_DIR}
)

target_link_libraries(ez_kernel PRIVATE
ez_event_notifier
ez_utilities
)
