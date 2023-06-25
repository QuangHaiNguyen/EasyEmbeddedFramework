# Configure the target
option(ENABLE_WINDOWS_TARGET    "Enable Windows target"                 ON)
option(ENABLE_LINUX_TARGET      "Enable Linux target"                   OFF)
option(ENABLE_EMBEDDED_TARGET   "Enable Embedded target"                OFF)

# Configure the features

# Configure Utilities modules
option(ENABLE_EZ_LOGGING        "Enable logging feature"                ON)
option(ENABLE_EZ_LOGGING_COLOR  "Enable logging using color feature"    ON)
option(ENABLE_EZ_LINKEDLIST     "Enable linked list feature"            ON)
option(ENABLE_EZ_HEXDUMP        "Enable hexdump feature"                ON)
option(ENABLE_EZ_RING_BUFFER    "Enable ring buffer feature"            ON)
option(ENABLE_EZ_ASSERT         "Enable assert feature"                 ON)
option(ENABLE_EZ_STATIC_ALLOC   "Enable static allocation feature"      ON)
option(ENABLE_EZ_SYS_ERROR      "Enable system error feature"           ON)
option(ENABLE_EZ_QUEUE          "Enable queue feature"                  ON)

# Configure Service modules
option(ENABLE_EZ_EVENT_NOTIFIER    "Enable the Event Notifier module"       ON)
option(ENABLE_EZ_KERNEL            "Enable the Kernel service"              ON)

# Configure application framework
option(ENABLE_DATA_MODEL        "Enable the Event Notifier module"          ON)
option(ENABLE_EZ_HAL_DRIVER     "Enable the Driver module"                  ON)

# Configure target
option(ENABLE_EZ_HAL_ECHO       "Enable eho driver in targer"               ON)