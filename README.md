# ExtGfx

Sada grafických objektů ("widgetů") pro **ePaper a LCD/TFT/OLED displeje,** pro které je k dispozici knihovna implementující rozhraní [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) nebo [Arduino_GFX](https://github.com/moononournation/Arduino_GFX).

Několik ukázek:

| Grafy      | Práce s textem      | Bar gauge |
| ----------- | ----------- | ----------- |
| <img src="doc/d6.jpg" width="300">  | <img src="doc/d1.jpg" width="300">   | <img src="doc/d5-1.jpg" width="300"> |
| <img src="doc/d9.jpg" width="300">  | <img src="doc/d2.jpg" width="300">   | <img src="doc/d5-3.jpg" width="300"> |


Knihovny jsou určené pro ESP32; asi budou fungovat i na ESP8266, ale nešetří pamětí.

## Práce s textem

Objekt [TextPainter](ExtGfx/src/extgfx/TextPainter.h) zajišťuje práci s textem.

Umí tisk textu s **UTF-8 češtinou** pomocí fontů připravených postupem popsaným zde: https://github.com/petrbrouzda/fontconvert8-iso8859-2 .

Implementuje tisk textu do bloku se skoro korektním rozdělováním slov na konci řádků. Zde je ukázka - nahoře s rozdělováním, dole bez:

<img src="doc/d1.jpg" width="300">

Tisk jednořádkových textů ("labelů") se zarovnáním na určenou stranu, s možností automatického podbarvení pozadí.

<img src="doc/d2.jpg" width="300">

Detailní kontrola nad řádkováním. 

<img src="doc/d4.jpg" width="300">
<img src="doc/d3.jpg" width="300">

Demo a ukázky, jak se používá, najdete v demonstračních aplikacích ve funkcích:
* demo1_zakladniTextovyBlok()
* demo2_labely()
* demo3_zmenaRadkovani()
* demo4_upravyRadkovaniFontu2()

## Horizontal bar gauge ("progress dialog")

Je implementován v  [HorizontalBar](ExtGfx/src/extgfx/HorizontalBar.h).

Umí udělat indikátor různých velikostí. Implementuje "barevnou paletu", kde určíte, jaké mají být barvy pro jednotlivé rozsahy vstupních hodnot. Takže pro normální stav může být indikátor bílý, pro vysoké hodnoty červený atd.

Informační text se zobrazovanou hodnotou se ukazuje ve středu barevného pruhu; pokud je barevný pruh moc krátký, text se zobrazí vpravo od něj.

<img src="doc/d5-3.jpg" width="300">
<img src="doc/d5-1.jpg" width="300">
<img src="doc/d5-2.jpg" width="300">
<img src="doc/d5-4.jpg" width="300">

Demo a ukázky, jak se používá, najdete v demonstračních aplikacích ve funkci demo5_horizontalBar().

## Jednoduché grafy

Najdete je v  [SmallChart](ExtGfx/src/extgfx/SmallChart.h).

Demo a ukázky, jak se používají, najdete v demonstračních aplikacích ve funkcích:
* demo9_smallChart()
* demo6_smallChart1()
* demo7_smallChart_bar()
* demo8_smallChart_line()

Základní ukázka jednobarevného grafu - s vyplněním, jen čára. Můžete volně určit, které okraje grafu budou vykreslené - v horním grafu jsou zapnuté všechny, v dolním jen levý a dolní.

<img src="doc/d9.jpg" width="300">

Graf může být obarven podle hodnoty zobrazované veličiny - např. bíle standardní stav, červeně hodnoty blížící se limitu. Ve vyplněném grafu zde vidíte obarvení horizontální, kde se v grafu zabarvují jen hodnoty nad určenou úrovní:

<img src="doc/d6.jpg" width="300">

Též je možné nastavit obarvení vertikální, kde se pro daný rozsah hodnot aplikuje barva na celé výšce grafu:

<img src="doc/d7.jpg" width="300">

Je možné určit, zda je celý graf v prostoru mezi okraji (pokud jsou vykresleny), nebo zda dolní okraj funguje zároveň jako nejnižší hodnota ("osa X") a horní okraj jako nejvyšší hodnota. U čárového grafu je možno u větších displejů zapnout dvojitou tloušťku čáry pro vyšší čitelnost.

<img src="doc/d8.jpg" width="300">


## Demonstrační aplikace

Aplikace [demo__Adafruit_GFX](demo__Adafruit_GFX/demo__Adafruit_GFX.ino ) je určeno pro ESP32-C3 a levný 3.2" 240x320 displej s driverem ST7789 (https://s.click.aliexpress.com/e/_Dd1MOOf).

Po změně inicializace displeje by měla fungovat s čímkoli, co podporuje Adafruit_GFX. Pokud je váš displej menší než 240x320, pak samozřejmě bude potřeba upravit souřadnice a velikosti v jednotlivých ukázkách.

Aplikace [demo__Arduino_GFX](demo__Arduino_GFX/demo__Arduino_GFX.ino) je určena pro 7" CYD display s procesorem ESP32-S3 (https://s.click.aliexpress.com/e/_Dn1RQ6d). Konfigurace potřebných knihoven je popsána zde: https://pebrou.wordpress.com/2025/01/07/levny-7-displej-s-esp32-s3-poznamky/ jako varianta 3. 

## Q & A

### Jak to zahrnu do své aplikace?

Ve vaší aplikaci si udělejte adresář **src** a nakopírujte do něj obsah **ExtGfx/src**.
A pak si do svého .ino souboru vložte includy objektů, které potřebujete:

```
#include "src/extgfx/TextPainter.h"
#include "src/extgfx/HorizontalBar.h"
#include "src/extgfx/SmallChart.h"
```

a můžete je používat - tak, jak je ukázáno v demu.

Pokud nutně chcete zmenšit aplikaci, smažte z src/extgfx/ objekty, které nepotřebujete.

### Proč to není jako Arduino knihovna?

Mám špatné zkušenosti s řešením závislostí na Arduino knihovnách v Arduino IDE. Vždy se najde kombinace projektů, které chtějí něco navrájem nekompatibilního. Pokud si specifické knihovny nesete s projektem, jeho kompilace jinde je jednodušší.

### Jak získám fonty s podporou češtiny?

Viz https://github.com/petrbrouzda/fontconvert8-iso8859-2 


## Cizí kód použitý v aplikaci

### src/gfxlatin2/
Nástroje pro konverzi UTF-8 do 8bit kódování a pro práci s Adafruit GFX fonty v tomto kódování.

Michel Deslierres, michel@sigmdel.ca, https://www.sigmdel.ca/michel/program/misc/gfxfont_8bit_en.html

Proti původní verzi upravena pro české kódování, originál byl pro Francouze.

### src/RingBuff/
Knihovna pro ring buffer. 

D. Aaron Wisner, daw268@cornell.edu, https://github.com/wizard97/ArduinoRingBuffer

Proti původní verzi doplněna možnost kompilace na ESP32, byť bez možnosti použití z ISR.

### fonty/

Fonty z Google Fonts, zpracované postupem dle https://github.com/petrbrouzda/fontconvert8-iso8859-2 


## Závislosti na externích knihovnách

### Adafruit GFX

Očekává se, že v Arduino IDE máte odpovídající Adafruit GFX knihovnu pro váš displej. Konkrétní závislosti pro  displej použitý v demu vypadají takto:
* "Adafruit ST7735 and ST7789 Library" 1.10.0
* "Adafruit GFX Library" 1.11.8 
* "Adafruit BusIO" 1.14.4 
* "Wire" 2.0.0 
* "SPI" 2.0.0

Kompilace je otestována na ESP32 core pro Arduino **2.0.11**.

### Arduino GFX

V Arduino IDE musíte mít knihovnu „GFX library for Arduino“ by Moon On Our Nation ve verzi 1.5.0, **ne vyšší**.

Kompilace je otestována na ESP32 core pro Arduino **2.0.17**.

(Teoreticky by mohla fungovat verze 1.5.1 s ESP32 core 3.0.x.)