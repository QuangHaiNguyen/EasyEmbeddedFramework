#build the complete sdk as static library
add_library(sdk STATIC)

include(features.cmake)

add_subdirectory(ezUtilities)
add_subdirectory(service)
add_subdirectory(ezApp)

# binCmdParser module
target_sources(sdk PRIVATE
ezApp/ezSdk.c
ezApp/cli/cli.c
ezApp/ezDriver/ezDriver.c
ezApp/ezDriver/dummy_driver.c
ezApp/ezmIpc/ezmIpc.c
ezApp/ezKernel/ezKernel.c)

# hal module
target_sources(sdk PRIVATE
ezHal/uart/uart.c
ezHal/hal_i2c/hal_i2c.c
ezHal/wifi_controller/wifi_controller.c
ezHal/network/mqtt/mqtt.c)

# platforms module
# PC target
target_sources(sdk PRIVATE
ezTargets/simulator/flash/flash_simulator.c
ezTargets/simulator/uart/uart_sim.c
ezTargets/simulator/wifi_controller/wifi_controller_sim.c)

if(ESP_PLATFORM)
  # On ESP-IDF, bar.c needs to include esp_spi_flash.h from the spi_flash component
  target_link_libraries(sdk PRIVATE idf::mqtt)
endif()

target_link_libraries(sdk PRIVATE
ez_utilities
ez_event_notifier
ez_app)

#include directory
target_include_directories(sdk PUBLIC .)
