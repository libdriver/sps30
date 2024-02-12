[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SPS30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sps30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SPS30粒子状物質（PM）センサーは、光学PMセンサーの技術的進歩です。その測定原理はレーザー散乱に基づいており、Sensirionの革新的な耐汚染性技術を利用しています。この技術は、高品質で長持ちするコンポーネントとともに、最初の操作から10年以上の寿命にわたって正確な測定を可能にします。さらに、Sensirionの高度なアルゴリズムは、さまざまなPMタイプに優れた精度を提供し、より高解像度の粒子サイズのビニングを提供し、さまざまな種類の環境ダストやその他の粒子を検出するための新しい可能性を開きます。わずか41x41 x 12 mm3の寸法で、壁に取り付けられたデバイスやコンパクトな空気品質のデバイスなど、サイズが最も重要なアプリケーションにも最適なソリューションです。

LibDriver SPS30は、LibDriverによって起動されたSPS30の全機能ドライバーです。空気品質の読み取り、自動クリーニング構成、IDの読み取りおよびその他の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SPS30のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SPS30用のプラットフォームに依存しないIIC,UARTバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SPS30ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SPS30プログラミング例が含まれています。

/ docディレクトリには、LibDriver SPS30オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SPS30データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC,UARTバステンプレートを参照して、指定したプラットフォームのIIC,UARTバスドライバを完成させます。

/ srcディレクトリ、/ interfaceディレクトリ、および/exampleディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/sps30/index.html](https://www.libdriver.com/docs/sps30/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。