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
 * @file      driver_sps30.c
 * @brief     driver sps30 source file
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

#include "driver_sps30.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion SPS30"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        4.50f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.50f                    /**< chip max supply voltage */
#define MAX_CURRENT               80.0f                    /**< chip max current */
#define TEMPERATURE_MIN           -10.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           60.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip address definition
 */
#define SPS30_ADDRESS             (0x69 << 1)              /**< chip iic address */

/**
 * @brief chip command definition
 */
#define SPS30_IIC_COMMAND_START_MEASUREMENT                        0x0010U        /**< start measurement command */
#define SPS30_IIC_COMMAND_STOP_MEASUREMENT                         0x0104U        /**< stop measurement command */
#define SPS30_IIC_COMMAND_READ_DATA_READY_FLAG                     0x0202U        /**< read data ready flag command */
#define SPS30_IIC_COMMAND_READ_MEASURED_VALUES                     0x0300U        /**< read measured values command */
#define SPS30_IIC_COMMAND_SLEEP                                    0x1001U        /**< sleep command */
#define SPS30_IIC_COMMAND_WAKE_UP                                  0x1103U        /**< wake up command */
#define SPS30_IIC_COMMAND_START_FAN_CLEANING                       0x5607U        /**< start fan cleaning command */
#define SPS30_IIC_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL        0x8004U        /**< read/write auto cleaning interval command */
#define SPS30_IIC_COMMAND_READ_PRODUCT_TYPE                        0xD002U        /**< read product type command */
#define SPS30_IIC_COMMAND_READ_SERIAL_NUMBER                       0xD033U        /**< read serial number command */
#define SPS30_IIC_COMMAND_READ_VERSION                             0xD100U        /**< read version command */
#define SPS30_IIC_COMMAND_READ_DEVICE_STATUS_REG                   0xD206U        /**< read device status register command */
#define SPS30_IIC_COMMAND_CLEAR_DEVICE_STATUS_REG                  0xD210U        /**< clear device status register command */
#define SPS30_IIC_COMMAND_RESET                                    0xD304U        /**< reset command */
#define SPS30_UART_COMMAND_START_MEASUREMENT                       0x00           /**< start measurement command */
#define SPS30_UART_COMMAND_STOP_MEASUREMENT                        0x01           /**< stop measurement command */
#define SPS30_UART_COMMAND_READ_MEASURED_VALUES                    0x03           /**< read measured values command */
#define SPS30_UART_COMMAND_SLEEP                                   0x10           /**< sleep command */
#define SPS30_UART_COMMAND_WAKE_UP                                 0x11           /**< wake up command */
#define SPS30_UART_COMMAND_START_FAN_CLEANING                      0x56           /**< start fan cleaning command */
#define SPS30_UART_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL       0x80           /**< read/write auto cleaning interval command */
#define SPS30_UART_COMMAND_READ_PRODUCT_TYPE                       0xD0           /**< read product type command */
#define SPS30_UART_COMMAND_READ_VERSION                            0xD1           /**< read version command */
#define SPS30_UART_COMMAND_READ_DEVICE_STATUS_REG                  0xD2           /**< read device status register command */
#define SPS30_UART_COMMAND_RESET                                   0xD3           /**< reset command */

/**
 * @brief     generate the crc
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_sps30_generate_crc(sps30_handle_t *handle, uint8_t* data, uint8_t count) 
{
    if (handle->iic_uart == SPS30_INTERFACE_IIC)
    {
        uint8_t current_byte;
        uint8_t crc = 0xFF;
        uint8_t crc_bit;

        for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
        {
            crc ^= (data[current_byte]);                                    /* xor data */
            for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
            {
                if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
                {
                    crc = (crc << 1) ^ 0x31;                                /* xor */
                }
                else
                {
                    crc = crc << 1;                                         /* left shift 1 */
                }
            }
        }
        
        return crc;                                                         /* return crc */
    }
    else
    {
        uint8_t i;
        uint32_t crc = 0x00;
        
        for (i = 0; i < count; i++)                                         /* sum */
        {
            crc += data[i];                                                 /* sum */
        }
        
        return (uint8_t)(~(crc & 0xFF));                                    /* take the least significant byte of the result and invert it */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  addr is the iic device address
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_sps30_iic_read(sps30_handle_t *handle, uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    buf[0] = (reg >> 8) & 0xFF;                                  /* set msb */
    buf[1] = (reg >> 0) & 0xFF;                                  /* set lsb */
    if (handle->iic_write_cmd(addr, (uint8_t *)buf, 2) != 0)     /* write data */
    {
        return 1;                                                /* return error */
    }
    handle->delay_ms(delay_ms);                                  /* delay ms */
    if (handle->iic_read_cmd(addr, (uint8_t *)data, len) != 0)   /* read data */
    {
        return 1;                                                /* return error */
    }
    
    return 0;                                                    /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] addr is the iic device address
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @param[in] delay_ms is the delay time in ms
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_sps30_iic_write(sps30_handle_t *handle, uint8_t addr, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[16];
    
    if (len > 14)                                                    /* check length */
    {
        return 1;                                                    /* return error */
    }
    buf[0] = (reg >> 8) & 0xFF;                                      /* set msb */
    buf[1] = (reg >> 0) & 0xFF;                                      /* set lsb */
    memcpy((uint8_t *)&buf[2], data, len);                           /* copy data */
    if (handle->iic_write_cmd(addr, (uint8_t *)buf, len + 2) != 0)   /* write data */
    {
        return 1;                                                    /* return error */
    }
    handle->delay_ms(delay_ms);                                      /* delay ms */
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief      uart get the rx frame
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  len is the buffer length
 * @param[out] *output points to a output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 uart get rx frame failed
 * @note       none
 */
static uint8_t a_sps30_uart_get_rx_frame(sps30_handle_t *handle, uint16_t len, uint8_t *output, uint16_t out_len)
{
    uint16_t i, point;
    
    output[0] = handle->buf[0];                                                                          /* set buf[0] */
    point = 1;                                                                                           /* set point 1 */
    for (i = 1; i < (len - 1); i++)                                                                      /* run n -2 times */
    {
        if (point >= (out_len - 1))                                                                      /* check length */
        {
            return 1;                                                                                    /* return error */
        }
        if (((handle->buf[i] == 0x7D) && (handle->buf[i + 1]) == 0x5E) ||                                /* check buffer */
            ((handle->buf[i] == 0x7D) && (handle->buf[i + 1]) == 0x5D) ||
            ((handle->buf[i] == 0x7D) && (handle->buf[i + 1]) == 0x31) ||
            ((handle->buf[i] == 0x7D) && (handle->buf[i + 1]) == 0x33)
           )
        {
            switch (handle->buf[i + 1])                                                                  /* judge */
            {
                case 0x5E :                                                                              /* 0x5E */
                {
                    output[point] = 0x7E;                                                                /* set output */
                    point++;                                                                             /* point++ */
                    i++;                                                                                 /* i++ */
                    
                    break;                                                                               /* break */
                }
                case 0x5D :                                                                              /* 0x5D */
                {
                    output[point] = 0x7D;                                                                /* set output */
                    point++;                                                                             /* point++ */
                    i++;                                                                                 /* i++ */
                    
                    break;                                                                               /* break */
                }
                case 0x31 :                                                                              /* 0x31 */
                {
                    output[point] = 0x11;                                                                /* set output */
                    point++;                                                                             /* point++ */
                    i++;                                                                                 /* i++ */
                    
                    break;                                                                               /* break */
                }
                case 0x33 :                                                                              /* 0x33 */
                {
                    output[point] = 0x13;                                                                /* set output */
                    point++;                                                                             /* point++ */
                    i++;                                                                                 /* i++ */
                    
                    break;                                                                               /* break */
                }
                default :
                {
                    break;                                                                               /* break */
                }
            }
        }
        else
        {
            output[point] = handle->buf[i];                                                              /* set output */
            point++;                                                                                     /* point++ */
        }
    }
    output[point] = handle->buf[len - 1];                                                                /* set the end part */
    point++;                                                                                             /* point++ */
    if (point != out_len)                                                                                /* check point */
    {
        return 1;                                                                                        /* return error */
    }
    
    return 0;                                                                                            /* success return 0 */
}

/**
 * @brief      uart set the tx frame
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  *input points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_len points to a output lengh buffer
 * @return     status code
 *             - 0 success
 *             - 1 uart set tx frame failed
 * @note       none
 */
static uint8_t a_sps30_uart_set_tx_frame(sps30_handle_t *handle, uint8_t *input, uint16_t in_len, uint16_t *out_len)
{
    uint16_t i;
    
    memset(handle->buf, 0, sizeof(uint8_t) * 256);                                                            /* clear buffer */
    handle->buf[0] = input[0];                                                                                /* set buf[0] */
    *out_len = 1;                                                                                             /* set output lenght */
    for (i = 1; i < (in_len - 1); i++)
    {
        if ((*out_len) >= 255)                                                                                /* check output length */
        {
            return 1;                                                                                         /* return error */
        }
        if ((input[i] == 0x7E) || (input[i] == 0x7D) ||                                                       /* check data */
            (input[i] == 0x11) || (input[i] == 0x13)
           )
        {
            switch (input[i])                                                                                 /* judge input */
            {
                case 0x7E :                                                                                   /* 0x7E */
                {
                    handle->buf[*out_len] = 0x7D;                                                             /* 0x7D */
                    (*out_len)++;                                                                             /* output length++ */
                    handle->buf[*out_len] = 0x5E;                                                             /* 0x5E */
                    (*out_len)++;                                                                             /* output length++ */
                    
                    break;                                                                                    /* break */
                }
                case 0x7D :                                                                                   /* 0x7D */
                {
                    handle->buf[*out_len] = 0x7D;                                                             /* set 0x7D */
                    (*out_len)++;                                                                             /* output length++ */
                    handle->buf[*out_len] = 0x5D;                                                             /* set 0x5D */
                    (*out_len)++;                                                                             /* output length++ */
                    
                    break;                                                                                    /* break */
                }
                case 0x11 :                                                                                   /* 0x11 */
                {
                    handle->buf[*out_len] = 0x7D;                                                             /* set 0x7D */
                    (*out_len)++;                                                                             /* output length++ */
                    handle->buf[*out_len] = 0x31;                                                             /* set 0x31 */
                    (*out_len)++;                                                                             /* output length++ */
                    
                    break;                                                                                    /* break */
                }
                case 0x13 :                                                                                   /* 0x13 */
                {
                    handle->buf[*out_len] = 0x7D;                                                             /* set 0x7D */
                    (*out_len)++;                                                                             /* output length++ */
                    handle->buf[*out_len] = 0x33;                                                             /* set 0x33 */
                    (*out_len)++;                                                                             /* output length++ */
                    
                    break;                                                                                    /* break */
                }
                default :
                {
                    break;                                                                                    /* break */
                }
            }
        }
        else
        {
            handle->buf[*out_len] = input[i];                                                                 /* set the buffer */
            (*out_len)++;                                                                                     /* output length++ */
        }
    }
    handle->buf[*out_len] = input[in_len - 1];
    (*out_len)++;                                                                                             /* copy the end frame */
    
    return 0;                                                                                                 /* success return 0 */
}

/**
 * @brief      write read bytes
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  *input points to a input buffer
 * @param[in]  in_len is the input length
 * @param[in]  delay_ms is the delay time in ms
 * @param[out] *output points to a output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_sps30_uart_write_read(sps30_handle_t *handle, uint8_t *input, uint16_t in_len,
                                       uint16_t delay_ms, uint8_t *output, uint16_t out_len)
{
    uint16_t len;
    
    if (a_sps30_uart_set_tx_frame(handle, input, in_len, (uint16_t *)&len) != 0)  /* set tx frame */
    {
        return 1;                                                                 /* return error */
    }
    if (handle->uart_flush() != 0)                                                /* uart flush */
    {
        return 1;                                                                 /* return error */
    }
    if (handle->uart_write(handle->buf, len) != 0)                                /* write data */
    {
        return 1;                                                                 /* return error */
    }
    handle->delay_ms(delay_ms);                                                   /* delay ms */
    len = handle->uart_read(handle->buf, 256);                                    /* read data */
    if (a_sps30_uart_get_rx_frame(handle, len, output, out_len) != 0)             /* get rx frame */
    {
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief     print error
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] e is the error code
 * @return    error code
 * @note      none
 */
static uint8_t a_sps30_uart_error(sps30_handle_t *handle, uint8_t e)
{
    switch (e)
    {
        case 0x00 :
        {
            break;
        }
        case 0x01 :
        {
            handle->debug_print("sps30: wrong data length for this command error.\n");        /* wrong data length for this command error */
            
            break;
        }
        case 0x02 :
        {
            handle->debug_print("sps30: unknown command.\n");                                 /* unknown command */
            
            break;
        }
        case 0x03 :
        {
            handle->debug_print("sps30: no access right for command.\n");                     /* no access right for command */
            
            break;
        }
        case 0x04 :
        {
            handle->debug_print("sps30: illegal command parameter or parameter "
                                "out of allowed range.\n");                                   /* illegal command parameter or parameter out of allowed range */
            
            break;
        }
        case 0x28 :
        {
            handle->debug_print("sps30: internal function argument out of range.\n");         /* internal function argument out of range */
            
            break;
        }
        case 0x43 :
        {
            handle->debug_print("sps30: command not allowed in current state.\n");            /* command not allowed in current state */
            
            break;
        }
        default :
        {
            handle->debug_print("sps30: unknow code.\n");                                     /* unknow code */
            
            break;
        }
    }
    
    return e;                                                                                 /* return error code */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sps30_set_interface(sps30_handle_t *handle, sps30_interface_t interface) 
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    
    handle->iic_uart = (uint8_t)interface;        /* set interface */
    
    return 0;                                     /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sps30_get_interface(sps30_handle_t *handle, sps30_interface_t *interface) 
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }
    
    *interface = (sps30_interface_t)(handle->iic_uart);        /* get interface */
    
    return 0;                                                  /* success return 0 */
}

/**
 * @brief     start the measurement
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] format is the data format
 * @return    status code
 *            - 0 success
 *            - 1 start measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_start_measurement(sps30_handle_t *handle, sps30_format_t format)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                                   /* check handle */
    {
        return 2;                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                         /* return error */
    }

    if (handle->iic_uart != 0)                                                                                            /* uart */
    {
        uint8_t input_buf[6 + 2];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                                              /* set start */
        input_buf[1] = 0x00;                                                                                              /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_START_MEASUREMENT;                                                              /* set command */
        input_buf[3] = 0x02;                                                                                              /* set length */
        input_buf[4] = 0x01;                                                                                              /* set 0x01 */
        input_buf[5] = format;                                                                                            /* set format */
        handle->format = format;                                                                                          /* save format */
        input_buf[6] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 5);                                         /* set crc */
        input_buf[7] = 0x7E;                                                                                              /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                          /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 8, 20, (uint8_t *)out_buf, 7);                        /* write read frame */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                           /* write read failed */
           
            return 1;                                                                                                     /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                                        /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                             /* crc check error */
           
            return 1;                                                                                                     /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                                  /* check status */
        {
            return 1;                                                                                                     /* return error */
        }
    }
    else                                                                                                                  /* iic */
    {
        uint8_t buf[3];
        
        buf[0] = format;                                                                                                  /* set format */
        handle->format = format;                                                                                          /* save format */
        buf[1] = 0x00;                                                                                                    /* set dummy */
        buf[2] = a_sps30_generate_crc(handle, (uint8_t *)buf, 2);                                                         /* generate crc */
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_START_MEASUREMENT, (uint8_t *)buf, 3, 20);       /* start measurement command */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: start measurement failed.\n");                                                    /* start measurement failed */
           
            return 1;                                                                                                     /* return error */
        }
    }
    
    return 0;                                                                                                             /* success return 0 */
}

/**
 * @brief     stop the measurement
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_stop_measurement(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (handle->iic_uart != 0)                                                                                 /* uart */
    {
        uint8_t input_buf[6 + 0];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                                   /* set start */
        input_buf[1] = 0x00;                                                                                   /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_STOP_MEASUREMENT;                                                    /* set command */
        input_buf[3] = 0x00;                                                                                   /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                              /* set crc */
        input_buf[5] = 0x7E;                                                                                   /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                               /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 20, (uint8_t *)out_buf, 7);             /* write read frame */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                /* write read failed */
           
            return 1;                                                                                          /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                             /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                  /* crc check error */
           
            return 1;                                                                                          /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                       /* check status */
        {
            return 1;                                                                                          /* return error */
        }
    }
    else                                                                                                       /* iic */
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_STOP_MEASUREMENT, NULL, 0, 20);       /* stop measurement command */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: stop measurement failed.\n");                                          /* stop measurement failed */
           
            return 1;                                                                                          /* return error */
        }
    }
        
    return 0;                                                                                                  /* success return 0 */
}

/**
 * @brief      read the data read flag
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *flag points to a data ready flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 read data read flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_read_data_flag(sps30_handle_t *handle, sps30_data_ready_flag_t *flag)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                                    /* check handle */
    {
        return 2;                                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                                               /* check handle initialization */
    {
        return 3;                                                                                                          /* return error */
    }

    if (handle->iic_uart != 0)                                                                                             /* uart */
    {
        handle->debug_print("sps30: uart has no command.\n");                                                              /* uart has no command */
         
        return 1;                                                                                                          /* return error */
    }
    else                                                                                                                   /* iic */
    {
        uint8_t buf[3];
        
        memset(buf, 0, sizeof(uint8_t) * 3);                                                                               /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_DATA_READY_FLAG, (uint8_t *)buf, 3, 0);       /* read data ready flag command */
        if (res != 0)                                                                                                      /* check result */
        {
            handle->debug_print("sps30: read data ready flag failed.\n");                                                  /* read data ready flag failed */
           
            return 1;                                                                                                      /* return error */
        }
        if (buf[2] != a_sps30_generate_crc(handle, (uint8_t *)buf, 2))                                                     /* check crc */
        {
            handle->debug_print("sps30: crc check failed.\n");                                                             /* crc check failed */
           
            return 1;                                                                                                      /* return error */
        }
        *flag = (sps30_data_ready_flag_t)(buf[1] & 0x01);                                                                  /* get the data ready flag */
    }
        
    return 0;                                                                                                              /* success return 0 */
}

/**
 * @brief     enter the sleep mode
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_sleep(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (handle->iic_uart != 0)                                                                                 /* uart */
    {
        uint8_t input_buf[6 + 0];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                                   /* set start */
        input_buf[1] = 0x00;                                                                                   /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_SLEEP;                                                               /* set command */
        input_buf[3] = 0x00;                                                                                   /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                              /* set crc */
        input_buf[5] = 0x7E;                                                                                   /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                               /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 5, (uint8_t *)out_buf, 7);              /* write read frame */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                /* write read failed */
           
            return 1;                                                                                          /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                             /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                  /* crc check error */
           
            return 1;                                                                                          /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                       /* check status */
        {
            return 1;                                                                                          /* return error */
        }
    }
    else                                                                                                       /* iic */
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_SLEEP, NULL, 0, 5);                   /* sleep command */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: sleep failed.\n");                                                     /* sleep failed */
           
            return 1;                                                                                          /* return error */
        }
    }
        
    return 0;                                                                                                  /* success return 0 */
}

/**
 * @brief     wake up the chip
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_wake_up(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                        /* check handle */
    {
        return 2;                                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                                   /* check handle initialization */
    {
        return 3;                                                                                              /* return error */
    }

    if (handle->iic_uart != 0)                                                                                 /* uart */
    {
        uint8_t input_buf[6 + 0];
        uint8_t out_buf[7];
        uint8_t wake_up = 0xFF;
        
        input_buf[0] = 0x7E;                                                                                   /* set start */
        input_buf[1] = 0x00;                                                                                   /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_WAKE_UP;                                                             /* set command */
        input_buf[3] = 0x00;                                                                                   /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                              /* set crc */
        input_buf[5] = 0x7E;                                                                                   /* set stop */
        if (handle->uart_write((uint8_t *)&wake_up, 1) != 0)                                                   /* write data */
        {
            return 1;                                                                                          /* return error */
        }
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                               /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 100, (uint8_t *)out_buf, 7);            /* write read frame */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                /* write read failed */
           
            return 1;                                                                                          /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                             /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                  /* crc check error */
           
            return 1;                                                                                          /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                       /* check status */
        {
            return 1;                                                                                          /* return error */
        }
    }
    else                                                                                                       /* iic */
    {
        (void)a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_WAKE_UP, NULL, 0, 0);                 /* wake up command */
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_WAKE_UP, NULL, 0, 100);               /* wake up command */
        if (res != 0)                                                                                          /* check result */
        {
            handle->debug_print("sps30: wake up failed.\n");                                                   /* wake up failed */
           
            return 1;                                                                                          /* return error */
        }
    }
        
    return 0;                                                                                                  /* success return 0 */
}

/**
 * @brief     start the fan cleaing
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start fan cleaning failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_start_fan_cleaning(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                         /* check handle */
    {
        return 2;                                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                                    /* check handle initialization */
    {
        return 3;                                                                                               /* return error */
    }

    if (handle->iic_uart != 0)                                                                                  /* uart */
    {
        uint8_t input_buf[6 + 0];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                                    /* set start */
        input_buf[1] = 0x00;                                                                                    /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_START_FAN_CLEANING;                                                   /* set command */
        input_buf[3] = 0x00;                                                                                    /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                               /* set crc */
        input_buf[5] = 0x7E;                                                                                    /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 20, (uint8_t *)out_buf, 7);              /* write read frame */
        if (res != 0)                                                                                           /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                 /* write read failed */
           
            return 1;                                                                                           /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                              /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                   /* crc check error */
           
            return 1;                                                                                           /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                        /* check status */
        {
            return 1;                                                                                           /* return error */
        }
    }
    else                                                                                                        /* iic */
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_START_FAN_CLEANING, NULL, 0, 5);       /* start the fan cleaing command */
        if (res != 0)                                                                                           /* check result */
        {
            handle->debug_print("sps30: start the fan cleaing failed.\n");                                      /* start the fan cleaing failed */
           
            return 1;                                                                                           /* return error */
        }
    }
        
    return 0;                                                                                                   /* success return 0 */
}

/**
 * @brief     set the auto cleaning interval
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] second is the interval
 * @return    status code
 *            - 0 success
 *            - 1 set auto cleaning interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 second is invlaid
 * @note      10 < second < 604800
 */
uint8_t sps30_set_auto_cleaning_interval(sps30_handle_t *handle, uint32_t second)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                          /* check handle */
    {
        return 2;                                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                                     /* check handle initialization */
    {
        return 3;                                                                                                /* return error */
    }
    if ((second < 10) || (second > 604800))                                                                      /* check second */
    {
        handle->debug_print("sps30: second is invlaid.\n");                                                      /* second is invlaid */
           
        return 4;                                                                                                /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                   /* uart */
    {
        uint8_t input_buf[6 + 5];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                                     /* set start */
        input_buf[1] = 0x00;                                                                                     /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL;                                     /* set command */
        input_buf[3] = 0x05;                                                                                     /* set length */
        input_buf[4] = 0x00;                                                                                     /* set 0x00 */
        input_buf[5] = (second >> 24) & 0xFF;                                                                    /* set 32 - 24 bits */
        input_buf[6] = (second >> 16) & 0xFF;                                                                    /* set 24 - 16 bits */
        input_buf[7] = (second >> 8) & 0xFF;                                                                     /* set 16 - 8 bits */
        input_buf[8] = (second >> 0) & 0xFF;                                                                     /* set 8 - 0 bits */
        input_buf[9] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 8);                                /* set crc */
        input_buf[10] = 0x7E;                                                                                    /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                 /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 11, 20, (uint8_t *)out_buf, 7);              /* write read frame */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                  /* write read failed */
           
            return 1;                                                                                            /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                               /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                    /* crc check error */
           
            return 1;                                                                                            /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                         /* check status */
        {
            return 1;                                                                                            /* return error */
        }
    }
    else                                                                                                         /* iic */
    {
        uint8_t buf[6];
        
        buf[0] = (second >> 24) & 0xFF;                                                                          /* set byte 4 */
        buf[1] = (second >> 16) & 0xFF;                                                                          /* set byte 3 */
        buf[2] = a_sps30_generate_crc(handle, (uint8_t *)&buf[0], 2);                                            /* set crc */
        buf[3] = (second >> 8) & 0xFF;                                                                           /* set byte 2 */
        buf[4] = (second >> 0) & 0xFF;                                                                           /* set byte 1 */
        buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&buf[3], 2);                                            /* set crc */
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL, 
                               (uint8_t *)buf, 6, 20);                                                           /* set auto cleaning interval command */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: set auto cleaning interval failed.\n");                                  /* set auto cleaning interval failed */
           
            return 1;                                                                                            /* return error */
        }
    }
        
    return 0;                                                                                                    /* success return 0 */
}

/**
 * @brief      get the auto cleaning interval
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *second points to a interval buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto cleaning interval failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_auto_cleaning_interval(sps30_handle_t *handle, uint32_t *second)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                          /* check handle */
    {
        return 2;                                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                                     /* check handle initialization */
    {
        return 3;                                                                                                /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                   /* uart */
    {
        uint8_t input_buf[6 + 1];
        uint8_t out_buf[7 + 4];
        
        input_buf[0] = 0x7E;                                                                                     /* set start */
        input_buf[1] = 0x00;                                                                                     /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL;                                     /* set command */
        input_buf[3] = 0x01;                                                                                     /* set length */
        input_buf[4] = 0x00;                                                                                     /* set length */
        input_buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 4);                                /* set crc */
        input_buf[6] = 0x7E;                                                                                     /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 11);                                                                /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 7, 20, (uint8_t *)out_buf, 11);              /* write read frame */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                  /* write read failed */
           
            return 1;                                                                                            /* return error */
        }
        if (out_buf[9] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 8))                               /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                    /* crc check error */
           
            return 1;                                                                                            /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                         /* check status */
        {
            return 1;                                                                                            /* return error */
        }
        *second = ((uint32_t)out_buf[5] << 24) | ((uint32_t)out_buf[6] << 16) | 
                  ((uint32_t)out_buf[7] << 8) | ((uint32_t)out_buf[8] << 0);                                     /* get second */
    }
    else                                                                                                         /* iic */
    {
        uint8_t buf[6];
        
        memset(buf, 0, sizeof(uint8_t) * 6);                                                                     /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL, 
                              (uint8_t *)buf, 6, 20);                                                            /* get auto cleaning interval command */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: get auto cleaning interval failed.\n");                                  /* get auto cleaning interval failed */
           
            return 1;                                                                                            /* return error */
        }
        if (buf[2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[0], 2))                                       /* check crc */
        {
            handle->debug_print("sps30: crc is error.\n");                                                       /* crc is error */
           
            return 1;                                                                                            /* return error */
        }
        if (buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&buf[3], 2))                                       /* check crc */
        {
            handle->debug_print("sps30: crc is error.\n");                                                       /* crc is error */
           
            return 1;                                                                                            /* return error */
        }
        *second = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | 
                  ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 0);                                             /* get second */
    }
        
    return 0;                                                                                                    /* success return 0 */
}

/**
 * @brief     disable the auto cleaning interval
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 disable auto cleaning interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_disable_auto_cleaning_interval(sps30_handle_t *handle)
{
    uint8_t res;
    uint32_t second;
    
    if (handle == NULL)                                                                                          /* check handle */
    {
        return 2;                                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                                     /* check handle initialization */
    {
        return 3;                                                                                                /* return error */
    }
    
    if (handle->iic_uart != 0)                                                                                   /* uart */
    {
        uint8_t input_buf[6 + 5];
        uint8_t out_buf[7];
        
        second = 0;                                                                                              /* disable second */
        input_buf[0] = 0x7E;                                                                                     /* set start */
        input_buf[1] = 0x00;                                                                                     /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL;                                     /* set command */
        input_buf[3] = 0x05;                                                                                     /* set length */
        input_buf[4] = 0x00;                                                                                     /* set length */
        input_buf[5] = (second >> 24) & 0xFF;                                                                    /* set 32 - 24 bits */
        input_buf[6] = (second >> 16) & 0xFF;                                                                    /* set 24 - 16 bits */
        input_buf[7] = (second >> 8) & 0xFF;                                                                     /* set 16 - 8 bits */
        input_buf[8] = (second >> 0) & 0xFF;                                                                     /* set 8 - 0 bits */
        input_buf[9] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 8);                                /* set crc */
        input_buf[10] = 0x7E;                                                                                    /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                                 /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 11, 20, (uint8_t *)out_buf, 7);              /* write read frame */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                  /* write read failed */
           
            return 1;                                                                                            /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                               /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                    /* crc check error */
           
            return 1;                                                                                            /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                         /* check status */
        {
            return 1;                                                                                            /* return error */
        }
    }
    else                                                                                                         /* iic */
    {
        uint8_t buf[6];
        
        second = 0;                                                                                              /* disable second */
        buf[0] = (second >> 24) & 0xFF;                                                                          /* set byte 4 */
        buf[1] = (second >> 16) & 0xFF;                                                                          /* set byte 3 */
        buf[2] = a_sps30_generate_crc(handle, (uint8_t *)&buf[0], 2);                                            /* set crc */
        buf[3] = (second >> 8) & 0xFF;                                                                           /* set byte 2 */
        buf[4] = (second >> 0) & 0xFF;                                                                           /* set byte 1 */
        buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&buf[3], 2);                                            /* set crc */
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_WRITE_AUTO_CLEANING_INTERVAL, 
                               (uint8_t *)buf, 6, 20);                                                           /* set auto cleaning interval command */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: set auto cleaning interval failed.\n");                                  /* set auto cleaning interval failed */
           
            return 1;                                                                                            /* return error */
        }
    }
        
    return 0;                                                                                                    /* success return 0 */
}

/**
 * @brief      get the product type
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *type points to a product type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_product_type(sps30_handle_t *handle, char type[9])
{
    uint8_t res, i, j;
    
    if (handle == NULL)                                                                                                   /* check handle */
    {
        return 2;                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                         /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                            /* uart */
    {
        uint8_t input_buf[6 + 1];
        uint8_t out_buf[7 + 9];
        
        input_buf[0] = 0x7E;                                                                                              /* set start */
        input_buf[1] = 0x00;                                                                                              /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_PRODUCT_TYPE;                                                              /* set command */
        input_buf[3] = 0x01;                                                                                              /* set length */
        input_buf[4] = 0x00;                                                                                              /* set length */
        input_buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 4);                                         /* set crc */
        input_buf[6] = 0x7E;                                                                                              /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 16);                                                                         /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 7, 10, (uint8_t *)out_buf, 16);                       /* write read frame */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                           /* write read failed */
           
            return 1;                                                                                                     /* return error */
        }
        if (out_buf[14] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 13))                                      /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                             /* crc check error */
           
            return 1;                                                                                                     /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                                  /* check status */
        {
            return 1;                                                                                                     /* return error */
        }
        memcpy((uint8_t *)type, (uint8_t *)&out_buf[5], 9);                                                               /* copy data*/
    }
    else                                                                                                                  /* iic */
    {
        uint8_t buf[12];
        
        memset(buf, 0, sizeof(uint8_t) * 12);                                                                             /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_PRODUCT_TYPE, (uint8_t *)buf, 12, 0);        /* read product type command */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: read product type failed.\n");                                                    /* read product type failed */
           
            return 1;                                                                                                     /* return error */
        }
        for (i = 0; i < 4; i++)                                                                                           /* check crc */
        {
            if (buf[i * 3 + 2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[i * 3], 2))                                /* check crc */
            {
                handle->debug_print("sps30: crc is error.\n");                                                            /* crc is error */
               
                return 1;                                                                                                 /* return error */
            }
        }
        j = 0;                                                                                                            /* set zero */
        for (i = 0; i < 4; i++)                                                                                           /* copy type */
        {
            type[j] = buf[i * 3 + 0];                                                                                     /* set type */
            j++;                                                                                                          /* j++ */
            type[j] = buf[i * 3 + 1];                                                                                     /* set type */
            j++;                                                                                                          /* j++ */
        }
        type[j] = 0;                                                                                                      /* set type */
    }
        
    return 0;                                                                                                             /* success return 0 */
}

/**
 * @brief      get the serial number
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *sn points to a serial number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_serial_number(sps30_handle_t *handle, char sn[17])
{
    uint8_t res, i, j;
    
    if (handle == NULL)                                                                                                   /* check handle */
    {
        return 2;                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                         /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                            /* uart */
    {
        uint8_t input_buf[6 + 1];
        uint8_t out_buf[7 + 17];
        
        input_buf[0] = 0x7E;                                                                                              /* set start */
        input_buf[1] = 0x00;                                                                                              /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_PRODUCT_TYPE;                                                              /* set command */
        input_buf[3] = 0x01;                                                                                              /* set length */
        input_buf[4] = 0x03;                                                                                              /* set command */
        input_buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 4);                                         /* set crc */
        input_buf[6] = 0x7E;                                                                                              /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 24);                                                                         /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 7, 20, (uint8_t *)out_buf, 24);                       /* write read frame */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                           /* write read failed */
           
            return 1;                                                                                                     /* return error */
        }
        if (out_buf[22] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 21))                                      /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                             /* crc check error */
           
            return 1;                                                                                                     /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                                  /* check status */
        {
            return 1;                                                                                                     /* return error */
        }
        memcpy((uint8_t *)sn, (uint8_t *)&out_buf[5], 17);                                                                /* copy data*/
    }
    else                                                                                                                  /* iic */
    {
        uint8_t buf[24];
        
        memset(buf, 0, sizeof(uint8_t) * 24);                                                                             /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_SERIAL_NUMBER, (uint8_t *)buf, 24, 0);       /* read serial number command */
        if (res != 0)                                                                                                     /* check result */
        {
            handle->debug_print("sps30: read serial number failed.\n");                                                   /* read serial number failed */
           
            return 1;                                                                                                     /* return error */
        }
        for (i = 0; i < 8; i++)                                                                                           /* check crc */
        {
            if (buf[i * 3 + 2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[i * 3], 2))                                /* check crc */
            {
                handle->debug_print("sps30: crc is error.\n");                                                            /* crc is error */
               
                return 1;                                                                                                 /* return error */
            }
        }
        j = 0;                                                                                                            /* set zero */
        for (i = 0; i < 8; i++)                                                                                           /* copy serial number */
        {
            sn[j] = buf[i * 3 + 0];                                                                                       /* set serial number */
            j++;                                                                                                          /* j++ */
            sn[j] = buf[i * 3 + 1];                                                                                       /* set serial number */
            j++;                                                                                                          /* j++ */
        }
        sn[j] = 0;                                                                                                        /* set NULL */
    }
    
    return 0;                                                                                                             /* success return 0 */
}

/**
 * @brief      get the version
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *major points to a major buffer
 * @param[out] *minor points to a minor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_version(sps30_handle_t *handle, uint8_t *major, uint8_t *minor)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                            /* check handle */
    {
        return 2;                                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                                       /* check handle initialization */
    {
        return 3;                                                                                                  /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                     /* uart */
    {
        uint8_t input_buf[6];
        uint8_t out_buf[7 + 7];
        
        input_buf[0] = 0x7E;                                                                                       /* set start */
        input_buf[1] = 0x00;                                                                                       /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_VERSION;                                                            /* set command */
        input_buf[3] = 0x00;                                                                                       /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                                  /* set crc */
        input_buf[5] = 0x7E;                                                                                       /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 14);                                                                  /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 20, (uint8_t *)out_buf, 14);                /* write read frame */
        if (res != 0)                                                                                              /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                    /* write read failed */
           
            return 1;                                                                                              /* return error */
        }
        if (out_buf[12] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 11))                               /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                      /* crc check error */
           
            return 1;                                                                                              /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                           /* check status */
        {
            return 1;                                                                                              /* return error */
        }
        *major = out_buf[5];                                                                                       /* set major */
        *minor = out_buf[6];                                                                                       /* set minor */
    }
    else                                                                                                           /* iic */
    {
        memset(buf, 0, sizeof(uint8_t) * 2);                                                                       /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_VERSION, (uint8_t *)buf, 3, 0);       /* read version command */
        if (res != 0)                                                                                              /* check result */
        {
            handle->debug_print("sps30: read version failed.\n");                                                  /* read version failed */
           
            return 1;                                                                                              /* return error */
        }
        if (buf[2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[0], 2))                                         /* check crc */
        {
            handle->debug_print("sps30: crc is error.\n");                                                         /* crc is error */
           
            return 1;                                                                                              /* return error */
        }
        *major = buf[0];                                                                                           /* set major */
        *minor = buf[1];                                                                                           /* set minor */
    }
        
    return 0;                                                                                                      /* success return 0 */
}

/**
 * @brief      get the device status
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_device_status(sps30_handle_t *handle, uint32_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                          /* check handle */
    {
        return 2;                                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                                     /* check handle initialization */
    {
        return 3;                                                                                                /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                   /* uart */
    {
        uint8_t input_buf[6 + 1];
        uint8_t out_buf[7 + 5];
        
        input_buf[0] = 0x7E;                                                                                     /* set start */
        input_buf[1] = 0x00;                                                                                     /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_DEVICE_STATUS_REG;                                                /* set command */
        input_buf[3] = 0x01;                                                                                     /* set length */
        input_buf[4] = 0x00;                                                                                     /* set 0x00 */
        input_buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 4);                                /* set crc */
        input_buf[6] = 0x7E;                                                                                     /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 12);                                                                /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 7, 20, (uint8_t *)out_buf, 12);              /* write read frame */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                  /* write read failed */
           
            return 1;                                                                                            /* return error */
        }
        if (out_buf[10] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 9))                              /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                    /* crc check error */
           
            return 1;                                                                                            /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                         /* check status */
        {
            return 1;                                                                                            /* return error */
        }
        *status = ((uint32_t)out_buf[5] << 24) | ((uint32_t)out_buf[6] << 16) | 
                  ((uint32_t)out_buf[7] << 8) | ((uint32_t)out_buf[8] << 0);                                     /* get status */
    }
    else                                                                                                         /* iic */
    {
        uint8_t buf[6];
        
        memset(buf, 0, sizeof(uint8_t) * 6);                                                                     /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_DEVICE_STATUS_REG, 
                              (uint8_t *)buf, 6, 0);                                                             /* get device status command */
        if (res != 0)                                                                                            /* check result */
        {
            handle->debug_print("sps30: get device status failed.\n");                                           /* get device status failed */
           
            return 1;                                                                                            /* return error */
        }
        if (buf[2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[0], 2))                                       /* check crc */
        {
            handle->debug_print("sps30: crc is error.\n");                                                       /* crc is error */
           
            return 1;                                                                                            /* return error */
        }
        if (buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&buf[3], 2))                                       /* check crc */
        {
            handle->debug_print("sps30: crc is error.\n");                                                       /* crc is error */
           
            return 1;                                                                                            /* return error */
        }
        *status = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | 
                  ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 0);                                             /* get status */
    }
        
    return 0;                                                                                                    /* success return 0 */
}

/**
 * @brief     clear the device status
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear device status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_clear_device_status(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                                              /* check handle */
    {
        return 2;                                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                                         /* check handle initialization */
    {
        return 3;                                                                                                    /* return error */
    }

    if (handle->iic_uart != 0)                                                                                       /* uart */
    {
        uint8_t input_buf[6 + 1];
        uint8_t out_buf[7 + 5];
        
        input_buf[0] = 0x7E;                                                                                         /* set start */
        input_buf[1] = 0x00;                                                                                         /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_READ_DEVICE_STATUS_REG;                                                    /* set command */
        input_buf[3] = 0x01;                                                                                         /* set length */
        input_buf[4] = 0x01;                                                                                         /* set 0x01 */
        input_buf[5] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 4);                                    /* set crc */
        input_buf[6] = 0x7E;                                                                                         /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 12);                                                                    /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 7, 20, (uint8_t *)out_buf, 12);                  /* write read frame */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                                      /* write read failed */
           
            return 1;                                                                                                /* return error */
        }
        if (out_buf[10] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 9))                                  /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                                        /* crc check error */
           
            return 1;                                                                                                /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                             /* check status */
        {
            return 1;                                                                                                /* return error */
        }
    }
    else                                                                                                             /* iic */
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_CLEAR_DEVICE_STATUS_REG, NULL, 0, 5);       /* clear device status command */
        if (res != 0)                                                                                                /* check result */
        {
            handle->debug_print("sps30: clear device status failed.\n");                                             /* clear device status failed */
           
            return 1;                                                                                                /* return error */
        }
    }
        
    return 0;                                                                                                        /* success return 0 */
}

/**
 * @brief     reset the chip
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_reset(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }

    if (handle->iic_uart != 0)                                                                       /* uart */
    {
        uint8_t input_buf[6];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                         /* set start */
        input_buf[1] = 0x00;                                                                         /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_RESET;                                                     /* set command */
        input_buf[3] = 0x00;                                                                         /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                    /* set crc */
        input_buf[5] = 0x7E;                                                                         /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                     /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 100, (uint8_t *)out_buf, 7);  /* write read frame */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                      /* write read failed */
           
            return 1;                                                                                /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                   /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                        /* crc check error */
           
            return 1;                                                                                /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                             /* check status */
        {
            return 1;                                                                                /* return error */
        }
    }
    else                                                                                             /* iic */
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_RESET, NULL, 0, 100);       /* reset command */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: reset failed.\n");                                           /* reset failed */
           
            return 1;                                                                                /* return error */
        }
    }
        
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      read the result
 * @param[in]  *handle points to a sps30 handle structure
 * @param[out] *pm points to a sps30 pm structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mode is invalid
 * @note       none
 */
uint8_t sps30_read(sps30_handle_t *handle, sps30_pm_t *pm)
{
    union float_u
    {
        float f;
        uint32_t i;
    };
    uint8_t res, i;
    union float_u f;
    
    if ((handle == NULL) || (pm == NULL))                                                                                       /* check handle */
    {
        return 2;                                                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                                                    /* check handle initialization */
    {
        return 3;                                                                                                               /* return error */
    }

    
    if (handle->iic_uart != 0)                                                                                                  /* uart */
    {
        if (handle->format == SPS30_FORMAT_IEEE754)                                                                             /* float */
        {
            uint8_t input_buf[6];
            uint8_t out_buf[7 + 40];
            
            input_buf[0] = 0x7E;                                                                                                /* set start */
            input_buf[1] = 0x00;                                                                                                /* set addr */
            input_buf[2] = SPS30_UART_COMMAND_READ_MEASURED_VALUES;                                                             /* set command */
            input_buf[3] = 0x00;                                                                                                /* set length */
            input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                                           /* set crc */
            input_buf[5] = 0x7E;                                                                                                /* set stop */
            memset(out_buf, 0, sizeof(uint8_t) * 47);                                                                           /* clear the buffer */
            res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 20, (uint8_t *)out_buf, 47);                         /* write read frame */
            if (res != 0)                                                                                                       /* check result */
            {
                handle->debug_print("sps30: write read failed.\n");                                                             /* write read failed */
               
                return 1;                                                                                                       /* return error */
            }
            if (out_buf[45] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 44))                                        /* check crc */
            {
                handle->debug_print("sps30: crc check error.\n");                                                               /* crc check error */
               
                return 1;                                                                                                       /* return error */
            }
            if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                                    /* check status */
            {
                return 1;                                                                                                       /* return error */
            }
            f.i = (uint32_t)(out_buf[5 + 0]) << 24 | (uint32_t)(out_buf[5 + 1]) << 16 |
                  (uint32_t)(out_buf[5 + 2]) << 8 | (uint32_t)(out_buf[5 + 3]) << 0;                                            /* copy data */
            pm->pm1p0_ug_m3 = f.f;                                                                                              /* copy pm1.0 ug/m3 */
            f.i = (uint32_t)(out_buf[5 + 4]) << 24 | (uint32_t)(out_buf[5 + 5]) << 16 |
                  (uint32_t)(out_buf[5 + 6]) << 8 | (uint32_t)(out_buf[5 + 7]) << 0;                                            /* copy data */
            pm->pm2p5_ug_m3 = f.f;                                                                                              /* copy pm2.5 ug/m3 */
            f.i = (uint32_t)(out_buf[5 + 8]) << 24 | (uint32_t)(out_buf[5 + 9]) << 16 |
                  (uint32_t)(out_buf[5 + 10]) << 8 | (uint32_t)(out_buf[5 + 11]) << 0;                                          /* copy data */
            pm->pm4p0_ug_m3 = f.f;                                                                                              /* copy pm4.0 ug/m3 */
            f.i = (uint32_t)(out_buf[5 + 12]) << 24 | (uint32_t)(out_buf[5 + 13]) << 16 |
                  (uint32_t)(out_buf[5 + 14]) << 8 | (uint32_t)(out_buf[5 + 15]) << 0;                                          /* copy data */
            pm->pm10_ug_m3 = f.f;                                                                                               /* copy pm10.0 ug/m3 */
            f.i = (uint32_t)(out_buf[5 + 16]) << 24 | (uint32_t)(out_buf[5 + 17]) << 16 |
                  (uint32_t)(out_buf[5 + 18]) << 8 | (uint32_t)(out_buf[5 + 19]) << 0;                                          /* copy data */
            pm->pm0p5_cm3 = f.f;                                                                                                /* copy pm0.5 cm3 */
            f.i = (uint32_t)(out_buf[5 + 20]) << 24 | (uint32_t)(out_buf[5 + 21]) << 16 |
                  (uint32_t)(out_buf[5 + 22]) << 8 | (uint32_t)(out_buf[5 + 23]) << 0;                                          /* copy data */
            pm->pm1p0_cm3 = f.f;                                                                                                /* copy pm1.0 cm3 */
            f.i = (uint32_t)(out_buf[5 + 24]) << 24 | (uint32_t)(out_buf[5 + 25]) << 16 |
                  (uint32_t)(out_buf[5 + 26]) << 8 | (uint32_t)(out_buf[5 + 27]) << 0;                                          /* copy data */
            pm->pm2p5_cm3 = f.f;                                                                                                /* copy pm2.5 cm3 */
            f.i = (uint32_t)(out_buf[5 + 28]) << 24 | (uint32_t)(out_buf[5 + 29]) << 16 |
                  (uint32_t)(out_buf[5 + 30]) << 8 | (uint32_t)(out_buf[5 + 31]) << 0;                                          /* copy data */
            pm->pm4p0_cm3 = f.f;                                                                                                /* copy pm4.0 cm3 */
            f.i = (uint32_t)(out_buf[5 + 32]) << 24 | (uint32_t)(out_buf[5 + 33]) << 16 |
                  (uint32_t)(out_buf[5 + 34]) << 8 | (uint32_t)(out_buf[5 + 35]) << 0;                                          /* copy data */
            pm->pm10_cm3 = f.f;                                                                                                 /* copy pm10.0 cm3 */
            f.i = (uint32_t)(out_buf[5 + 36]) << 24 | (uint32_t)(out_buf[5 + 37]) << 16 |
                  (uint32_t)(out_buf[5 + 38]) << 8 | (uint32_t)(out_buf[5 + 39]) << 0;                                          /* copy data */
            pm->typical_particle_um = f.f;                                                                                      /* copy typical particle um */
        }
        else if (handle->format == SPS30_FORMAT_UINT16)                                                                         /* uint16 */
        {
            uint8_t input_buf[6];
            uint8_t out_buf[7 + 20];
            
            input_buf[0] = 0x7E;                                                                                                /* set start */
            input_buf[1] = 0x00;                                                                                                /* set addr */
            input_buf[2] = SPS30_UART_COMMAND_READ_MEASURED_VALUES;                                                             /* set command */
            input_buf[3] = 0x00;                                                                                                /* set length */
            input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                                           /* set crc */
            input_buf[5] = 0x7E;                                                                                                /* set stop */
            memset(out_buf, 0, sizeof(uint8_t) * 27);                                                                           /* clear the buffer */
            res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 20, (uint8_t *)out_buf, 27);                         /* write read frame */
            if (res != 0)                                                                                                       /* check result */
            {
                handle->debug_print("sps30: write read failed.\n");                                                             /* write read failed */
               
                return 1;                                                                                                       /* return error */
            }
            if (out_buf[25] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 24))                                        /* check crc */
            {
                handle->debug_print("sps30: crc check error.\n");                                                               /* crc check error */
               
                return 1;                                                                                                       /* return error */
            }
            if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                                                    /* check status */
            {
                return 1;                                                                                                       /* return error */
            }
            pm->pm1p0_ug_m3 = (float)(((uint16_t)(out_buf[5 + 0]) << 8) | ((uint16_t)(out_buf[5 + 1]) << 0));                   /* copy pm1.0 ug/m3 */
            pm->pm2p5_ug_m3 = (float)(((uint16_t)(out_buf[5 + 2]) << 8) | ((uint16_t)(out_buf[5 + 3]) << 0));                   /* copy pm2.5 ug/m3 */
            pm->pm4p0_ug_m3 = (float)(((uint16_t)(out_buf[5 + 4]) << 8) | ((uint16_t)(out_buf[5 + 5]) << 0));                   /* copy pm4.0 ug/m3 */
            pm->pm10_ug_m3 = (float)(((uint16_t)(out_buf[5 + 6]) << 8) | ((uint16_t)(out_buf[5 + 7]) << 0));                    /* copy pm10 ug/m3 */
            pm->pm0p5_cm3 = (float)(((uint16_t)(out_buf[5 + 8]) << 8) | ((uint16_t)(out_buf[5 + 9]) << 0));                     /* copy pm0.5 cm3 */
            pm->pm1p0_cm3 = (float)(((uint16_t)(out_buf[5 + 10]) << 8) | ((uint16_t)(out_buf[5 + 11]) << 0));                   /* copy pm1.0 cm3 */
            pm->pm2p5_cm3 = (float)(((uint16_t)(out_buf[5 + 12]) << 8) | ((uint16_t)(out_buf[5 + 13]) << 0));                   /* copy pm2.5 cm3 */
            pm->pm4p0_cm3 = (float)(((uint16_t)(out_buf[5 + 14]) << 8) | ((uint16_t)(out_buf[5 + 15]) << 0));                   /* copy pm4.0 cm3 */
            pm->pm10_cm3 = (float)(((uint16_t)(out_buf[5 + 16]) << 8) | ((uint16_t)(out_buf[5 + 17]) << 0));                    /* copy pm10 cm3 */
            pm->typical_particle_um = (float)(((uint16_t)(out_buf[5 + 18]) << 8) | ((uint16_t)(out_buf[5 + 19]) << 0));         /* copy typical particle */
            pm->typical_particle_um /= 1000.0f;                                                                                 /* div 1000 */
        }
        else
        {
            handle->debug_print("sps30: mode is invalid.\n");                                                                   /* mode is invalid */
           
            return 4;                                                                                                           /* return error */
        }
    }
    else                                                                                                                        /* iic */
    {
        uint8_t check[3];
        
        memset(check, 0, sizeof(uint8_t) * 3);                                                                                  /* clear the buffer */
        res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_DATA_READY_FLAG, (uint8_t *)check, 3, 0);          /* read data ready flag command */
        if (res != 0)                                                                                                           /* check result */
        {
            handle->debug_print("sps30: read data ready flag failed.\n");                                                       /* read data ready flag failed */
           
            return 1;                                                                                                           /* return error */
        }
        if (check[2] != a_sps30_generate_crc(handle, (uint8_t *)check, 2))                                                      /* check crc */
        {
            handle->debug_print("sps30: crc check failed.\n");                                                                  /* crc check failed */
           
            return 1;                                                                                                           /* return error */
        }
        if ((check[1] & 0x01) == 0)                                                                                             /* check flag */
        {
            handle->debug_print("sps30: data not ready.\n");                                                                    /* data not ready */
           
            return 1;                                                                                                           /* return error */
        }
        
        if (handle->format == SPS30_FORMAT_IEEE754)                                                                             /* float */
        {
            uint8_t buf[60];
            
            memset(buf, 0, sizeof(uint8_t) * 60);                                                                               /* clear the buffer */
            res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_MEASURED_VALUES, (uint8_t *)buf, 60, 0);       /* read measured values command */
            if (res != 0)                                                                                                       /* check result */
            {
                handle->debug_print("sps30: read measured values failed.\n");                                                   /* read measured values failed */
               
                return 1;                                                                                                       /* return error */
            }
            for (i = 0; i < 20; i++)                                                                                            /* check crc */
            {
                if (buf[i * 3 + 2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[i * 3], 2))                                  /* check crc */
                {
                    handle->debug_print("sps30: crc is error.\n");                                                              /* crc is error */
                   
                    return 1;                                                                                                   /* return error */
                }
            }
            f.i = (uint32_t)(buf[0]) << 24 | (uint32_t)(buf[1]) << 16 |
                  (uint32_t)(buf[3]) << 8 | (uint32_t)(buf[4]) << 0;                                                            /* copy data */
            pm->pm1p0_ug_m3 = f.f;                                                                                              /* copy pm1.0 ug/m3 */
            f.i = (uint32_t)(buf[6]) << 24 | (uint32_t)(buf[7]) << 16 |
                  (uint32_t)(buf[9]) << 8 | (uint32_t)(buf[10]) << 0;                                                           /* copy data */
            pm->pm2p5_ug_m3 = f.f;                                                                                              /* copy pm2.5 ug/m3 */
            f.i = (uint32_t)(buf[12]) << 24 | (uint32_t)(buf[13]) << 16 |
                  (uint32_t)(buf[15]) << 8 | (uint32_t)(buf[16]) << 0;                                                          /* copy data */
            pm->pm4p0_ug_m3 = f.f;                                                                                              /* copy pm4.0 ug/m3 */
            f.i = (uint32_t)(buf[18]) << 24 | (uint32_t)(buf[19]) << 16 |
                  (uint32_t)(buf[21]) << 8 | (uint32_t)(buf[22]) << 0;                                                          /* copy data */
            pm->pm10_ug_m3 = f.f;                                                                                               /* copy pm10.0 ug/m3 */
            f.i = (uint32_t)(buf[24]) << 24 | (uint32_t)(buf[25]) << 16 |
                  (uint32_t)(buf[27]) << 8 | (uint32_t)(buf[28]) << 0;                                                          /* copy data */
            pm->pm0p5_cm3 = f.f;                                                                                                /* copy pm0.5 cm3 */
            f.i = (uint32_t)(buf[30]) << 24 | (uint32_t)(buf[31]) << 16 |
                  (uint32_t)(buf[33]) << 8 | (uint32_t)(buf[34]) << 0;                                                          /* copy data */
            pm->pm1p0_cm3 = f.f;                                                                                                /* copy pm1.0 cm3 */
            f.i = (uint32_t)(buf[36]) << 24 | (uint32_t)(buf[37]) << 16 |
                  (uint32_t)(buf[39]) << 8 | (uint32_t)(buf[40]) << 0;                                                          /* copy data */
            pm->pm2p5_cm3 = f.f;                                                                                                /* copy pm2.5 cm3 */
            f.i = (uint32_t)(buf[42]) << 24 | (uint32_t)(buf[43]) << 16 |
                  (uint32_t)(buf[45]) << 8 | (uint32_t)(buf[46]) << 0;                                                          /* copy data */
            pm->pm4p0_cm3 = f.f;                                                                                                /* copy pm4.0 cm3 */
            f.i = (uint32_t)(buf[48]) << 24 | (uint32_t)(buf[49]) << 16 |
                  (uint32_t)(buf[51]) << 8 | (uint32_t)(buf[52]) << 0;                                                          /* copy data */
            pm->pm10_cm3 = f.f;                                                                                                 /* copy pm10.0 cm3 */
            f.i = (uint32_t)(buf[54]) << 24 | (uint32_t)(buf[55]) << 16 |
                  (uint32_t)(buf[57]) << 8 | (uint32_t)(buf[58]) << 0;                                                          /* copy data */
            pm->typical_particle_um = f.f;                                                                                      /* copy typical particle um */
        }
        else if (handle->format == SPS30_FORMAT_UINT16)                                                                         /* uint16 */
        {
            uint8_t buf[30];
            
            memset(buf, 0, sizeof(uint8_t) * 30);                                                                               /* clear the buffer */
            res = a_sps30_iic_read(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_READ_MEASURED_VALUES, (uint8_t *)buf, 30, 0);       /* read measured values command */
            if (res != 0)                                                                                                       /* check result */
            {
                handle->debug_print("sps30: read measured values failed.\n");                                                   /* read measured values failed */
               
                return 1;                                                                                                       /* return error */
            }
            for (i = 0; i < 10; i++)                                                                                            /* check crc */
            {
                if (buf[i * 3 + 2] != a_sps30_generate_crc(handle, (uint8_t *)&buf[i * 3], 2))                                  /* check crc */
                {
                    handle->debug_print("sps30: crc is error.\n");                                                              /* crc is error */
                   
                    return 1;                                                                                                   /* return error */
                }
            }
            pm->pm1p0_ug_m3 = (float)(((uint16_t)(buf[0]) << 8) | ((uint16_t)(buf[1]) << 0));                                   /* copy pm1.0 ug/m3 */
            pm->pm2p5_ug_m3 = (float)(((uint16_t)(buf[3]) << 8) | ((uint16_t)(buf[4]) << 0));                                   /* copy pm2.5 ug/m3 */
            pm->pm4p0_ug_m3 = (float)(((uint16_t)(buf[6]) << 8) | ((uint16_t)(buf[7]) << 0));                                   /* copy pm4.0 ug/m3 */
            pm->pm10_ug_m3 = (float)(((uint16_t)(buf[9]) << 8) | ((uint16_t)(buf[10]) << 0));                                   /* copy pm10 ug/m3 */
            pm->pm0p5_cm3 = (float)(((uint16_t)(buf[12]) << 8) | ((uint16_t)(buf[13]) << 0));                                   /* copy pm0.5 cm3 */
            pm->pm1p0_cm3 = (float)(((uint16_t)(buf[15]) << 8) | ((uint16_t)(buf[16]) << 0));                                   /* copy pm1.0 cm3 */
            pm->pm2p5_cm3 = (float)(((uint16_t)(buf[18]) << 8) | ((uint16_t)(buf[19]) << 0));                                   /* copy pm2.5 cm3 */
            pm->pm4p0_cm3 = (float)(((uint16_t)(buf[21]) << 8) | ((uint16_t)(buf[22]) << 0));                                   /* copy pm4.0 cm3 */
            pm->pm10_cm3 = (float)(((uint16_t)(buf[24]) << 8) | ((uint16_t)(buf[25]) << 0));                                    /* copy pm10 cm3 */
            pm->typical_particle_um = (float)(((uint16_t)(buf[27]) << 8) | ((uint16_t)(buf[28]) << 0));                         /* copy typical particle */
            pm->typical_particle_um /= 1000.0f;                                                                                 /* div 1000 */
        }
        else
        {
            handle->debug_print("sps30: mode is invalid.\n");                                                                   /* mode is invalid */
           
            return 4;                                                                                                           /* return error */
        }
    }
    
    return 0;                                                                                                                   /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t sps30_init(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                                                 /* check debug_print */
    {
        return 3;                                                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                                                    /* check iic_init */
    {
        handle->debug_print("sps30: iic_init is null.\n");                                           /* iic_init is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                                                  /* check iic_deinit */
    {
        handle->debug_print("sps30: iic_deinit is null.\n");                                         /* iic_deinit is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                                               /* check iic_write_cmd */
    {
        handle->debug_print("sps30: iic_write_cmd is null.\n");                                      /* iic_write_cmd is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                                                /* check iic_read_cmd */
    {
        handle->debug_print("sps30: iic_read_cmd is null.\n");                                       /* iic_read_cmd is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->uart_init == NULL)                                                                   /* check uart_init */
    {
        handle->debug_print("sps30: uart_init is null.\n");                                          /* uart_init is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->uart_deinit == NULL)                                                                 /* check uart_deinit */
    {
        handle->debug_print("sps30: uart_deinit is null.\n");                                        /* uart_deinit is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->uart_read == NULL)                                                                   /* check uart_read */
    {
        handle->debug_print("sps30: uart_read is null.\n");                                          /* uart_read is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->uart_write == NULL)                                                                  /* check uart_write */
    {
        handle->debug_print("sps30: uart_write is null.\n");                                         /* uart_write is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->uart_flush == NULL)                                                                  /* check uart_flush */
    {
        handle->debug_print("sps30: uart_flush is null.\n");                                         /* uart_flush is null */
    
        return 3;                                                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                                                    /* check delay_ms */
    {
        handle->debug_print("sps30: delay_ms is null.\n");                                           /* delay_ms is null */
    
        return 3;                                                                                    /* return error */
    }
    
    if (handle->iic_uart != 0)
    {
        uint8_t input_buf[6];
        uint8_t out_buf[7];
        
        if (handle->uart_init() != 0)                                                                /* uart init */
        {
            handle->debug_print("sps30: uart init failed.\n");                                       /* uart init failed */
        
            return 3;                                                                                /* return error */
        }
        input_buf[0] = 0x7E;                                                                         /* set start */
        input_buf[1] = 0x00;                                                                         /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_RESET;                                                     /* set command */
        input_buf[3] = 0x00;                                                                         /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                    /* set crc */
        input_buf[5] = 0x7E;                                                                         /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                     /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 100, (uint8_t *)out_buf, 7);  /* write read frame */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                      /* write read failed */
           
            return 1;                                                                                /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                   /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                        /* crc check error */
           
            return 1;                                                                                /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                             /* check status */
        {
            return 1;                                                                                /* return error */
        }
    }
    else
    {
        if (handle->iic_init() != 0)                                                                 /* iic init */
        {
            handle->debug_print("sps30: iic init failed.\n");                                        /* iic init failed */
        
            return 3;                                                                                /* return error */
        }
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_RESET, NULL, 0, 100);       /* reset command */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: reset failed.\n");                                           /* reset failed */
           
            return 4;                                                                                /* return error */
        }
    }
    handle->inited = 1;                                                                              /* flag finish initialization */
  
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t sps30_deinit(sps30_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }    
    
    if (handle->iic_uart != 0)
    {
        uint8_t input_buf[6];
        uint8_t out_buf[7];
        
        input_buf[0] = 0x7E;                                                                         /* set start */
        input_buf[1] = 0x00;                                                                         /* set addr */
        input_buf[2] = SPS30_UART_COMMAND_RESET;                                                     /* set command */
        input_buf[3] = 0x00;                                                                         /* set length */
        input_buf[4] = a_sps30_generate_crc(handle, (uint8_t *)&input_buf[1], 3);                    /* set crc */
        input_buf[5] = 0x7E;                                                                         /* set stop */
        memset(out_buf, 0, sizeof(uint8_t) * 7);                                                     /* clear the buffer */
        res = a_sps30_uart_write_read(handle, (uint8_t *)input_buf, 6, 100, (uint8_t *)out_buf, 7);  /* write read frame */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: write read failed.\n");                                      /* write read failed */
           
            return 4;                                                                                /* return error */
        }
        if (out_buf[5] != a_sps30_generate_crc(handle, (uint8_t *)&out_buf[1], 4))                   /* check crc */
        {
            handle->debug_print("sps30: crc check error.\n");                                        /* crc check error */
           
            return 4;                                                                                /* return error */
        }
        if (a_sps30_uart_error(handle, out_buf[3]) != 0)                                             /* check status */
        {
            return 4;                                                                                /* return error */
        }
        if (handle->uart_deinit() != 0)                                                              /* uart deinit */
        {
            handle->debug_print("sps30: uart deinit failed.\n");                                     /* uart deinit failed */
        
            return 1;                                                                                /* return error */
        }
    }
    else
    {
        res = a_sps30_iic_write(handle, SPS30_ADDRESS, SPS30_IIC_COMMAND_RESET, NULL, 0, 100);       /* reset command */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: reset failed.\n");                                           /* reset failed */
           
            return 4;                                                                                /* return error */
        }
        res = handle->iic_deinit();                                                                  /* iic deinit */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("sps30: iic deinit failed.\n");                                      /* iic deinit */
           
            return 1;                                                                                /* return error */
        }
    }
    
    handle->inited = 0;                                                                              /* flag close initialization */
  
    return 0;                                                                                        /* success return 0 */
}

/**
 * @brief      set and get the chip register with uart interface
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  *input points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *output points to a output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t sps30_set_get_reg_uart(sps30_handle_t *handle, uint8_t *input, uint16_t in_len, uint8_t *output, uint16_t out_len)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    if (handle->iic_uart != 0)
    {
        return a_sps30_uart_write_read(handle, input, in_len, 20, output, out_len);       /* write and read with the uart interface */
    }
    else
    {
        handle->debug_print("sps30: iic interface is invalid.\n");                        /* iic interface is invalid */
       
        return 1;                                                                         /* return error */
    }
}

/**
 * @brief     set the chip register with iic interface
 * @param[in] *handle points to a sps30 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_set_reg_iic(sps30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    if (handle->iic_uart != 0)
    {
        handle->debug_print("sps30: uart interface is invalid.\n");               /* uart interface is invalid */
       
        return 1;                                                                 /* return error */
    }
    else
    {
        return a_sps30_iic_write(handle, SPS30_ADDRESS, reg, buf, len, 20);       /* write the data */
    }
}

/**
 * @brief      get the chip register with iic interface
 * @param[in]  *handle points to a sps30 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_reg_iic(sps30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    if (handle->iic_uart != 0)
    {
        handle->debug_print("sps30: uart interface is invalid.\n");              /* uart interface is invalid */
       
        return 1;
    }
    else
    {
        return a_sps30_iic_read(handle, SPS30_ADDRESS, reg, buf, len, 20);       /* read the data */
    }
}

/**
 * @brief      get chip information
 * @param[out] *info points to a sps30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sps30_info(sps30_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(sps30_info_t));                          /* initialize sps30 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART IIC", 16);                       /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver verison */
    
    return 0;                                                       /* success return 0 */
}
