### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

UART2 Pin:  TX/RX PA2/PA3.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SPS30

#### 3.1 Command Instruction

1. Show sps30 chip and driver information.

   ```shell
   sps30 (-i | --information)
   ```

2. Show sps30 help.

   ```shell
   sps30 (-h | --help)
   ```

3. Show sps30 pin connections of the current board.

   ```shell
   sps30 (-p | --port)
   ```

4. Run sps30 register test.

   ```shell
   sps30 (-t reg | --test=reg) [--interface=<iic | uart>]
   ```

5. Run sps30 read test, num means the test times.

   ```shell
   sps30 (-t read | --test=read) [--interface=<iic | uart>] [--times=<num>]
   ```

6. Run sps30 basic read function, num means the read times.

   ```shell
   sps30 (-e read | --example=read) [--interface=<iic | uart>] [--times=<num>]
   ```

7. Run sps30 basic get type function. 

   ```shell
   sps30 (-e type | --example=type) [--interface=<iic | uart>]
   ```

8. Run sps30 basic get sn function.

   ```shell
   sps30 (-e sn | --example=sn) [--interface=<iic | uart>]
   ```

9. Run sps30 basic clean function.  

   ```shell
   sps30 (-e clean | --example=clean) [--interface=<iic | uart>]
   ```

10. Run sps30 basic get version function.  

    ```shell
    sps30 (-e version | --example=version) [--interface=<iic | uart>]
    ```

11. Run sps30 basic get status function.  

    ```shell
    sps30 (-e status | --example=status) [--interface=<iic | uart>]
    ```

#### 3.2 Command Example

```shell
sps30 -i

sps30: chip is Sensirion SPS30.
sps30: manufacturer is Sensirion.
sps30: interface is UART IIC.
sps30: driver version is 1.0.
sps30: min supply voltage is 4.5V.
sps30: max supply voltage is 5.5V.
sps30: max current is 80.00mA.
sps30: max temperature is 60.0C.
sps30: min temperature is -10.0C.
```

```shell
sps30 -p

sps30: IIC interface SCL connected to GPIOB PIN8.
sps30: IIC interface SDA connected to GPIOB PIN9.
sps30: UART interface TX connected to GPIOA PIN2.
sps30: UART interface RX connected to GPIOA PIN3.
```

```shell
sps30 -t reg --interface=iic

sps30: chip is Sensirion SPS30.
sps30: manufacturer is Sensirion.
sps30: interface is UART IIC.
sps30: driver version is 1.0.
sps30: min supply voltage is 4.5V.
sps30: max supply voltage is 5.5V.
sps30: max current is 80.00mA.
sps30: max temperature is 60.0C.
sps30: min temperature is -10.0C.
sps30: start register test.
sps30: sps30_set_interface/sps30_get_interface test.
sps30: set interface iic.
sps30: check interface ok.
sps30: set interface uart.
sps30: check interface ok.
sps30: sps30_set_auto_cleaning_interval/sps30_get_auto_cleaning_interval test.
sps30: set auto cleaning interval 106672.
sps30: check interval ok.
sps30: sps30_disable_auto_cleaning_interval test.
sps30: check disable cleaning ok.
sps30: sps30_start_fan_cleaning test.
sps30: check fan cleaning ok.
sps30: sps30_get_product_type test.
sps30: type is 00080000.
sps30: sps30_get_serial_number test.
sps30: serial number is 219CF286F39C0D87.
sps30: sps30_get_version test.
sps30: major is 0x02 minor is 0x02 .
sps30: sps30_get_device_status test.
sps30: status is 0x180000 .
sps30: sps30_clear_device_status test.
sps30: check clear status ok.
sps30: sps30_read_data_flag test.
sps30: flag is 0x0 .
sps30: sps30_start_measurement/sps30_stop_measurement test.
sps30: check start measurement ok.
sps30: check stop measurement ok.
sps30: sps30_sleep test.
sps30: check sleep ok.
sps30: sps30_wake_up test.
sps30: check wake up ok.
sps30: sps30_reset test.
sps30: check reset ok.
sps30: finish register test.
```

```shell
sps30 -t read --interface=iic --times=3

sps30: chip is Sensirion SPS30.
sps30: manufacturer is Sensirion.
sps30: interface is UART IIC.
sps30: driver version is 1.0.
sps30: min supply voltage is 4.5V.
sps30: max supply voltage is 5.5V.
sps30: max current is 80.00mA.
sps30: max temperature is 60.0C.
sps30: min temperature is -10.0C.
sps30: start read test.
sps30: IEEE754 read test.
sps30: pm1.0 is 10.74 ug/m3.
sps30: pm2.5 is 23.69 ug/m3.
sps30: pm4.0 is 33.68 ug/m3.
sps30: pm10.0 is 38.95 ug/m3.
sps30: pm0.5 is 40.64 cm3.
sps30: pm1.0 is 70.37 cm3.
sps30: pm2.5 is 84.41 cm3.
sps30: pm4.0 is 86.98 cm3.
sps30: pm10.0 is 87.60 cm3.
sps30: typical is 0.95 um.
sps30: pm1.0 is 9.27 ug/m3.
sps30: pm2.5 is 15.31 ug/m3.
sps30: pm4.0 is 19.81 ug/m3.
sps30: pm10.0 is 22.19 ug/m3.
sps30: pm0.5 is 48.38 cm3.
sps30: pm1.0 is 66.37 cm3.
sps30: pm2.5 is 72.80 cm3.
sps30: pm4.0 is 73.97 cm3.
sps30: pm10.0 is 74.26 cm3.
sps30: typical is 0.85 um.
sps30: pm1.0 is 8.80 ug/m3.
sps30: pm2.5 is 13.28 ug/m3.
sps30: pm4.0 is 16.53 ug/m3.
sps30: pm10.0 is 18.25 ug/m3.
sps30: pm0.5 is 49.27 cm3.
sps30: pm1.0 is 64.46 cm3.
sps30: pm2.5 is 69.17 cm3.
sps30: pm4.0 is 70.02 cm3.
sps30: pm10.0 is 70.23 cm3.
sps30: typical is 0.73 um.
sps30: UINT16 read test.
sps30: pm1.0 is 5.00 ug/m3.
sps30: pm2.5 is 10.00 ug/m3.
sps30: pm4.0 is 14.00 ug/m3.
sps30: pm10.0 is 17.00 ug/m3.
sps30: pm0.5 is 19.00 cm3.
sps30: pm1.0 is 32.00 cm3.
sps30: pm2.5 is 38.00 cm3.
sps30: pm4.0 is 39.00 cm3.
sps30: pm10.0 is 39.00 cm3.
sps30: typical is 0.73 um.
sps30: pm1.0 is 4.00 ug/m3.
sps30: pm2.5 is 4.00 ug/m3.
sps30: pm4.0 is 4.00 ug/m3.
sps30: pm10.0 is 4.00 ug/m3.
sps30: pm0.5 is 25.00 cm3.
sps30: pm1.0 is 29.00 cm3.
sps30: pm2.5 is 29.00 cm3.
sps30: pm4.0 is 29.00 cm3.
sps30: pm10.0 is 29.00 cm3.
sps30: typical is 0.48 um.
sps30: pm1.0 is 4.00 ug/m3.
sps30: pm2.5 is 4.00 ug/m3.
sps30: pm4.0 is 4.00 ug/m3.
sps30: pm10.0 is 4.00 ug/m3.
sps30: pm0.5 is 26.00 cm3.
sps30: pm1.0 is 30.00 cm3.
sps30: pm2.5 is 30.00 cm3.
sps30: pm4.0 is 30.00 cm3.
sps30: pm10.0 is 30.00 cm3.
sps30: typical is 0.55 um.
sps30: finish read test.
```

```shell
sps30 -e read --interface=iic --times=3

sps30: 1/3.
sps30: pm1.0 is 5.24 ug/m3.
sps30: pm2.5 is 9.26 ug/m3.
sps30: pm4.0 is 12.29 ug/m3.
sps30: pm10.0 is 13.89 ug/m3.
sps30: pm0.5 is 25.76 cm3.
sps30: pm1.0 is 36.83 cm3.
sps30: pm2.5 is 41.14 cm3.
sps30: pm4.0 is 41.93 cm3.
sps30: pm10.0 is 42.12 cm3.
sps30: typical is 0.78 um.
sps30: 2/3.
sps30: pm1.0 is 4.79 ug/m3.
sps30: pm2.5 is 6.55 ug/m3.
sps30: pm4.0 is 7.78 ug/m3.
sps30: pm10.0 is 8.42 ug/m3.
sps30: pm0.5 is 28.60 cm3.
sps30: pm1.0 is 35.86 cm3.
sps30: pm2.5 is 37.67 cm3.
sps30: pm4.0 is 37.99 cm3.
sps30: pm10.0 is 38.07 cm3.
sps30: typical is 0.75 um.
sps30: 3/3.
sps30: pm1.0 is 4.75 ug/m3.
sps30: pm2.5 is 5.74 ug/m3.
sps30: pm4.0 is 6.35 ug/m3.
sps30: pm10.0 is 6.67 ug/m3.
sps30: pm0.5 is 30.26 cm3.
sps30: pm1.0 is 36.33 cm3.
sps30: pm2.5 is 37.29 cm3.
sps30: pm4.0 is 37.46 cm3.
sps30: pm10.0 is 37.50 cm3.
sps30: typical is 0.72 um.
```

```shell
sps30 -e type --interface=iic

sps30: type is 00080000.
```

```shell
sps30 -e sn --interface=iic 

sps30: sn is 219CF286F39C0D87.
```

```shell
sps30 -e version --interface=iic

sps30: major is 0x02 minor is 0x02.
```

```shell
sps30 -e status --interface=iic 

sps30: status is 00100000.
```

```shell
sps30 -e clean --interface=iic  

sps30: start cleaning.
sps30: stop cleaning.
```

```shell
sps30 -h

Usage:
  sps30 (-i | --information)
  sps30 (-h | --help)
  sps30 (-p | --port)
  sps30 (-t reg | --test=reg) [--interface=<iic | uart>]
  sps30 (-t read | --test=read) [--interface=<iic | uart>] [--times=<num>]
  sps30 (-e read | --example=read) [--interface=<iic | uart>] [--times=<num>]
  sps30 (-e type | --example=type) [--interface=<iic | uart>]
  sps30 (-e sn | --example=sn) [--interface=<iic | uart>]
  sps30 (-e clean | --example=clean) [--interface=<iic | uart>]
  sps30 (-e version | --example=version) [--interface=<iic | uart>]
  sps30 (-e status | --example=status) [--interface=<iic | uart>]

Options:
  -e <read | type | sn | clean | version | status>, --example=<read | type | sn | clean | version | status>
                                          Run the driver example.
  -h, --help                              Show the help.
  -i, --information                       Show the chip information.
      --interface=<iic | uart>            Set the chip interface.([default: iic])
  -p, --port                              Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>    Run the driver test.
      --times=<num>                       Set the running times.([default: 3])
```

