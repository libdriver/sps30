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
 * @file      driver_sps30_basic.h
 * @brief     driver sps30 basic header file
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

#ifndef DRIVER_SPS30_BASIC_H
#define DRIVER_SPS30_BASIC_H

#include "driver_sps30_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sps30_example_driver sps30 example driver function
 * @brief    sps30 example driver modules
 * @ingroup  sps30_driver
 * @{
 */
 
/**
 * @brief sps30 basic example default definition
 */
#define SPS30_BASIC_DEFAULT_FORMAT                        SPS30_FORMAT_IEEE754           /**< ieee754 */
#define SPS30_BASIC_DEFAULT_AUTO_CLEANING_INTERVAL        3600 * 24                      /**< 1 day */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t sps30_basic_init(sps30_interface_t interface);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sps30_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *pm points to a sps30_pm_t structure
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sps30_basic_read(sps30_pm_t *pm);

/**
 * @brief  basic example start fan cleaning
 * @return status code
 *         - 0 success
 *         - 1 start fan cleaning failed
 * @note   none
 */
uint8_t sps30_basic_start_fan_cleaning(void);

/**
 * @brief  basic example sleep
 * @return status code
 *         - 0 success
 *         - 1 sleep failed
 * @note   none
 */
uint8_t sps30_basic_sleep(void);

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t sps30_basic_wake_up(void);

/**
 * @brief  basic example reset
 * @return status code
 *         - 0 success
 *         - 1 reset failed
 * @note   none
 */
uint8_t sps30_basic_reset(void);

/**
 * @brief      basic example get the product type
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get product type failed
 * @note       none
 */
uint8_t sps30_basic_get_product_type(char type[9]);

/**
 * @brief      basic example get the serial number
 * @param[out] *sn points to a sn buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t sps30_basic_get_serial_number(char sn[17]);

/**
 * @brief      basic example get the version
 * @param[out] *major points to a major buffer
 * @param[out] *minor points to a minor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 * @note       none
 */
uint8_t sps30_basic_get_version(uint8_t *major, uint8_t *minor);

/**
 * @brief      basic example get the status
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 * @note       none
 */
uint8_t sps30_basic_get_status(uint32_t *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
