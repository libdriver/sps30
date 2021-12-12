/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
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
 * @file      driver_sps30_register_test.c
 * @brief     driver sps30 register test source file
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

#include "driver_sps30_register_test.h"
#include <stdlib.h>

static sps30_handle_t gs_handle;        /**< sps30 handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sps30_register_test(sps30_interface_t interface)
{
    volatile uint8_t res;
    volatile uint32_t second, second_check;
    volatile char type[9];
    volatile char sn[17];
    volatile uint8_t major;
    volatile uint8_t minor;
    volatile uint32_t status;
    sps30_info_t info;
    sps30_interface_t interface_check;
    sps30_data_ready_flag_t flag;
    
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
    
    /* get information */
    res = sps30_info(&info);
    if (res)
    {
        sps30_interface_debug_print("sps30: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        sps30_interface_debug_print("sps30: chip is %s.\n", info.chip_name);
        sps30_interface_debug_print("sps30: manufacturer is %s.\n", info.manufacturer_name);
        sps30_interface_debug_print("sps30: interface is %s.\n", info.interface);
        sps30_interface_debug_print("sps30: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sps30_interface_debug_print("sps30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sps30_interface_debug_print("sps30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sps30_interface_debug_print("sps30: max current is %0.2fmA.\n", info.max_current_ma);
        sps30_interface_debug_print("sps30: max temperature is %0.1fC.\n", info.temperature_max);
        sps30_interface_debug_print("sps30: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    sps30_interface_debug_print("sps30: start register test.\n");
    
    /* sps30_set_interface/sps30_get_interface test */
    sps30_interface_debug_print("sps30: sps30_set_interface/sps30_get_interface test.\n");
    
    /* set iic interface */
    res = sps30_set_interface(&gs_handle, SPS30_INTERFACE_IIC);
    if (res)
    {
        sps30_interface_debug_print("sps30: set interface failed.\n");
    
        return 1;
    }
    sps30_interface_debug_print("sps30: set interface iic.\n");
    res = sps30_get_interface(&gs_handle, &interface_check);
    if (res)
    {
        sps30_interface_debug_print("gps30: set interface failed.\n");
    
        return 1;
    }
    sps30_interface_debug_print("sps30: check interface %s.\n", interface_check == SPS30_INTERFACE_IIC ? "ok" : "error");
    
    /* set uart interface */
    res = sps30_set_interface(&gs_handle, SPS30_INTERFACE_UART);
    if (res)
    {
        sps30_interface_debug_print("sps30: set interface failed.\n");
    
        return 1;
    }
    sps30_interface_debug_print("sps30: set interface uart.\n");
    res = sps30_get_interface(&gs_handle, &interface_check);
    if (res)
    {
        sps30_interface_debug_print("gps30: set interface failed.\n");
    
        return 1;
    }
   sps30_interface_debug_print("sps30: check interface %s.\n", interface_check == SPS30_INTERFACE_UART ? "ok" : "error");
    
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
    
    /* sps30_set_auto_cleaning_interval/sps30_get_auto_cleaning_interval test */
    res = sps30_interface_debug_print("sps30: sps30_set_auto_cleaning_interval/sps30_get_auto_cleaning_interval test.\n");
    
    second = rand() % 65536 + 65536;
    res = sps30_set_auto_cleaning_interval(&gs_handle, second);
    if (res)
    {
        sps30_interface_debug_print("sps30: set auto cleaning interval failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: set auto cleaning interval %d.\n", second);
    res = sps30_get_auto_cleaning_interval(&gs_handle, (uint32_t *)&second_check);
    if (res)
    {
        sps30_interface_debug_print("sps30: get auto cleaning interval failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check interval %s.\n", second_check == second ? "ok" : "error");
    
    /* sps30_disable_auto_cleaning_interval test */
    sps30_interface_debug_print("sps30: sps30_disable_auto_cleaning_interval test.\n");
    res = sps30_disable_auto_cleaning_interval(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: disable auto cleaning interval failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check disable cleaning %s.\n", res == 0 ? "ok" : "error");
    
    /* sps30_start_fan_cleaning test */
    sps30_interface_debug_print("sps30: sps30_start_fan_cleaning test.\n");
    res = sps30_start_measurement(&gs_handle, SPS30_FORMAT_IEEE754);
    if (res)
    {
        sps30_interface_debug_print("sps30: start measurement failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    res = sps30_start_fan_cleaning(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: start fan cleaning failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 10s */
    sps30_interface_delay_ms(1000 * 10);
    sps30_interface_debug_print("sps30: check fan cleaning %s.\n", res == 0 ? "ok" : "error");
    res = sps30_stop_measurement(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: stop measurement failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sps30_get_product_type test */
    sps30_interface_debug_print("sps30: sps30_get_product_type test.\n");
    res = sps30_get_product_type(&gs_handle, (char *)type);
    if (res)
    {
        sps30_interface_debug_print("sps30: get product type failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: type is %s.\n", type);
    
    /* sps30_get_serial_number test */
    sps30_interface_debug_print("sps30: sps30_get_serial_number test.\n");
    res = sps30_get_serial_number(&gs_handle, (char *)sn);
    if (res)
    {
        sps30_interface_debug_print("sps30: get serial number failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: serial number is %s.\n", sn);
    
    /* sps30_get_version test */
    sps30_interface_debug_print("sps30: sps30_get_version test.\n");
    res = sps30_get_version(&gs_handle, (uint8_t *)&major, (uint8_t *)&minor);
    if (res)
    {
        sps30_interface_debug_print("sps30: get version failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: major is 0x%02X minor is 0x%02X .\n", major, minor);
    
    /* sps30_get_device_status test */
    sps30_interface_debug_print("sps30: sps30_get_device_status test.\n");
    res = sps30_get_device_status(&gs_handle, (uint32_t *)&status);
    if (res)
    {
        sps30_interface_debug_print("sps30: sps30 get device status failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: status is 0x%X .\n", status);
    
    /* sps30_clear_device_status test */
    sps30_interface_debug_print("sps30: sps30_clear_device_status test.\n");
    res = sps30_clear_device_status(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: clear device status failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check clear status %s.\n", res == 0 ? "ok" : "error");
    
    /* iic interface */
    if (interface == SPS30_INTERFACE_IIC)
    {
        /* sps30_read_data_flag test */
        sps30_interface_debug_print("sps30: sps30_read_data_flag test.\n");
        res = sps30_read_data_flag(&gs_handle, &flag);
        if (res)
        {
            sps30_interface_debug_print("sps30: sps30 read data flag failed.\n");
            sps30_deinit(&gs_handle);
            
            return 1;
        }
        sps30_interface_debug_print("sps30: flag is 0x%X .\n", flag);
    }
    
    /* sps30_start_measurement/sps30_stop_measurement test */
    sps30_interface_debug_print("sps30: sps30_start_measurement/sps30_stop_measurement test.\n");
    
    /* start measurement */
    res = sps30_start_measurement(&gs_handle, SPS30_FORMAT_IEEE754);
    if (res)
    {
        sps30_interface_debug_print("sps30: start measurement failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check start measurement %s.\n", res == 0 ? "ok" : "error");
    sps30_interface_delay_ms(1000);
    
    /* stop measurement */
    res = sps30_stop_measurement(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: stop measurement failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check stop measurement %s.\n", res == 0 ? "ok" : "error");
    
    /* sps30_sleep test */
    sps30_interface_debug_print("sps30: sps30_sleep test.\n");
    res = sps30_sleep(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: sleep failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check sleep %s.\n", res == 0 ? "ok" : "error");
    
    /* sps30_wake_up test */
    sps30_interface_debug_print("sps30: sps30_wake_up test.\n");
    sps30_interface_delay_ms(1000);
    res = sps30_wake_up(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: wake up failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check wake up %s.\n", res == 0 ? "ok" : "error");
    
    /* sps30_reset test */
    sps30_interface_debug_print("sps30: sps30_reset test.\n");
    sps30_interface_delay_ms(1000);
    res = sps30_reset(&gs_handle);
    if (res)
    {
        sps30_interface_debug_print("sps30: reset failed.\n");
        sps30_deinit(&gs_handle);
        
        return 1;
    }
    sps30_interface_debug_print("sps30: check reset %s.\n", res == 0 ? "ok" : "error");
    
    /* finish register test */
    sps30_interface_debug_print("sps30: finish register test.\n");
    sps30_deinit(&gs_handle);
    
    return 0;
}
