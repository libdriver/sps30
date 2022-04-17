[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver SPS30
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sps30/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der SPS30 Particulate Matter (PM) Sensor ist ein technologischer Durchbruch bei optischen PM-Sensoren. Sein Messprinzip basiert auf Laserstreuung und nutzt die innovative Kontaminationsresistenz-Technologie von Sensirion. Diese Technologie, zusammen mit hochwertigen und langlebigen Komponenten, ermöglicht präzise Messungen von der ersten Inbetriebnahme an und über die gesamte Lebensdauer von mehr als zehn Jahren. Darüber hinaus bieten die fortschrittlichen Algorithmen von Sensirion eine überlegene Präzision für verschiedene PM-Typen und eine Partikelgrößenklassifizierung mit höherer Auflösung, was neue Möglichkeiten für die Erkennung verschiedener Arten von Umweltstaub und anderen Partikeln eröffnet. Mit Abmessungen von nur 41 x 41 x 12 mm3 ist es auch die perfekte Lösung für Anwendungen, bei denen es auf die Größe ankommt, wie z. B. wandmontierte oder kompakte Luftqualitätsgeräte.

LibDriver SPS30 ist der Treiber mit vollem Funktionsumfang von SPS30, der von LibDriver gestartet wurde. Es bietet Luftqualitätsmessung, automatische Reinigungskonfiguration, ID-Lesung und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver SPS30-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver SPS30 IIC,UART。

/test enthält den Testcode des LibDriver SPS30-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen。

/example enthält LibDriver SPS30-Beispielcode.

/doc enthält das LibDriver SPS30-Offlinedokument.

/Datenblatt enthält SPS30-Datenblatt。

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC,UART-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC,UART-Treiber fertig.

Fügen Sie /src, /interface und /example zu Ihrem Projekt hinzu.

### Nutzung

#### example basic

```C
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

### Dokument

Online-Dokumente: https://www.libdriver.com/docs/sps30/index.html

Offline-Dokumente: /doc/html/index.html

### Beitrag

Bitte senden Sie eine E-Mail an lishifenging@outlook.com

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com