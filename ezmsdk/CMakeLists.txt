#build the complete sdk as static library
add_library(sdk STATIC)

# utilities module
target_sources(sdk PRIVATE
ezUtilities/ezmAssert/ezmAssert.c
ezUtilities/hexdump/hexdump.c
ezUtilities/linked_list/linked_list.c
ezUtilities/logging/logging.c
ezUtilities/ring_buffer/ring_buffer.c
ezUtilities/smalloc/smalloc.c
ezUtilities/stcmem/stcmem.c
ezUtilities/system_error/system_error.c)

# Service module
target_sources(sdk PRIVATE
ezService/ezEventNotifier/ezEventNotifier.c)

# binCmdParser module
target_sources(sdk PRIVATE
ezApp/ezSdk.c
ezApp/cli/cli.c
ezApp/ezDriver/ezDriver.c
ezApp/ezDriver/dummy_driver.c
ezApp/ezmIpc/ezmIpc.c
ezApp/ezKernel/ezKernel.c
ezApp/data_model/data_model.c)

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

#include directory
target_include_directories(sdk PUBLIC .)
