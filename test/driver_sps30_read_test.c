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
 * @file      driver_sps30_read_test.c
 * @brief     driver sps30 read test source file
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

#include "driver_sps30_read_test.h"

static sps30_handle_t gs_handle;        /**< sps30 handle */

/**
 * @brief     read test
 * @param[in] interface chip interface
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sps30_read_test(sps30_interface_t interface, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    sps30_info_t info;
    sps30_pm_t pm;
    
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
    if (res != 0)
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
    
    /* start read test */
    sps30_interface_debug_print("sps30: start read test.\n");
    
    /* set the interface */
    res = sps30_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: set interface failed.\n");
    
        return 1;
    }
    
    /* init the chip */
    res = sps30_init(&gs_handle);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: init failed.\n");
    
        return 1;
    }
    
    /* set auto cleaning interval */
    res = sps30_set_auto_cleaning_interval(&gs_handle, 3600 * 24);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: set auto cleaning interval failed.\n");
        (void)sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* IEEE754 read test */
    sps30_interface_debug_print("sps30: IEEE754 read test.\n");
    
    /* start measurement */
    res = sps30_start_measurement(&gs_handle, SPS30_FORMAT_IEEE754);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: start measurement failed.\n");
        (void)sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    sps30_interface_delay_ms(2000);
    
    for (i = 0; i < times; i++)
    {
        res = sps30_read(&gs_handle, &pm);
        if (res != 0)
        {
            sps30_interface_debug_print("sps30: read failed.\n");
            (void)sps30_deinit(&gs_handle);
            
            return 1;
        }
        sps30_interface_debug_print("sps30: pm1.0 is %0.2f ug/m3.\n", pm.pm1p0_ug_m3);
        sps30_interface_debug_print("sps30: pm2.5 is %0.2f ug/m3.\n", pm.pm2p5_ug_m3);
        sps30_interface_debug_print("sps30: pm4.0 is %0.2f ug/m3.\n", pm.pm4p0_ug_m3);
        sps30_interface_debug_print("sps30: pm10.0 is %0.2f ug/m3.\n", pm.pm10_ug_m3);
        sps30_interface_debug_print("sps30: pm0.5 is %0.2f cm3.\n", pm.pm0p5_cm3);
        sps30_interface_debug_print("sps30: pm1.0 is %0.2f cm3.\n", pm.pm1p0_cm3);
        sps30_interface_debug_print("sps30: pm2.5 is %0.2f cm3.\n", pm.pm2p5_cm3);
        sps30_interface_debug_print("sps30: pm4.0 is %0.2f cm3.\n", pm.pm4p0_cm3);
        sps30_interface_debug_print("sps30: pm10.0 is %0.2f cm3.\n", pm.pm10_cm3);
        sps30_interface_debug_print("sps30: typical is %0.2f um.\n", pm.typical_particle_um);
        
        /* delay 2000 ms */
        sps30_interface_delay_ms(2000);
    }
    
    /* stop measurement */
    res = sps30_stop_measurement(&gs_handle);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: stop measurement failed.\n");
        (void)sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    sps30_interface_delay_ms(2000);
    
    /* UINT16 read test */
    sps30_interface_debug_print("sps30: UINT16 read test.\n");
    
    /* start measurement */
    res = sps30_start_measurement(&gs_handle, SPS30_FORMAT_UINT16);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: start measurement failed.\n");
        (void)sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    sps30_interface_delay_ms(2000);
    
    for (i = 0; i < times; i++)
    {
        res = sps30_read(&gs_handle, &pm);
        if (res != 0)
        {
            sps30_interface_debug_print("sps30: read failed.\n");
            (void)sps30_deinit(&gs_handle);
            
            return 1;
        }
        sps30_interface_debug_print("sps30: pm1.0 is %0.2f ug/m3.\n", pm.pm1p0_ug_m3);
        sps30_interface_debug_print("sps30: pm2.5 is %0.2f ug/m3.\n", pm.pm2p5_ug_m3);
        sps30_interface_debug_print("sps30: pm4.0 is %0.2f ug/m3.\n", pm.pm4p0_ug_m3);
        sps30_interface_debug_print("sps30: pm10.0 is %0.2f ug/m3.\n", pm.pm10_ug_m3);
        sps30_interface_debug_print("sps30: pm0.5 is %0.2f cm3.\n", pm.pm0p5_cm3);
        sps30_interface_debug_print("sps30: pm1.0 is %0.2f cm3.\n", pm.pm1p0_cm3);
        sps30_interface_debug_print("sps30: pm2.5 is %0.2f cm3.\n", pm.pm2p5_cm3);
        sps30_interface_debug_print("sps30: pm4.0 is %0.2f cm3.\n", pm.pm4p0_cm3);
        sps30_interface_debug_print("sps30: pm10.0 is %0.2f cm3.\n", pm.pm10_cm3);
        sps30_interface_debug_print("sps30: typical is %0.2f um.\n", pm.typical_particle_um);

        
        /* delay 2000 ms */
        sps30_interface_delay_ms(2000);
    }
    
    /* stop measurement */
    res = sps30_stop_measurement(&gs_handle);
    if (res != 0)
    {
        sps30_interface_debug_print("sps30: stop measurement failed.\n");
        (void)sps30_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2000 ms */
    sps30_interface_delay_ms(2000);
    
    /* finish read test */
    sps30_interface_debug_print("sps30: finish read test.\n");
    (void)sps30_deinit(&gs_handle);
    
    return 0;
}
