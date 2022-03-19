/*******************************************************************************
* Title                 :   module 
* Filename              :   module.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  module.c
 *  @brief This is the source template for a module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmDebug.h"
#include "../app/app_config.h"

#if(EZM_DEBUG == 1U)

#include <stdio.h>
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NO_SUPPORT                          '?'
#define VA_LIST_INIT(list, first_var)       (list = &first_var)
#define VA_ARGS(list, type)                 (*((type*)((char*)list += sizeof(void*))))
#define COPY_STRING(from,to,to_size)        while (to_size > 1 && *from != '\0'){*(to++) = *(from++);to_size--;}
#define COPY_ONE_BYTE(byte,to,to_size)      if(to_size > 1){*(to++) = byte;to_size--;}
#define NUMBER_BUFFER_SIZE                  32U

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef void* VA_LIST;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static char numbers[] = "0123456789abcdef";
static char num_buff[NUMBER_BUFFER_SIZE] = { 0 };
static char printf_buff[PRINTF_BUFF_SIZE] = { 0 };
static PRINT_STRING PrintString;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static char* convert_number(unsigned int number, int base);
static int StringFormat(char* buff, unsigned int buff_size, char* fmt, VA_LIST args);
static inline void mock_print(char* buff, int size);

int ezm_StringCopy(char* src, char* dst, int size_dst)
{
    int ret = -1;

    if (dst != NULL && src != NULL && size_dst > 0)
    {
        ret = 0;
        while ( ret < size_dst)
        {
            *(dst) = *(src);
            if (*src == '\0')
            {
                break;
            }
            else
            {
                ret++;
                dst++;
                src++;
            }
            
        }
    }
    return ret;
}

int ezm_snprintf(char* buff, int buff_size, char* fmt, ...)
{
    int result = -1;
    int byte_count = 0;
    VA_LIST args;
    VA_LIST_INIT(args, fmt);

    result = StringFormat(buff, buff_size, fmt, args);

    if (result == 0)
    {
        byte_count = ezm_StringLen(buff);
    }
    else if (result == 1)
    {
        byte_count = buff_size;
    }
    else
    {
        byte_count = -1;
    }

    return byte_count;
}

int ezm_printf(char* fmt, ...)
{
    int ret = -1;
    int ptr = 0;
    VA_LIST args;
    VA_LIST_INIT(args, fmt);
    while (ret != 0)
    {
        ret = StringFormat(printf_buff, PRINTF_BUFF_SIZE, fmt + ptr, args);
        if (ret > -1)
        {
            mock_print(printf_buff, PRINTF_BUFF_SIZE);
            ptr = ptr + PRINTF_BUFF_SIZE;
        }
        else
        {
            break;
        }
    }
    return ret;
}

unsigned int ezm_StringLen(char* string)
{
    unsigned int len = 0;
    while (*(string++) != '\0')
    {
        len++;
    }
    return len;
}

static int StringFormat(char* buff, unsigned int buff_size, char* fmt, VA_LIST args)
{
    char* tmp_ptr;
    char* transverse = fmt;
    int value = 0U;
    int ret = 0;
    if (buff != NULL && buff_size > 0 && fmt != NULL)
    {
        while (*transverse != '\0')
        {
            if (buff_size < 1)
            {
                ret = 1;
                break;
            }

            if (*transverse == '%')
            {
                transverse++;
                switch (*transverse)
                {
                case 'd':
                    value = VA_ARGS(args, int);
                    if (value < 0)
                    {
                        COPY_ONE_BYTE('-', buff, buff_size);
                        value = -value;
                    }
                    tmp_ptr = convert_number(value, 10);
                    COPY_STRING(tmp_ptr, buff, buff_size);
                    break;

                case 'x':
                    value = VA_ARGS(args, int);
                    if (value < 0)
                    {
                        value = (unsigned int)value;
                    }

                    COPY_ONE_BYTE('0', buff, buff_size);
                    COPY_ONE_BYTE('x', buff, buff_size);
                    tmp_ptr = convert_number(value, 16);
                    COPY_STRING(tmp_ptr, buff, buff_size);
                    break;

                case 'c':
                    value = VA_ARGS(args, char);
                    COPY_ONE_BYTE((char)value, buff, buff_size);
                    break;

                case 'f':
                    value = (int)VA_ARGS(args, int);
                    COPY_ONE_BYTE(NO_SUPPORT, buff, buff_size);
                    COPY_ONE_BYTE('.', buff, buff_size);
                    COPY_ONE_BYTE(NO_SUPPORT, buff, buff_size);
                    break;

                case 's':
                    tmp_ptr = VA_ARGS(args, char*);
                    COPY_STRING(tmp_ptr, buff, buff_size);
                    break;

                default:
                    break;
                }
            }
            else
            {
                *(buff++) = *transverse;
                buff_size--;
            }
            transverse++;
        }
        *(buff++) = '\0';
        buff_size--;
    }
    else
    {
        ret = -1;
    }
    return ret;
}

static char* convert_number(unsigned int number, int base)
{
    char* ptr;
    ptr = &num_buff[NUMBER_BUFFER_SIZE - 1];
    *ptr = '\0';

    if (base == 10 || base == 16)
    {
        do
        {
            *--ptr = numbers[number % base];
            number = number / base;
        } while (number != 0);
    }
    return ptr;
}

static inline void mock_print(char* buff, int size)
{
    for (int i = 0; i < size && buff[i] != '\0'; i++)
    {
        putchar(buff[i]);
    }
}

#endif
/* End of file*/