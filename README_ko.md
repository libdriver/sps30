[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver SPS30

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sps30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SPS30 미립자 물질(PM) 센서는 광학 PM 센서의 획기적인 기술입니다. 측정 원리는 레이저 산란을 기반으로 하며 Sensirion의 혁신적인 내오염성 기술을 사용합니다. 이 기술은 고품질의 오래 지속되는 구성 요소와 함께 첫 작동부터 10년 이상의 수명 동안 정확한 측정을 가능하게 합니다. 또한 Sensirion의 고급 알고리즘은 다양한 PM 유형 및 고해상도 입자 크기 비닝에 대해 탁월한 정밀도를 제공하여 다양한 종류의 환경 먼지 및 기타 입자를 감지할 수 있는 새로운 가능성을 열어줍니다. 크기가 41 x 41 x 12 mm3에 불과한 이 제품은 벽걸이형 또는 소형 공기 청정 장치와 같이 크기가 가장 중요한 애플리케이션을 위한 완벽한 솔루션이기도 합니다.

LibDriver SPS30은 LibDriver에서 출시한 SPS30의 전체 기능 드라이버입니다. 공기질 판독, 자동 청소 구성, ID 판독 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver SPS30의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver SPS30용 플랫폼 독립적인 IIC,UART버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver SPS30드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver SPS30프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver SPS30오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 SPS30데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC,UART버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC,UART버스 드라이버를 완성하십시오.

/src 디렉토리, /interface 디렉토리 및 /example 디렉토리를 프로젝트에 추가하십시오.

### 사용

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

### 문서

온라인 문서: https://www.libdriver.com/docs/sps30/index.html

오프라인 문서: /doc/html/index.html

### 기고

연락주세요lishifenging@outlook.com

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com