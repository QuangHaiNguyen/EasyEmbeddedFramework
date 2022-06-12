#build the complete sdk as static library
add_library(sdk STATIC)

# utilities module
target_sources(sdk PRIVATE
utilities/ezmAssert/ezmAssert.c
utilities/hexdump/hexdump.c
utilities/linked_list/linked_list.c
utilities/logging/logging.c
utilities/ring_buffer/ring_buffer.c
utilities/smalloc/smalloc.c
utilities/stcmem/stcmem.c
utilities/system_error/system_error.c
utilities/event_notifier/event_notifier.c)

# app module
target_sources(sdk PRIVATE
app/app.c)

# binCmdParser module
target_sources(sdk PRIVATE
binCmdParser/binCmdParser.c)

# cli module
target_sources(sdk PRIVATE
cli/cli.c)

# ezmDebug module
target_sources(sdk PRIVATE
ezmDebug/ezmDebug.c)

# ezmDriver module
target_sources(sdk PRIVATE
ezmDriver/driver.c
ezmDriver/dummy_driver.c)

# ezmIpc module
target_sources(sdk PRIVATE
ezmIpc/ezmIpc.c)

# ezmKernel module
target_sources(sdk PRIVATE
ezmKernel/ezmKernel.c)

# scheduler module
target_sources(sdk PRIVATE
scheduler/scheduler.c)

# statemachine module
target_sources(sdk PRIVATE
statemachine/statemachine.c)

# hal module
target_sources(sdk PRIVATE
hal/uart/uart.c
hal/wifi_controller/wifi_controller.c)

# platforms module
# PC target
target_sources(sdk PRIVATE
platforms/simulator/flash/flash_simulator.c
platforms/simulator/uart/uart_sim.c
platforms/simulator/wifi_controller/wifi_controller_sim.c)

#esp32 target
target_sources(sdk PRIVATE
platforms/esp32/uart/esp32_uart.c)

#include directory
target_include_directories(sdk PUBLIC .)
