[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SPS30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sps30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SPS30顆粒物（PM）傳感器是光學PM傳感器的技術突破。其測量原理基於激光散射，並利用了Sensirion創新的抗污染技術。這項技術，加上高質量和持久耐用的組件，使精確測量從第一次運行，並在整個生命週期超過十年。此外，Sensirion的先進算法為不同PM類型和更高分辨率的顆粒尺寸分類提供了更高的精度，為檢測不同種類的環境粉塵和其他顆粒開闢了新的可能性。尺寸僅為41 x 41 x 12 mm3，對於尺寸至關重要的應用，如壁掛式或緊湊型空氣質量設備，它也是完美的解決方案。

LibDriver SPS30是LibDriver推出的SPS30的全功能驅動，該驅動提供空氣質量讀取、自動清理配置和ID讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver SPS30的源文件。

/interface目錄包含了LibDriver SPS30與平台無關的IIC、UART總線模板。

/test目錄包含了LibDriver SPS30驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver SPS30編程範例。

/doc目錄包含了LibDriver SPS30離線文檔。

/datasheet目錄包含了SPS30數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、UART總線模板，完成指定平台的IIC、UART總線驅動。

將/src目錄，/interface目錄和/example目錄加入工程。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/sps30/index.html](https://www.libdriver.com/docs/sps30/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。