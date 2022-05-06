#include "pch.h"
#include <limits.h>
#include "gtest/gtest.h"


#define TEST_IPC                0U
#define TEST_BIN_PARSER         0U
#define TEST_ASSERT             0U
#define TEST_HEXDUMP            0U
#define TEST_CLI                0U
#define TEST_LINKEDLIST         0U
#define TEST_SMALLOC            0U
#define TEST_RINGBUFFER         0U
#define TEST_STCMEM             0U
#define TEST_KERNEL             0U
#define TEST_PRINTF             0U
#define TEST_SYSTEM_ERROR       0U
#define TEST_FLASH_SIMULATOR    1U

#if (TEST_IPC == 1U)
#include "../unit_test_ipc.h"
#endif /* TEST_IPC */

#if (TEST_BIN_PARSER == 1U)
#include "../unit_test_binParser.h"
#endif /* TEST_BIN_PARSER */

#if (TEST_ASSERT == 1U)
#include "../unit_test_assert.h"
#endif /* TEST_ASSERT */

#if (TEST_HEXDUMP == 1U)
#include "../unit_test_hexdump.h"
#endif /* TEST_HEXDUMP */

#if (TEST_CLI == 1U)
#include "../unit_test_cli.h"
#endif /* TEST_CLI */

#if (TEST_LINKEDLIST == 1U)
#include "../unit_test_linked_list.h"
#endif /* TEST_LINKEDLIST */

#if (TEST_SMALLOC == 1U)
#include "../unit_test_smalloc.h"
#endif /* TEST_SMALLOC */

#if (TEST_RINGBUFFER == 1U)
#include "../unit_test_ring_buffer.h"
#endif /* TEST_RINGBUFFER */

#if (TEST_STCMEM == 1U)
#include "../unit_test_stcmem.h"
#endif /* TEST_STCMEM */

#if (TEST_KERNEL == 1U)
#include "../unit_test_kernel.h"
#endif /* TEST_KERNEL */

#if (TEST_PRINTF == 1U)
#include "../unit_test_printf.h"
#endif /* TEST_PRINTF */

#if (TEST_SYSTEM_ERROR == 1U)
#include "../unit_test_system_error.h"
#endif /* TEST_SYSTEM_ERROR */

#if (TEST_FLASH_SIMULATOR == 1U)
#include "../unit_test_flash_simulator.h"
#endif /* TEST_FLASH_SIMULATOR */