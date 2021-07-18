### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

iic pin: SCL/SDA GPIO3/GPIO2

uart pin: TX/RX GPIO14/GPIO15

### 2. install

#### 2.1 install info

```shell
make
```

### 3. sps30

#### 3.1 command Instruction

​          sps30 is a basic command which can test all sps30 driver function:

​           -i        show sps30 chip and driver information.

​           -h       show sps30 help.

​           -p       show sps30 pin connections of the current board.

​           -t (reg (-iic | -uart) | read <times> (-iic | -uart)) 

​           -t reg (-iic | -uart)       run sps30 register test.

​           -t read <times> (-iic | -uart)        run sps30 read test. times means the test times.

​           -c (basic read<times> (-iic | -uart) | basic type (-iic | -uart) | basic sn (-iic | -uart) | basic clean (-iic | -uart) | basic  version (-iic | -uart) | basic status (-iic | -uart))

​           -c basic read <times> (-iic | -uart)        run sps30 basic read function. times is the read times.

​           -c basic type  (-iic | -uart)        run sps30 basic get type function. 

​           -c basic sn  (-iic | -uart)        run sps30 basic get sn function.

​           -c basic clean (-iic | -uart)        run sps30 basic clean function.  

​           -c basic version (-iic | -uart)        run sps30 basic get version function.  

​           -c basic status (-iic | -uart)        run sps30 basic get status function.  

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
./sps30 -t reg -uart

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
./sps30 -t read 3 -uart

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
sps30: pm1.0 is 52.32 ug/m3.
sps30: pm2.5 is 74.44 ug/m3.
sps30: pm4.0 is 90.18 ug/m3.
sps30: pm10.0 is 98.47 ug/m3.
sps30: pm0.5 is 304.43 cm3.
sps30: pm1.0 is 388.05 cm3.
sps30: pm2.5 is 411.04 cm3.
sps30: pm4.0 is 415.18 cm3.
sps30: pm10.0 is 416.22 cm3.
sps30: typical is 0.76 um.
sps30: pm1.0 is 51.10 ug/m3.
sps30: pm2.5 is 60.21 ug/m3.
sps30: pm4.0 is 65.55 ug/m3.
sps30: pm10.0 is 68.35 ug/m3.
sps30: pm0.5 is 329.75 cm3.
sps30: pm1.0 is 392.76 cm3.
sps30: pm2.5 is 401.39 cm3.
sps30: pm4.0 is 402.87 cm3.
sps30: pm10.0 is 403.28 cm3.
sps30: typical is 0.67 um.
sps30: pm1.0 is 52.14 ug/m3.
sps30: pm2.5 is 59.20 ug/m3.
sps30: pm4.0 is 62.85 ug/m3.
sps30: pm10.0 is 64.76 ug/m3.
sps30: pm0.5 is 342.31 cm3.
sps30: pm1.0 is 403.27 cm3.
sps30: pm2.5 is 409.59 cm3.
sps30: pm4.0 is 410.64 cm3.
sps30: pm10.0 is 410.95 cm3.
sps30: typical is 0.65 um.
sps30: UINT16 read test.
sps30: pm1.0 is 36.00 ug/m3.
sps30: pm2.5 is 52.00 ug/m3.
sps30: pm4.0 is 64.00 ug/m3.
sps30: pm10.0 is 70.00 ug/m3.
sps30: pm0.5 is 210.00 cm3.
sps30: pm1.0 is 269.00 cm3.
sps30: pm2.5 is 286.00 cm3.
sps30: pm4.0 is 289.00 cm3.
sps30: pm10.0 is 290.00 cm3.
sps30: typical is 0.80 um.
sps30: pm1.0 is 35.00 ug/m3.
sps30: pm2.5 is 40.00 ug/m3.
sps30: pm4.0 is 44.00 ug/m3.
sps30: pm10.0 is 46.00 ug/m3.
sps30: pm0.5 is 225.00 cm3.
sps30: pm1.0 is 267.00 cm3.
sps30: pm2.5 is 272.00 cm3.
sps30: pm4.0 is 273.00 cm3.
sps30: pm10.0 is 274.00 cm3.
sps30: typical is 0.69 um.
sps30: pm1.0 is 37.00 ug/m3.
sps30: pm2.5 is 42.00 ug/m3.
sps30: pm4.0 is 44.00 ug/m3.
sps30: pm10.0 is 46.00 ug/m3.
sps30: pm0.5 is 244.00 cm3.
sps30: pm1.0 is 287.00 cm3.
sps30: pm2.5 is 291.00 cm3.
sps30: pm4.0 is 292.00 cm3.
sps30: pm10.0 is 292.00 cm3.
sps30: typical is 0.66 um.
sps30: finish read test.
```

```shell
./sps30 -c basic read 3 -uart

sps30: 1/3.
sps30: pm1.0 is 57.18 ug/m3.
sps30: pm2.5 is 59.96 ug/m3.
sps30: pm4.0 is 59.96 ug/m3.
sps30: pm10.0 is 59.96 ug/m3.
sps30: pm0.5 is 388.30 cm3.
sps30: pm1.0 is 447.74 cm3.
sps30: pm2.5 is 449.17 cm3.
sps30: pm4.0 is 449.30 cm3.
sps30: pm10.0 is 449.41 cm3.
sps30: typical is 0.51 um.
sps30: 2/3.
sps30: pm1.0 is 57.21 ug/m3.
sps30: pm2.5 is 60.00 ug/m3.
sps30: pm4.0 is 60.00 ug/m3.
sps30: pm10.0 is 60.00 ug/m3.
sps30: pm0.5 is 388.52 cm3.
sps30: pm1.0 is 447.99 cm3.
sps30: pm2.5 is 449.42 cm3.
sps30: pm4.0 is 449.56 cm3.
sps30: pm10.0 is 449.66 cm3.
sps30: typical is 0.54 um.
sps30: 3/3.
sps30: pm1.0 is 57.37 ug/m3.
sps30: pm2.5 is 60.16 ug/m3.
sps30: pm4.0 is 60.16 ug/m3.
sps30: pm10.0 is 60.16 ug/m3.
sps30: pm0.5 is 389.57 cm3.
sps30: pm1.0 is 449.20 cm3.
sps30: pm2.5 is 450.63 cm3.
sps30: pm4.0 is 450.76 cm3.
sps30: pm10.0 is 450.87 cm3.
sps30: typical is 0.54 um.
```

```shell
./sps30 -c basic type -uart

sps30: type is 00080000.
```

```shell
./sps30 -c basic sn -uart 

sps30: sn is 219CF286F39C0D87.
```

```shell
./sps30 -c basic version -uart

sps30: major is 0x02 minor is 0x02.
```

```shell
./sps30 -c basic status -uart  

sps30: status is 00100000.
```

```shell
./sps30 -c basic clean -uart  

sps30: start cleaning.
sps30: stop cleaning.
```

```shell
./sps30 -h

sps30 -i
	show sps30 chip and driver information.
sps30 -h
	show sps30 help.
sps30 -p
	show sps30 pin connections of the current board.
sps30 -t reg (-iic | -uart)
	run sps30 register test.
sps30 -t read <times> (-iic | -uart)
	run sps30 read test.times means the test times.
sps30 -c basic read <times> (-iic | -uart)
	run sps30 basic read function.times is the read times.
sps30 -c basic type (-iic | -uart)
	run sps30 basic get type function.
sps30 -c basic sn (-iic | -uart)
	run sps30 basic get sn function.
sps30 -c basic clean (-iic | -uart)
	run sps30 basic clean function.
sps30 -c basic version (-iic | -uart)
	run sps30 basic get version function.
sps30 -c basic status (-iic | -uart)
	run sps30 basic get status function.
```

