add_library(ez_event_notifier STATIC)

message(STATUS "    Adding Event Notifier service")

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDK_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}../../../)

target_sources(ez_event_notifier PRIVATE ez_event_notifier.c)

target_compile_definitions(ez_event_notifier
PUBLIC 
EZ_EVENT_NOTIFIER=$<BOOL:${ENABLE_EZ_EVENT_NOTIFIER}>
)

target_include_directories(ez_event_notifier PUBLIC
${CURRENT_DIR} 
${SDK_ROOT_DIR})

target_link_libraries(ez_event_notifier PRIVATE ez_utilities)