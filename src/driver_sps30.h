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
 * @file      driver_sps30.h
 * @brief     driver sps30 header file
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

#ifndef DRIVER_SPS30_H
#define DRIVER_SPS30_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sps30_driver sps30 driver function
 * @brief    sps30 driver modules
 * @{
 */

/**
 * @addtogroup sps30_basic_driver
 * @{
 */

/**
 * @brief sps30 interface enumeration definition
 */
typedef enum
{
    SPS30_INTERFACE_IIC  = 0x00,       /**< iic interface */
    SPS30_INTERFACE_UART = 0x01,       /**< uart interface */
} sps30_interface_t;

/**
 * @brief sps30 format enumeration definition
 */
typedef enum
{
    SPS30_FORMAT_IEEE754 = 0x03,        /**< big endian IEEE754 float values */
    SPS30_FORMAT_UINT16  = 0x05,        /**< big endian unsigned 16 bit integer values */
} sps30_format_t;

/**
 * @brief sps30 data ready flag enumeration definition
 */
typedef enum
{
    SPS30_DATA_READY_FLAG_NOT_READY = 0x00,        /**< no new measurements available */
    SPS30_DATA_READY_FLAG_AVAILABLE = 0x01,        /**< new measurements ready to read */
} sps30_data_ready_flag_t;

/**
 * @brief sps30 status enumeration definition
 */
typedef enum
{
    SPS30_STATUS_FAN_SPEED_ERROR = (1 << 21),       /**< fan speed is too high or too low */
    SPS30_STATUS_LASER_ERROR     = (1 << 5),        /**< laser is switched on and current is out of range */
    SPS30_STATUS_FAN_ERROR       = (1 << 4),        /**< fan is switched on but the measured fan speed is 0 rpm */
} sps30_status_t;

/**
 * @brief sps30 handle structure definition
 */
typedef struct sps30_handle_s
{
    uint8_t (*iic_init)(void);                                                /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                              /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);       /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_read_cmd function address */
    uint8_t (*uart_init)(void);                                               /**< point to a uart_init function address */
    uint8_t (*uart_deinit)(void);                                             /**< point to a uart_deinit function address */
    uint16_t (*uart_read)(uint8_t *buf, uint16_t len);                        /**< point to a uart_read function address */
    uint8_t (*uart_flush)(void);                                              /**< point to a uart_flush function address */
    uint8_t (*uart_write)(uint8_t *buf, uint16_t len);                        /**< point to a uart_write function address */
    void (*delay_ms)(uint32_t ms);                                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                          /**< point to a debug_print function address */
    uint8_t inited;                                                           /**< inited flag */
    uint8_t iic_uart;                                                         /**< iic uart */
    uint8_t format;                                                           /**< format */
    uint8_t buf[256];                                                         /**< inner buffer */
} sps30_handle_t;

/**
 * @brief sps30 pm structure definition
 */
typedef struct sps30_pm_s
{
    float pm1p0_ug_m3;                /**< mass concentration pm1.0 [μg/m3] */
    float pm2p5_ug_m3;                /**< mass concentration pm2.5 [μg/m3] */
    float pm4p0_ug_m3;                /**< mass concentration pm4.0 [μg/m3] */
    float pm10_ug_m3;                 /**< mass concentration pm10 [μg/m³] */
    float pm0p5_cm3;                  /**< number concentration pm0.5 [#/cm3] */
    float pm1p0_cm3;                  /**< number concentration pm1.0 [#/cm3] */
    float pm2p5_cm3;                  /**< number concentration pm2.5 [#/cm3] */
    float pm4p0_cm3;                  /**< number concentration pm4.0 [#/cm3] */
    float pm10_cm3;                   /**< number concentration pm10 [#/cm3] */
    float typical_particle_um;        /**< typical particle size[um] */
} sps30_pm_t;

/**
 * @brief sps30 information structure definition
 */
typedef struct sps30_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[16];                /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} sps30_info_t;

/**
 * @}
 */

/**
 * @defgroup sps30_link_driver sps30 link driver function
 * @brief    sps30 link driver modules
 * @ingroup  sps30_driver
 * @{
 */

/**
 * @brief     initialize sps30_handle_t structure
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] STRUCTURE sps30_handle_t
 * @note      none
 */
#define DRIVER_SPS30_LINK_INIT(HANDLE, STRUCTURE)              memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link uart_init function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a uart_init function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_UART_INIT(HANDLE, FUC)              (HANDLE)->uart_init = FUC

/**
 * @brief     link uart_deinit function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a uart_deinit function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_UART_DEINIT(HANDLE, FUC)            (HANDLE)->uart_deinit = FUC

/**
 * @brief     link uart_read function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a uart_read function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_UART_READ(HANDLE, FUC)              (HANDLE)->uart_read = FUC

/**
 * @brief     link uart_write function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a uart_write function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_UART_WRITE(HANDLE, FUC)             (HANDLE)->uart_write = FUC

/**
 * @brief     link uart_flush function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a uart_flush function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_UART_FLUSH(HANDLE, FUC)             (HANDLE)->uart_flush = FUC

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_IIC_INIT(HANDLE, FUC)               (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_IIC_DEINIT(HANDLE, FUC)             (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)    (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_IIC_READ_COMMAND(HANDLE, FUC)     (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_DELAY_MS(HANDLE, FUC)               (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an sps30 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_SPS30_LINK_DEBUG_PRINT(HANDLE, FUC)            (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup sps30_basic_driver sps30 basic driver function
 * @brief    sps30 basic driver modules
 * @ingroup  sps30_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info pointer to an sps30 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sps30_info(sps30_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle pointer to an sps30 handle structure
 * @param[in] interface chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sps30_set_interface(sps30_handle_t *handle, sps30_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *interface pointer to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sps30_get_interface(sps30_handle_t *handle, sps30_interface_t *interface);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t sps30_init(sps30_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t sps30_deinit(sps30_handle_t *handle);

/**
 * @brief      read the result
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *pm pointer to an sps30 pm structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mode is invalid
 * @note       none
 */
uint8_t sps30_read(sps30_handle_t *handle, sps30_pm_t *pm);

/**
 * @brief     enter the sleep mode
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_sleep(sps30_handle_t *handle);

/**
 * @brief     reset the chip
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_reset(sps30_handle_t *handle);

/**
 * @brief     wake up the chip
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_wake_up(sps30_handle_t *handle);

/**
 * @brief     start the measurement
 * @param[in] *handle pointer to an sps30 handle structure
 * @param[in] format data format
 * @return    status code
 *            - 0 success
 *            - 1 start measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_start_measurement(sps30_handle_t *handle, sps30_format_t format);

/**
 * @brief     stop the measurement
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_stop_measurement(sps30_handle_t *handle);

/**
 * @brief      read the data read flag
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *flag pointer to a data ready flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 read data read flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_read_data_flag(sps30_handle_t *handle, sps30_data_ready_flag_t *flag);

/**
 * @brief     start the fan cleaning
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start fan cleaning failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_start_fan_cleaning(sps30_handle_t *handle);

/**
 * @brief     set the auto cleaning interval
 * @param[in] *handle pointer to an sps30 handle structure
 * @param[in] second interval
 * @return    status code
 *            - 0 success
 *            - 1 set auto cleaning interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 second is invalid
 * @note      10 < second < 604800
 */
uint8_t sps30_set_auto_cleaning_interval(sps30_handle_t *handle, uint32_t second);

/**
 * @brief      get the auto cleaning interval
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *second pointer to an interval buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto cleaning interval failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_auto_cleaning_interval(sps30_handle_t *handle, uint32_t *second);

/**
 * @brief     disable the auto cleaning interval
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 disable auto cleaning interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_disable_auto_cleaning_interval(sps30_handle_t *handle);

/**
 * @brief      get the product type
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *type pointer to a product type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_product_type(sps30_handle_t *handle, char type[9]);

/**
 * @brief      get the serial number
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *sn pointer to a serial number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_serial_number(sps30_handle_t *handle, char sn[17]);

/**
 * @brief      get the version
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *major pointer to a major buffer
 * @param[out] *minor pointer to a minor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_version(sps30_handle_t *handle, uint8_t *major, uint8_t *minor);

/**
 * @brief      get the device status
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_device_status(sps30_handle_t *handle, uint32_t *status);

/**
 * @brief     clear the device status
 * @param[in] *handle pointer to an sps30 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear device status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_clear_device_status(sps30_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup sps30_extern_driver sps30 extern driver function
 * @brief    sps30 extern driver modules
 * @ingroup  sps30_driver
 * @{
 */

/**
 * @brief     set the chip register with iic interface
 * @param[in] *handle pointer to an sps30 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sps30_set_reg_iic(sps30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register with iic interface
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sps30_get_reg_iic(sps30_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      set and get the chip register with uart interface
 * @param[in]  *handle pointer to an sps30 handle structure
 * @param[in]  *input pointer to an input buffer
 * @param[in]  in_len input length
 * @param[out] *output pointer to an output buffer
 * @param[in]  out_len output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t sps30_set_get_reg_uart(sps30_handle_t *handle, uint8_t *input, uint16_t in_len, uint8_t *output, uint16_t out_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
