### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

UART Pin: TX/RX GPIO14/GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(sps30 REQUIRED)
```


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
./sps30 -i

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
./sps30 -p

sps30: IIC interface SCL connected to GPIO3(BCM3).
sps30: IIC interface SDA connected to GPIO2(BCM2).
sps30: UART interface TX connected to GPIO14(BCM14).
sps30: UART interface RX connected to GPIO15(BCM15).
```

```shell
./sps30 -t reg --interface=uart

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
sps30: set auto cleaning interval 83303.
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
./sps30 -t read --interface=uart --times=3

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
sps30: pm1.0 is 6.27 ug/m3.
sps30: pm2.5 is 8.17 ug/m3.
sps30: pm4.0 is 9.45 ug/m3.
sps30: pm10.0 is 10.13 ug/m3.
sps30: pm0.5 is 38.44 cm3.
sps30: pm1.0 is 47.34 cm3.
sps30: pm2.5 is 49.26 cm3.
sps30: pm4.0 is 49.61 cm3.
sps30: pm10.0 is 49.69 cm3.
sps30: typical is 0.62 um.
sps30: pm1.0 is 5.91 ug/m3.
sps30: pm2.5 is 6.19 ug/m3.
sps30: pm4.0 is 6.19 ug/m3.
sps30: pm10.0 is 6.19 ug/m3.
sps30: pm0.5 is 40.10 cm3.
sps30: pm1.0 is 46.24 cm3.
sps30: pm2.5 is 46.39 cm3.
sps30: pm4.0 is 46.40 cm3.
sps30: pm10.0 is 46.41 cm3.
sps30: typical is 0.42 um.
sps30: pm1.0 is 5.89 ug/m3.
sps30: pm2.5 is 6.18 ug/m3.
sps30: pm4.0 is 6.18 ug/m3.
sps30: pm10.0 is 6.18 ug/m3.
sps30: pm0.5 is 39.99 cm3.
sps30: pm1.0 is 46.11 cm3.
sps30: pm2.5 is 46.26 cm3.
sps30: pm4.0 is 46.27 cm3.
sps30: pm10.0 is 46.28 cm3.
sps30: typical is 0.43 um.
sps30: UINT16 read test.
sps30: pm1.0 is 6.00 ug/m3.
sps30: pm2.5 is 9.00 ug/m3.
sps30: pm4.0 is 12.00 ug/m3.
sps30: pm10.0 is 13.00 ug/m3.
sps30: pm0.5 is 32.00 cm3.
sps30: pm1.0 is 42.00 cm3.
sps30: pm2.5 is 46.00 cm3.
sps30: pm4.0 is 47.00 cm3.
sps30: pm10.0 is 47.00 cm3.
sps30: typical is 0.82 um.
sps30: pm1.0 is 6.00 ug/m3.
sps30: pm2.5 is 8.00 ug/m3.
sps30: pm4.0 is 10.00 ug/m3.
sps30: pm10.0 is 10.00 ug/m3.
sps30: pm0.5 is 33.00 cm3.
sps30: pm1.0 is 42.00 cm3.
sps30: pm2.5 is 44.00 cm3.
sps30: pm4.0 is 45.00 cm3.
sps30: pm10.0 is 45.00 cm3.
sps30: typical is 0.71 um.
sps30: pm1.0 is 6.00 ug/m3.
sps30: pm2.5 is 7.00 ug/m3.
sps30: pm4.0 is 8.00 ug/m3.
sps30: pm10.0 is 9.00 ug/m3.
sps30: pm0.5 is 34.00 cm3.
sps30: pm1.0 is 42.00 cm3.
sps30: pm2.5 is 43.00 cm3.
sps30: pm4.0 is 44.00 cm3.
sps30: pm10.0 is 44.00 cm3.
sps30: typical is 0.65 um.
sps30: finish read test.
```

```shell
./sps30 -e read --interface=uart --times=3

sps30: 1/3.
sps30: pm1.0 is 4.46 ug/m3.
sps30: pm2.5 is 4.67 ug/m3.
sps30: pm4.0 is 4.67 ug/m3.
sps30: pm10.0 is 4.67 ug/m3.
sps30: pm0.5 is 30.26 cm3.
sps30: pm1.0 is 34.89 cm3.
sps30: pm2.5 is 35.00 cm3.
sps30: pm4.0 is 35.01 cm3.
sps30: pm10.0 is 35.02 cm3.
sps30: typical is 0.55 um.
sps30: 2/3.
sps30: pm1.0 is 4.39 ug/m3.
sps30: pm2.5 is 4.61 ug/m3.
sps30: pm4.0 is 4.61 ug/m3.
sps30: pm10.0 is 4.61 ug/m3.
sps30: pm0.5 is 29.84 cm3.
sps30: pm1.0 is 34.41 cm3.
sps30: pm2.5 is 34.52 cm3.
sps30: pm4.0 is 34.53 cm3.
sps30: pm10.0 is 34.54 cm3.
sps30: typical is 0.49 um.
sps30: 3/3.
sps30: pm1.0 is 4.42 ug/m3.
sps30: pm2.5 is 4.64 ug/m3.
sps30: pm4.0 is 4.64 ug/m3.
sps30: pm10.0 is 4.64 ug/m3.
sps30: pm0.5 is 30.03 cm3.
sps30: pm1.0 is 34.63 cm3.
sps30: pm2.5 is 34.74 cm3.
sps30: pm4.0 is 34.75 cm3.
sps30: pm10.0 is 34.76 cm3.
sps30: typical is 0.46 um.
```

```shell
./sps30 -e type --interface=uart

sps30: type is 00080000.
```

```shell
./sps30 -e sn --interface=uart 

sps30: sn is 219CF286F39C0D87.
```

```shell
./sps30 -e version --interface=uart

sps30: major is 0x02 minor is 0x02.
```

```shell
./sps30 -e status --interface=uart  

sps30: status is 00100000.
```

```shell
./sps30 -e clean --interface=uart  

sps30: start cleaning.
sps30: stop cleaning.
```

```shell
./sps30 -h

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

