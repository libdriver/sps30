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
#include <getopt.h>
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
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"interface", required_argument, NULL, 1},
        {"times", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[32] = "unknow";
    uint32_t times = 3;
    sps30_interface_t interface = SPS30_INTERFACE_IIC;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args*/
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 32);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 32);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 32);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 32);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 32);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* interface */
            case 1 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = SPS30_INTERFACE_IIC;
                }
                else if (strcmp("uart", optarg) == 0)
                {
                    interface = SPS30_INTERFACE_UART;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 2 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* reg test */
        if (sps30_register_test(interface) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* read test */
        if (sps30_read_test(interface, times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 2000 ms */
        sps30_interface_delay_ms(2000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            sps30_pm_t pm;
            
            /* read data */
            res = sps30_basic_read(&pm);
            if (res != 0)
            {
                (void)sps30_basic_deinit();
                
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
        
        /* deinit */
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_type", type) == 0)
    {
        uint8_t res;
        char type[9];
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* get the type */
        res = sps30_basic_get_product_type((char *)type);
        if (res != 0)
        {
            (void)sps30_basic_deinit();
            
            return 1;
        }
        
        /* print */
        sps30_interface_debug_print("sps30: type is %s.\n", type);
        
        /* deinit */
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_sn", type) == 0)
    {
        uint8_t res;
        char sn[9];
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* get the sn */
        res = sps30_basic_get_serial_number((char *)sn);
        if (res != 0)
        {
            (void)sps30_basic_deinit();
            
            return 1;
        }
        
        /* print */
        sps30_interface_debug_print("sps30: sn is %s.\n", sn);
        
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_clean", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* print */
        sps30_interface_debug_print("sps30: start cleaning.\n");
        
        /* start fan cleaning */
        res = sps30_basic_start_fan_cleaning();
        if (res != 0)
        {
            (void)sps30_basic_deinit();
            
            return 1;
        }
        
        /* delay 10s */
        sps30_interface_delay_ms(1000 * 10);
        
        /* print */
        sps30_interface_debug_print("sps30: stop cleaning.\n");
        
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_version", type) == 0)
    {
        uint8_t res;
        uint8_t major, minor;
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* get the version */
        res = sps30_basic_get_version((uint8_t *)&major, (uint8_t *)&minor);
        if (res != 0)
        {
            (void)sps30_basic_deinit();
            
            return 1;
        }
        
        /* print */
        sps30_interface_debug_print("sps30: major is 0x%02X minor is 0x%02X.\n", major, minor);
        
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_status", type) == 0)
    {
        uint8_t res;
        uint32_t status;
        
        /* init */
        res = sps30_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* get the status */
        res = sps30_basic_get_status((uint32_t *)&status);
        if (res != 0)
        {
            (void)sps30_basic_deinit();
            
            return 1;
        }
        
        /* print */
        sps30_interface_debug_print("sps30: status is %08X.\n", status);
        
        (void)sps30_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        sps30_interface_debug_print("Usage:\n");
        sps30_interface_debug_print("  sps30 (-i | --information)\n");
        sps30_interface_debug_print("  sps30 (-h | --help)\n");
        sps30_interface_debug_print("  sps30 (-p | --port)\n");
        sps30_interface_debug_print("  sps30 (-t reg | --test=reg) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("  sps30 (-t read | --test=read) [--interface=<iic | uart>] [--times=<num>]\n");
        sps30_interface_debug_print("  sps30 (-e read | --example=read) [--interface=<iic | uart>] [--times=<num>]\n");
        sps30_interface_debug_print("  sps30 (-e type | --example=type) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("  sps30 (-e sn | --example=sn) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("  sps30 (-e clean | --example=clean) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("  sps30 (-e version | --example=version) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("  sps30 (-e status | --example=status) [--interface=<iic | uart>]\n");
        sps30_interface_debug_print("\n");
        sps30_interface_debug_print("Options:\n");
        sps30_interface_debug_print("  -e <read | type | sn | clean | version | status>, --example=<read | type | sn | clean | version | status>\n");
        sps30_interface_debug_print("                                          Run the driver example.\n");
        sps30_interface_debug_print("  -h, --help                              Show the help.\n");
        sps30_interface_debug_print("  -i, --information                       Show the chip information.\n");
        sps30_interface_debug_print("      --interface=<iic | uart>            Set the chip interface.([default: iic])\n");
        sps30_interface_debug_print("  -p, --port                              Display the pin connections of the current board.\n");
        sps30_interface_debug_print("  -t <reg | read>, --test=<reg | read>    Run the driver test.\n");
        sps30_interface_debug_print("      --times=<num>                       Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
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
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        sps30_interface_debug_print("sps30: IIC interface SCL connected to GPIO3(BCM3).\n");
        sps30_interface_debug_print("sps30: IIC interface SDA connected to GPIO2(BCM2).\n");
        sps30_interface_debug_print("sps30: UART interface TX connected to GPIO14(BCM14).\n");
        sps30_interface_debug_print("sps30: UART interface RX connected to GPIO15(BCM15).\n");
        
        return 0;
    }
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
