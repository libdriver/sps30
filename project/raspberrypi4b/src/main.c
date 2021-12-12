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
 * @file      main.c
 * @brief     main source file
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
#include "driver_sps30_read_test.h"
#include "driver_sps30_basic.h"
#include <stdlib.h>

/**
 * @brief     sps30 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t sps30(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            sps30_info_t info;
            
            /* print sps30 info */
            
            sps30_info(&info);
            sps30_interface_debug_print("sps30: chip is %s.\n", info.chip_name);
            sps30_interface_debug_print("sps30: manufacturer is %s.\n", info.manufacturer_name);
            sps30_interface_debug_print("sps30: interface is %s.\n", info.interface);
            sps30_interface_debug_print("sps30: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
            sps30_interface_debug_print("sps30: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            sps30_interface_debug_print("sps30: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            sps30_interface_debug_print("sps30: max current is %0.2fmA.\n", info.max_current_ma);
            sps30_interface_debug_print("sps30: max temperature is %0.1fC.\n", info.temperature_max);
            sps30_interface_debug_print("sps30: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            
            sps30_interface_debug_print("sps30: IIC interface SCL connected to GPIO3(BCM3).\n");
            sps30_interface_debug_print("sps30: IIC interface SDA connected to GPIO2(BCM2).\n");
            sps30_interface_debug_print("sps30: UART interface TX connected to GPIO14(BCM14).\n");
            sps30_interface_debug_print("sps30: UART interface RX connected to GPIO15(BCM15).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show sps30 help */
            
            help:
            
            sps30_interface_debug_print("sps30 -i\n\tshow sps30 chip and driver information.\n");
            sps30_interface_debug_print("sps30 -h\n\tshow sps30 help.\n");
            sps30_interface_debug_print("sps30 -p\n\tshow sps30 pin connections of the current board.\n");
            sps30_interface_debug_print("sps30 -t reg (-iic | -uart)\n\trun sps30 register test.\n");
            sps30_interface_debug_print("sps30 -t read <times> (-iic | -uart)\n\trun sps30 read test.times means the test times.\n");
            sps30_interface_debug_print("sps30 -c basic read <times> (-iic | -uart)\n\trun sps30 basic read function.times is the read times.\n");
            sps30_interface_debug_print("sps30 -c basic type (-iic | -uart)\n\trun sps30 basic get type function.\n");
            sps30_interface_debug_print("sps30 -c basic sn (-iic | -uart)\n\trun sps30 basic get sn function.\n");
            sps30_interface_debug_print("sps30 -c basic clean (-iic | -uart)\n\trun sps30 basic clean function.\n");
            sps30_interface_debug_print("sps30 -c basic version (-iic | -uart)\n\trun sps30 basic get version function.\n");
            sps30_interface_debug_print("sps30 -c basic status (-iic | -uart)\n\trun sps30 basic get status function.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    if (sps30_register_test(SPS30_INTERFACE_IIC))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if (strcmp("-uart", argv[3]) == 0)
                {
                    if (sps30_register_test(SPS30_INTERFACE_UART))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[4]) == 0)
                {
                    if (sps30_read_test(SPS30_INTERFACE_IIC, atoi(argv[3])))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if (strcmp("-uart", argv[4]) == 0)
                {
                    if (sps30_read_test(SPS30_INTERFACE_UART, atoi(argv[3])))
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("type", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile char type[9];
                        
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_product_type((char *)type);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: type is %s.\n", type);
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile char type[9];
                        
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_product_type((char *)type);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: type is %s.\n", type);
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else if (strcmp("sn", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile char sn[9];
                        
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_serial_number((char *)sn);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: sn is %s.\n", sn);
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile char sn[9];
                        
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_serial_number((char *)sn);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: sn is %s.\n", sn);
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else if (strcmp("version", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint8_t major, minor;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_version((uint8_t *)&major, (uint8_t *)&minor);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: major is 0x%02X minor is 0x%02X.\n", major, minor);
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint8_t major, minor;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_version((uint8_t *)&major, (uint8_t *)&minor);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: major is 0x%02X minor is 0x%02X.\n", major, minor);
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else if (strcmp("status", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint32_t status;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_status((uint32_t *)&status);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: status is %08X.\n", status);
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint32_t status;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        res = sps30_basic_get_status((uint32_t *)&status);
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: status is %08X.\n", status);
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else if (strcmp("clean", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: start cleaning.\n");
                        
                        /* start fan cleaning */
                        res = sps30_basic_start_fan_cleaning();
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        sps30_interface_delay_ms(1000 * 10);
                        
                        /* print */
                        sps30_interface_debug_print("sps30: stop cleaning.\n");
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[4]) == 0)
                    {
                        volatile uint8_t res;
                        
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        
                        /* print */
                        sps30_interface_debug_print("sps30: start cleaning.\n");
                        
                        /* start fan cleaning */
                        res = sps30_basic_start_fan_cleaning();
                        if (res)
                        {
                            sps30_basic_deinit();
                            
                            return 1;
                        }
                        sps30_interface_delay_ms(1000 * 10);
                        
                        /* print */
                        sps30_interface_debug_print("sps30: stop cleaning.\n");
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("read", argv[3]) == 0)
                {
                    if (strcmp("-iic", argv[5]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint32_t i, times;
                        
                        times = atoi(argv[4]);
                        res = sps30_basic_init(SPS30_INTERFACE_IIC);
                        if (res)
                        {
                            return 1;
                        }
                        
                        /* delay 2000 ms */
                        sps30_interface_delay_ms(2000);
                        
                        for (i = 0; i < times; i++)
                        {
                            sps30_pm_t pm;
                            
                            /* read data */
                            res = sps30_basic_read(&pm);
                            if (res)
                            {
                                sps30_basic_deinit();
                                
                                return 1;
                            }
                            
                            /* print */
                            sps30_interface_debug_print("sps30: %d/%d.\n", i + 1, times);
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
                        
                        return sps30_basic_deinit();
                    }
                    else if (strcmp("-uart", argv[5]) == 0)
                    {
                        volatile uint8_t res;
                        volatile uint32_t i, times;
                        
                        times = atoi(argv[4]);
                        res = sps30_basic_init(SPS30_INTERFACE_UART);
                        if (res)
                        {
                            return 1;
                        }
                        
                        /* delay 2000 ms */
                        sps30_interface_delay_ms(2000);
                        
                        for (i = 0; i < times; i++)
                        {
                            sps30_pm_t pm;
                            
                            /* read data */
                            res = sps30_basic_read(&pm);
                            if (res)
                            {
                                sps30_basic_deinit();
                                
                                return 1;
                            }
                            
                            /* print */
                            sps30_interface_debug_print("sps30: %d/%d.\n", i + 1, times);
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
                        
                        return sps30_basic_deinit();
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = sps30(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        sps30_interface_debug_print("sps30: run failed.\n");
    }
    else if (res == 5)
    {
        sps30_interface_debug_print("sps30: param is invalid.\n");
    }
    else
    {
        sps30_interface_debug_print("sps30: unknow status code.\n");
    }

    return 0;
}
