/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sps30_basic.c
 * @brief     driver sps30 basic source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-25
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/25  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_sps30_basic.h"

static sps30_handle_t gs_handle;        /**< sps30 handle */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t sps30_basic_init(sps30_interface_t interface)
{
    volatile uint8_t res;
    
    /* link functions */
    DRIVER_SPS30_LINK_INIT(&gs_handle, sps30_handle_t);
    DRIVER_SPS30_LINK_UART_INIT(&gs_handle, sps30_interface_uart_init);
    DRIVER_SPS30_LINK_UART_DEINIT(&gs_handle, sps30_interface_uart_deinit);
    DRIVER_SPS30_LINK_UART_READ(&gs_handle, sps30_interface_uart_read);
    DRIVER_SPS30_LINK_UART_WRITE(&gs_handle, sps30_interface_uart_write);
    DRIVER_SPS30_LINK_UART_FLUSH(&gs_handle, sps30_interface_uart_flush);
    DRIVER_SPS30_LINK_IIC_INIT(&gs_handle, sps30_interface_iic_init);
    DRIVER_SPS30_LINK_IIC_DEINIT(&gs_handle, sps30_interface_iic_deinit);
    DRIVER_SPS30_LINK_IIC_WRITE_COMMAND(&gs_handle, sps30_interface_iic_write_cmd);
    DRIVER_SPS30_LINK_IIC_READ_COMMAND(&gs_handle, sps30_interface_iic_read_cmd);
    DRIVER_SPS30_LINK_DELAY_MS(&gs_handle, sps30_interface_delay_ms);
    DRIVER_SPS30_LINK_DEBUG_PRINT(&gs_handle, sps30_interface_debug_print);
    
    /* set the interface */
    res = sps30_set_interface(&gs_handle, interface);
    if (res)
    {
        sps30_interface_debug_print("sps30: set interface failed.\n");
    
        return 1;
    }
    
    /* init the chip */
    res = sps30_init(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: init failed.\n");
    
        return 1;
    }
    
    /* set auto cleaning interval */
    res = sps30_set_auto_cleaning_interval(&gs_handle, SPS30_BASIC_DEFAULT_AUTO_CLEANING_INTERVAL);
    if (res)
    {
        sps30_interface_debug_print("sps30: set auto cleaning interval failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }

    /* start measurement */
    res = sps30_start_measurement(&gs_handle, SPS30_BASIC_DEFAULT_FORMAT);
    if (res)
    {
        sps30_interface_debug_print("sps30: start measurement failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }

    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sps30_basic_deinit(void)
{
    volatile uint8_t res;
    
    /* stop measurement */
    res = sps30_stop_measurement(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: stop measurement failed.\n");
    
        return 1;
    }
    
    /* deinit */
    res = sps30_deinit(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: deinit failed.\n");
    
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *pm points to a sps30_pm_t structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sps30_basic_read(sps30_pm_t *pm)
{
    volatile uint8_t res;
    
    /* read data */
    res = sps30_read(&gs_handle, pm);
    if (res)
    {
        sps30_interface_debug_print("sps30: read failed.\n");
    
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example start fan cleaning
 * @return status code
 *         - 0 success
 *         - 1 start fan cleaning failed
 * @note   none
 */
uint8_t sps30_basic_start_fan_cleaning(void)
{
    if (sps30_start_fan_cleaning(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example reset
 * @return status code
 *         - 0 success
 *         - 1 reset failed
 * @note   none
 */
uint8_t sps30_basic_reset(void)
{
    if (sps30_reset(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example sleep
 * @return status code
 *         - 0 success
 *         - 1 sleep failed
 * @note   none
 */
uint8_t sps30_basic_sleep(void)
{
    if (sps30_sleep(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t sps30_basic_wake_up(void)
{
    if (sps30_wake_up(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example get the product type
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product type failed
 * @note       none
 */
uint8_t sps30_basic_get_product_type(char type[9])
{
    if (sps30_get_product_type(&gs_handle, type))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example get the serial number
 * @param[out] *sn points to a sn buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t sps30_basic_get_serial_number(char sn[17])
{
    if (sps30_get_serial_number(&gs_handle, sn))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example get the version
 * @param[out] *major points to a major buffer
 * @param[out] *minor points to a minor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 * @note       none
 */
uint8_t sps30_basic_get_version(uint8_t *major, uint8_t *minor)
{
    if (sps30_get_version(&gs_handle, major, minor))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      basic example get the status
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 * @note       none
 */
uint8_t sps30_basic_get_status(uint32_t *status)
{
    if (sps30_get_device_status(&gs_handle, status))
    {
        return 1;
    }
    else
    {
        if (sps30_clear_device_status(&gs_handle))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
