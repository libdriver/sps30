[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SPS30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue)](https://www.libdriver.com/docs/sps30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SPS30颗粒物（PM）传感器是光学PM传感器的技术突破。其测量原理基于激光散射，并利用了Sensirion创新的抗污染技术。这项技术，加上高质量和持久耐用的组件，使精确测量从第一次运行，并在整个生命周期超过十年。此外，Sensirion的先进算法为不同PM类型和更高分辨率的颗粒尺寸分类提供了更高的精度，为检测不同种类的环境粉尘和其他颗粒开辟了新的可能性。尺寸仅为41 x 41 x 12 mm3，对于尺寸至关重要的应用，如壁挂式或紧凑型空气质量设备，它也是完美的解决方案。

LibDriver SPS30是LibDriver推出的SPS30的全功能驱动，该驱动提供空气质量读取、自动清理配置和ID读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver SPS30的源文件。

/interface目录包含了LibDriver SPS30与平台无关的IIC、UART总线模板。

/test目录包含了LibDriver SPS30驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver SPS30编程范例。

/doc目录包含了LibDriver SPS30离线文档。

/datasheet目录包含了SPS30数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC、UART总线模板，完成指定平台的IIC、UART总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

#### example basic

```C
#include "driver_sps30_basic.h"

uint8_t res;
uint32_t i;
uint8_t major, minor;
uint32_t status;
char type[9];
char sn[9];

res = sps30_basic_init(SPS30_INTERFACE_UART);
if (res != 0)
{
    return 1;
}

...

/* delay 2000 ms */
sps30_interface_delay_ms(2000);

/* get type */
res = sps30_basic_get_product_type((char *)type);
if (res != 0)
{
    (void)sps30_basic_deinit();

    return 1;
}
sps30_interface_debug_print("sps30: type is %s.\n", type);

/* get sn */
res = sps30_basic_get_serial_number((char *)sn);
if (res != 0)
{
    (void)sps30_basic_deinit();

    return 1;
}
sps30_interface_debug_print("sps30: sn is %s.\n", sn);

/* get version */
res = sps30_basic_get_version((uint8_t *)&major, (uint8_t *)&minor);
if (res != 0)
{
    (void)sps30_basic_deinit();

    return 1;
}
sps30_interface_debug_print("sps30: major is 0x%02X minor is 0x%02X.\n", major, minor);

/* get status */
res = sps30_basic_get_status((uint32_t *)&status);
if (res != 0)
{
    (void)sps30_basic_deinit();

    return 1;
}
sps30_interface_debug_print("sps30: status is %08X.\n", status);

...
/* print */
sps30_interface_debug_print("sps30: start cleaning.\n");

/* start fan cleaning */
res = sps30_basic_start_fan_cleaning();
if (res != 0)
{
    (void)sps30_basic_deinit();

    return 1;
}
sps30_interface_delay_ms(1000 * 10);

/* print */
sps30_interface_debug_print("sps30: stop cleaning.\n");
...

for (i = 0; i < 3; i++)
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
    sps30_interface_debug_print("sps30: %d/%d.\n", i + 1, 3);
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
    
    ...
    
}

...

(void)sps30_basic_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/sps30/index.html](https://www.libdriver.com/docs/sps30/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。