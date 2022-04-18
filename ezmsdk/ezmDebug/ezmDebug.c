/*******************************************************************************
* Filename:         temp.c _ TBD
* Author:           Hai Nguyen
* Original Date:    18.04.2022 - 12:29:44
* Last Update:      18.04.2022 - 12:29:44
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright (C) 2022 by Easy Embedded
*
*******************************************************************************/

/** @file   temp.c - TBD
 *  @author Hai Nguyen
 *  @date   18.04.2022 - 12:29:44
 *  @brief  This is the source for a module
 *
 *  @details
 *
 */
#include "ezmDebug.h"
#include "app/app_config.h"

#if(EZM_DEBUG == 1U)

#include <stdio.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NO_SUPPORT                          '?'
#define VA_LIST_INIT(list, first_var)       (list = &first_var)
#define COPY_ONE_BYTE(byte,to,to_size)      if(to_size > 1){*(to++) = byte;to_size--;}
#define COPY_STRING(from,to,to_size)        while (to_size > 1 && *(from) != '\0'){*((to)++) = *((from)++);to_size--;}
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

/******************************************************************************
* Function Definitions
*******************************************************************************/
static inline void GetNextArgPtr(VA_LIST *arg);
static inline void CopyString(char* from, char* to, int to_size_bytes);
static char* convert_number(unsigned int number, int base);
static int FormatString(char* buff, unsigned int buff_size, char* fmt, VA_LIST args);
static inline void mock_print(char* buff, int size);

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
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

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
int ezm_snprintf(char* buff, int buff_size, char* fmt, ...)
{
    int result = -1;
    int byte_count = 0;
    VA_LIST args;
    VA_LIST_INIT(args, fmt);

    result = FormatString(buff, buff_size, fmt, args);

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

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
int ezm_printf(char* fmt, ...)
{
    int ret = -1;
    int ptr = 0;
    VA_LIST args;
    VA_LIST_INIT(args, fmt);
    while (ret != 0)
    {
        ret = FormatString(printf_buff, PRINTF_BUFF_SIZE, fmt + ptr, args);
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

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
unsigned int ezm_StringLen(char* string)
{
    unsigned int len = 0;
    while (*(string++) != '\0')
    {
        len++;
    }
    return len;
}

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static int FormatString(char* buff, unsigned int buff_size, char* fmt, VA_LIST args)
{
    char    *number;
    char    **string;
    char    *transverse = fmt;
    int     value = 0U;

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
                    GetNextArgPtr(&args);
                    value = *(int*)args;
                    if (value < 0)
                    {
                        COPY_ONE_BYTE('-', buff, buff_size);
                        value = -value;
                    }
                    number = convert_number(value, 10);
                    COPY_STRING(number, buff, buff_size);
                    break;

                case 'x':
                    GetNextArgPtr(&args);
                    value = *(int*)args;
                    if (value < 0)
                    {
                        value = (unsigned int)value;
                    }

                    COPY_ONE_BYTE('0', buff, buff_size);
                    COPY_ONE_BYTE('x', buff, buff_size);
                    number = convert_number(value, 16);
                    COPY_STRING(number, buff, buff_size);
                    break;

                case 'c':
                    GetNextArgPtr(&args);
                    value = *(char*)args;
                    COPY_ONE_BYTE((char)value, buff, buff_size);
                    break;

                case 'f':
                    GetNextArgPtr(&args);
                    value = *(int*)args;
                    COPY_ONE_BYTE(NO_SUPPORT, buff, buff_size);
                    COPY_ONE_BYTE('.', buff, buff_size);
                    COPY_ONE_BYTE(NO_SUPPORT, buff, buff_size);
                    break;

                case 's':
                {
                    GetNextArgPtr(&args);
                    string = args;
                    COPY_STRING(*string, buff, buff_size);
                    break;
                }
                    

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

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
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

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static inline void mock_print(char* buff, int size)
{
    for (int i = 0; i < size && buff[i] != '\0'; i++)
    {
        putchar(buff[i]);
    }
}

/******************************************************************************
* Function : sum
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static inline void GetNextArgPtr(VA_LIST *arg)
{
    *arg = (char*)(*arg) + sizeof(void*);
}

/******************************************************************************
* Function : sum
*//** 
* @Description:
*
* This function initializes the ring buffer
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static inline void CopyString(char* from, char* to, int to_size_bytes)
{
    while (to_size_bytes > 0 && *from != '\0')
    { 
        *(to++) = *(from++); 
        to_size_bytes--;
    }
}
#endif
/* End of file*/