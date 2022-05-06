#include <limits.h>
#include "gtest/gtest.h"
#include "stdio.h"
#ifndef UNIT_TEST_FLASH_SIMULATOR
#define UNIT_TEST_FLASH_SIMULATOR

extern "C" {
#include "app/app_config.h"
#include "hw_simulator/flash/flash_simulator.h"
#include "helper/hexdump/hexdump.h"
#include "ezmDebug/ezmDebug.h"

#define DEFAULT_FLASH   "default_flash"
#define PAGE_SIZE       256U

    void CreatePattern1(uint8_t* buffer, uint16_t size)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            if (i % 2)
            {
                buffer[i] = 0xfe;
            }
            else
            {
                buffer[i] = 0xca;
            }
        }
    }

    void CreatePattern2(uint8_t* buffer, uint16_t size)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            if (i % 2)
            {
                buffer[i] = 0xef;
            }
            else
            {
                buffer[i] = 0xbe;
            }
        }
    }

    void WriteTestPatternToFlash(void)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0 };

        test_flag_bool = FlashSim_Initialization();
        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE)
        {
            if (i % 2)
            {
                CreatePattern1(pattern, PAGE_SIZE);
            }
            else
            {
                CreatePattern2(pattern, PAGE_SIZE);
            }

            (void)FlashSim_WritePage(test_handle,
                                        i,
                                        pattern,
                                        PAGE_SIZE);
        }
    }
}


namespace
{
#if (FLASH_SIM == 1U) && (SUPPORTED_CHIP == WIN)
    
    TEST(flash_simulator, WarningMessge)
    {
        PRINTINFO("PLS REMEMBER TO DELETE THE BINARY FILE BEFORE RUNNING THE TEST");
    }

    TEST(flash_simulator, Initialization)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);
    }

    TEST(flash_simulator, WriteReadPage)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = {0};
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        for (uint32_t i = 0; 
                i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
                i = i + PAGE_SIZE)
        {
            if (i % 2)
            {
                CreatePattern1(pattern, PAGE_SIZE);
            }
            else
            {
                CreatePattern2(pattern, PAGE_SIZE);
            }

            num_of_byte = FlashSim_WritePage(test_handle,
                                                i,
                                                pattern,
                                                PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);

            num_of_byte = FlashSim_Read(test_handle,
                                        i,
                                        read_buffer,
                                        PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);
            compare = memcmp(read_buffer, pattern, PAGE_SIZE);
            ASSERT_EQ(compare, 0);
        }
    }

    TEST(flash_simulator, ErasePage)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0xff };
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        memset(pattern, 0xFF, PAGE_SIZE);

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE)
        {
            test_flag_bool = FlashSim_ErasePage(test_handle, i);
            ASSERT_EQ(test_flag_bool, true);

            num_of_byte = FlashSim_Read(test_handle,
                i,
                read_buffer,
                PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);
            compare = memcmp(read_buffer, pattern, PAGE_SIZE);
            ASSERT_EQ(compare, 0);
        }
    }

    TEST(flash_simulator, EraseSector)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0xff };
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        WriteTestPatternToFlash();

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        memset(pattern, 0xFF, PAGE_SIZE);

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE * 4U)
        {
            test_flag_bool = FlashSim_EraseSector(test_handle, i);
            ASSERT_EQ(test_flag_bool, true);
        }

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE)
        {
            num_of_byte = FlashSim_Read(test_handle,
                                            i,
                                            read_buffer,
                                            PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);
            compare = memcmp(read_buffer, pattern, PAGE_SIZE);
            ASSERT_EQ(compare, 0);
        }
    }

    TEST(flash_simulator, EraseBlock)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0xff };
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        WriteTestPatternToFlash();

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        memset(pattern, 0xFF, PAGE_SIZE);

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE * 4U *4U)
        {
            test_flag_bool = FlashSim_EraseBlock(test_handle, i);
            ASSERT_EQ(test_flag_bool, true);
        }

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE)
        {
            num_of_byte = FlashSim_Read(test_handle,
                i,
                read_buffer,
                PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);
            compare = memcmp(read_buffer, pattern, PAGE_SIZE);
            ASSERT_EQ(compare, 0);
        }
    }

    TEST(flash_simulator, EraseChip)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0xff };
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        WriteTestPatternToFlash();

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        memset(pattern, 0xFF, PAGE_SIZE);

        test_flag_bool = FlashSim_EraseChip(test_handle);
        ASSERT_EQ(test_flag_bool, true);

        for (uint32_t i = 0;
            i < 0x00000000 + FlashSim_GetFlashSizeInBytes(test_handle);
            i = i + PAGE_SIZE)
        {
            num_of_byte = FlashSim_Read(test_handle,
                i,
                read_buffer,
                PAGE_SIZE);

            ASSERT_EQ(num_of_byte, PAGE_SIZE);
            compare = memcmp(read_buffer, pattern, PAGE_SIZE);
            ASSERT_EQ(compare, 0);
        }
    }

    TEST(flash_simulator, IllegalAddressWrite)
    {
        bool test_flag_bool = false;
        FlashHandle test_handle;
        uint8_t pattern[PAGE_SIZE] = { 0x00 };
        uint32_t num_of_byte = 0U;
        uint8_t read_buffer[PAGE_SIZE] = { 0 };
        int compare = 0U;

        test_flag_bool = FlashSim_Initialization();
        ASSERT_EQ(test_flag_bool, true);

        test_handle = FlashSim_GetFlash(DEFAULT_FLASH);
        ASSERT_NE(test_handle, INVALID_HANDLE);
        ASSERT_LT(test_handle, NUM_OF_FLASH);

        CreatePattern1(pattern, PAGE_SIZE);

        num_of_byte = FlashSim_WritePage(test_handle, 0x00000005,
                                            pattern, PAGE_SIZE);

        ASSERT_EQ(num_of_byte, 0);


        num_of_byte = FlashSim_WritePage(test_handle, 0x00001234,
                                        pattern, PAGE_SIZE);

        ASSERT_EQ(num_of_byte, 0);

        num_of_byte = FlashSim_WritePage(test_handle, 0xFFFFFFFF,
                                        pattern, PAGE_SIZE);

        ASSERT_EQ(num_of_byte, 0);

    }
#endif /* FLASH_SIM */
#endif /* UNIT_TEST_FLASH_SIMULATOR */
}