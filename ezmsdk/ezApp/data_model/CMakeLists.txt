add_library(ez_data_model STATIC)

message(STATUS "Buidling Data Model app")

target_compile_definitions(ez_data_model
PUBLIC 
DATA_MODEL=$<BOOL:${ENABLE_DATA_MODEL}>
)

target_sources(ez_data_model PRIVATE data_model.c)

target_include_directories(ez_data_model PUBLIC . ../../)

target_link_libraries(ez_data_model PUBLIC ez_event_notifier)