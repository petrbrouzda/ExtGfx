#include "Adafruit_ST7789.h" 
#include "Adafruit_ST77xx.h"
#include <SPI.h>

#include "fonts/PragatiNarrow-Regular16pt8b.h"
#include "fonts/PragatiNarrow-Regular20pt8b.h"
#include "fonts/FrederickatheGreat-Regular40pt8b.h"

#include "src/extgfx/TextPainter.h"
#include "src/extgfx/HorizontalBar.h"
#include "src/extgfx/SmallChart.h"
#include "src/extgfx/BasicColors.h"

#include <math.h>

// parametry pripojeni displeje
#define TFT_CS   3
#define TFT_RST  2
#define TFT_DC   1
#define TFT_MOSI 6
#define TFT_SCLK 4
#define SPI_MISO_UNUSED -1

// LOW = vypnuto, HIGH = zapnuto
#define TFT_BACKLIGHT 0
#define TFT_BACKLIGHT_ON HIGH

Adafruit_ST7789 *tft;

TextPainter * painter;

TpFontConfig malePismo;
TpFontConfig malePismoSVetsimRadkovanim;
TpFontConfig vetsiPismo;


void setup() {
  Serial.begin(115200);
  delay(3000); 

  Serial.println("");
  Serial.println( "ExtGfx demo start" );

  // ++++ zde si nastavte svuj displej

    // nastavení pro 3.2" 240x320 displej s driverem ST7789 
    // https://s.click.aliexpress.com/e/_Dd1MOOf
    
    SPI.end(); // release standard SPI pins
    SPI.begin(TFT_SCLK, SPI_MISO_UNUSED, TFT_MOSI, TFT_CS); // map and init SPI pins
    tft = new Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

    pinMode( TFT_BACKLIGHT, OUTPUT );
    digitalWrite( TFT_BACKLIGHT, TFT_BACKLIGHT_ON );
    tft->init(240, 320);
    tft->invertDisplay(false);
    tft->setRotation(0);
    tft->fillScreen(EG_BLACK);
    tft->setTextColor(EG_WHITE);
  // ----- nastaveni displeje

  painter = new TextPainter( tft );
  painter->createFontConfig( &malePismo, &PragatiNarrow_Regular16pt8b );
  painter->createFontConfig( &vetsiPismo, &PragatiNarrow_Regular20pt8b );

  painter->createFontConfig( &malePismoSVetsimRadkovanim, &PragatiNarrow_Regular16pt8b );
  malePismoSVetsimRadkovanim.lineHeight = malePismoSVetsimRadkovanim.lineHeight + 5;

}


void demo1_zakladniTextovyBlok()
{
  // --- ukázka tisku textového bloku s rozdělováním slov na konci řádků

  // rozměry textového bloku
  int x = 20;
  int y = 25;
  int w = 200;
  int h = 115;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );

  painter->setFont( &malePismo );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Blok textu s rozdělováním slov");
  
  // začneme textový blok a zapíšeme do něj
  painter->startText( x, y, w, h );
  tft->setTextColor(EG_WHITE);
  int offset = painter->printText( (char*)"Text s češtinou, který by se měl zalamovat na konci řádku a je ve fialovém rámečku.");
  tft->setTextColor(EG_YELLOW);
  // použijeme offset vrácený z předešlého printText() jako druhý parametr a budeme pokračovat na stejné řádce dál
  painter->printText( (char*)" A za něj přidáme žlutý text.", offset );
  tft->setTextColor(EG_WHITE);
  // odřádkování
  painter->textLf();
  offset = painter->printText( (char*)"Nová řádka." );
  painter->printText( (char*)" Tenhle text už se nám do rámečku nevejde: Ó, náhlý déšť teď zvířil prach a čilá laň s houfcem gazel běží k úkrytům.", offset );


  // --- druhý textový blok bez rozdělování slov

  // rozměry textového bloku
  x = 20;
  y = 180;
  w = 200;
  h = 52;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 160, (char*)"Blok textu bez rozdělování");

  // vypneme rozdělování slov
  painter->setHyphenation( false );

  painter->startText( x, y, w, h );
  tft->setTextColor(EG_WHITE);
  offset = painter->printText( (char*)"Text s češtinou, který by se měl zalamovat na konci řádku a je ve fialovém rámečku.");

  // zase ho zapneme
  painter->setHyphenation( true );
}


void demo2_labely()
{
  painter->setFont( &malePismo );

  // --- zarovnání labelů

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Labely (jednořádkový text)");

#define Y_START_DEMO2 30

  tft->drawFastVLine( 4, Y_START_DEMO2, 55, EG_YELLOW );
  tft->drawFastVLine( 236, Y_START_DEMO2, 55, EG_YELLOW );
  tft->drawFastVLine( 120, Y_START_DEMO2, 55, EG_YELLOW );

  tft->setTextColor(EG_RED);
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, Y_START_DEMO2, (char*)"Zarovnané vlevo");
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, Y_START_DEMO2+18, (char*)"Zarovnané na střed");
  painter->printLabel( TextPainter::ALIGN_RIGHT, 235, Y_START_DEMO2+18+18, (char*)"Zarovnané vpravo");


  // --- práce s pozadím u labelů 
  // --- a taky ukázka použití getFont()

#define Y_START_DEMO2B Y_START_DEMO2+60

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, Y_START_DEMO2B+5, (char*)"Vyplněné pozadí");

  tft->setTextColor(EG_RED);

  /* 
  Zde si budeme nastavovat větší písmo. Abychom nerozbili konfiguraci zbytku aplikace 
  (tváříme se, že nevíme, jaké písmo je tam nastavené), vyčteme si stávající konfiguraci 
  a po tisku našich věcí ji tam vrátíme.
  */
  TpFontConfig * puvodniFont = painter->getFont();
  painter->setFont( &vetsiPismo );

  // zapneme pozadí
  painter->fillBackground( EG_WHITE );

  // tři různě vysoké texty vedle sebe, aby bylo vidět, jak ten obdélník s pozadím vypadá
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, Y_START_DEMO2B+25, (char*)"ahoj");
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, Y_START_DEMO2B+25, (char*)"---");
  painter->printLabel( TextPainter::ALIGN_RIGHT, 235, Y_START_DEMO2B+25, (char*)"VPRAVO");

  // zapneme pozadí s borderem
  painter->fillBackground( EG_WHITE, 6 );

  // tři různě vysoké texty vedle sebe, aby bylo vidět, jak ten obdélník s pozadím vypadá
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, Y_START_DEMO2B+55, (char*)"ahoj");
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, Y_START_DEMO2B+55, (char*)"---");
  painter->printLabel( TextPainter::ALIGN_RIGHT, 235, Y_START_DEMO2B+55, (char*)"VPRAVO");
  

  // uvedeme nastavení pozadí do původního stavu
  painter->noBackground();

  // a vrátíme konfiguraci malého písma ze zálohy, aniž bychom věděli, co tam je
  painter->setFont( puvodniFont );

  // vytiskneme text původním malým písmem
  tft->setTextColor(EG_WHITE);
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, Y_START_DEMO2B+100, (char*)"Vrácena původní velikost písma");

}


void demo3_zmenaRadkovani()
{
  // --- ukázka tisku změny řádkování

  painter->setFont( &malePismo );

  // rozměry textového bloku
  int x = 20;
  int y = 25;
  int w = 200;
  int h = 90;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Defaultní řádkování");
  
  // začneme textový blok a zapíšeme do něj
  painter->startText( x, y, w, h );
  tft->setTextColor(EG_WHITE);
  painter->printText( (char*)"Ó, náhlý déšť teď zvířil prach a čilá laň s houfcem gazel běží k úkrytům. Loď čeří kýlem tůň obzvlášť v Grónské úžině.");

  
  // --- druhý textový blok

  painter->setFont( &malePismoSVetsimRadkovanim );

  // rozměry textového bloku
  y = 150;
  w = 200;
  h = 90;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 125, (char*)"Zvětšené řádkování +5 px");

  painter->startText( x, y, w, h );
  tft->setTextColor(EG_WHITE);
  painter->printText( (char*)"Ó, náhlý déšť teď zvířil prach a čilá laň s houfcem gazel běží k úkrytům. Loď čeří kýlem tůň obzvlášť v Grónské úžině.");

}


void demo4_upravyRadkovaniFontu2()
{
  // ---- jako labely

  painter->setFont( &malePismo );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Ukázka úprav řádkování 1");

  // font beze změn
  TpFontConfig frederica1;
  painter->createFontConfig( &frederica1, &FrederickatheGreat_Regular40pt8b );
  painter->setFont( &frederica1 );
  painter->fillBackground( EG_WHITE );
  tft->setTextColor(EG_RED);
#define Y_START_DEMO3 30
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, Y_START_DEMO3, (char*)"Ahoj");

  // zvětšíme řádkování a offset první řádky
  TpFontConfig frederica2;
  painter->createFontConfig( &frederica2, &FrederickatheGreat_Regular40pt8b );
  frederica2.lineHeight -= 10;
  frederica2.firstLineHeightOffset += 20;
  painter->setFont( &frederica2 );
  painter->printLabel( TextPainter::ALIGN_RIGHT, 235, Y_START_DEMO3, (char*)"Ahoj");

  // uvedeme nastavení pozadí do původního stavu
  painter->noBackground();


  // ---- jako textové bloky
  painter->setFont( &malePismo );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 100, (char*)"Ukázka úpravy řádkování 2");

  tft->setTextColor(EG_WHITE);

  // rozměry textového bloku
  int x = 5;
  int y = 130;
  int w = 100;
  int h = 150;

  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );
  painter->setFont( &frederica1 );
  painter->startText( x, y, w, h );
  painter->printText( (char*)"Ahoj");
  painter->textLf();
  painter->printText( (char*)"lidi");

  // u tohohle boxu bude odstup řádek menší; ale první řádka začne s větším odstupem od hořejška

  x = 135;
  tft->drawRect( x-1, y-1, w+2, h+2, EG_MAGENTA );
  painter->setFont( &frederica2 );
  painter->startText( x, y, w, h );
  painter->printText( (char*)"Ahoj");
  painter->textLf();
  painter->printText( (char*)"lidi");

  // Všimněte si, že písmeno "A" přesahuje doleva přes okraj boxu.
  // To je schválně (funkce Adafruit GFX a fontu), aby to bylo zarovnané doleva správně.

}


  /**
   * Horizontal bar.
   * 
   * Demo: Ukázka indikátoru výkonu střídače.
   * Střídač má nominální výkon 0 - 2400 W, ale krátkodobě dokáže dodávat až 3000 W.
   * Overload režim můžeme znázornit jinou barvou (viz profil colors2).
   *
   * První dva bary budou ukazovat jen výkon 0-2400 W a při overloadu už zůstanou na 100 %
   * (a první nerozlišuje barevně mezi běžným výkonem a overloadem, druhý naopak při overloadu 
   * přejde do fialové), třetí ukáže plný rozsah 0-3000.
   */
void demo5_horizontalBar()
{
  painter->setFont( &malePismo );

  tft->setTextColor(EG_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Horizontal bar");

  /**
   * Barevný profil:
   * - pro nejnižší hodnoty je bílá barva
   * - pro rozsah 1440-2160 je žlutá
   * - pro víc než 2160 je červená
   * 
   * Je možné pro jednotlivé rozsahy hodnot nezávisle nastravit barvu baru, pozadí, textu a rámečku.
   * 
   */
  HbColorProfile c11( 0.0, EG_WHITE, EG_WHITE, EG_BLACK, EG_WHITE, EG_WHITE, EG_BLACK );
  HbColorProfile c12( 1440.0, EG_YELLOW, EG_YELLOW, EG_BLACK, EG_YELLOW, EG_YELLOW, EG_BLACK );
  HbColorProfile c13( 2160.0, EG_RED, EG_RED, EG_WHITE, EG_RED, EG_RED, EG_BLACK );
  // musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně
  HbColorProfile *colors1[] = { &c11, &c12, &c13, NULL };

  /**
   * Barevný profil:
   * - pro nejnižší hodnoty je bílá barva
   * - pro rozsah 1440-2160 je žlutá
   * - pro 2160-2400 je červená
   * - pro víc než 2400 (overload) je fialová
   */
  HbColorProfile c21( 0.0, EG_WHITE, EG_YELLOW, EG_WHITE, EG_BLACK, EG_WHITE, EG_BLACK );
  HbColorProfile c22( 1440.0, EG_YELLOW, EG_YELLOW, EG_YELLOW, EG_BLACK, EG_YELLOW, EG_BLACK );
  HbColorProfile c23( 2160.0, EG_RED, EG_YELLOW, EG_RED, EG_BLACK, EG_WHITE, EG_BLACK );
  HbColorProfile c24( 2400.0, EG_MAGENTA, EG_YELLOW, EG_RED, EG_BLACK, EG_WHITE, EG_BLACK );
  // musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně
  HbColorProfile *colors2[] = { &c21, &c22, &c23, &c24, NULL };

  tft->setTextColor(EG_WHITE);
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, 30, (char*)"Výkon (0-2400 W, overload až 3000 W):");

  HorizontalBar hb1( tft, painter );
  // pracovni rozsah stridace je 0-2400 W, ve spicce 3000 W; budeme ukazovat jen zakladni rozsah 0-2400
  hb1.setRange( 0, 2400 );
  hb1.setPosition( 5, 50, 230, 20 );
  hb1.setColors( (HbColorProfile**)&colors1 );
  hb1.setFont( &malePismo );

  HorizontalBar hb2( tft, painter );
  // pracovni rozsah stridace je 0-2400 W, ve spicce 3000 W; budeme ukazovat jen zakladni rozsah 0-2400
  hb2.setRange( 0, 2400 );
  hb2.setPosition( 5, 75, 230, 35 );
  hb2.setColors( (HbColorProfile**)&colors2 );
  hb2.setFont( &vetsiPismo );

  HorizontalBar hb3( tft, painter );
  // pracovni rozsah stridace je 0-2400 W, ve spicce 3000 W; zde ukazeme cely rozsah 0-3000
  hb3.setRange( 0, 3000 );
  hb3.setPosition( 5, 115, 230, 25 );
  hb3.setColors( (HbColorProfile**)&colors1 );
  hb3.setFont( &malePismo );

  // hodně úzký bar
  HorizontalBar hb4( tft, painter );
  // pracovni rozsah stridace je 0-2400 W, ve spicce 3000 W; zde ukazeme cely rozsah 0-3000
  hb4.setRange( 0, 3000 );
  hb4.setPosition( 5, 150, 230, 16 );
  hb4.setColors( (HbColorProfile**)&colors1 );
  hb4.setFont( &malePismo );



  /*
   * Všimněte si, že text se do baru nastavuje nezávisle na číselné hodnotě.
   * Tak je možné renderovat například odlišný text při různých hodnotách.
   */
  for( int i=0; i<2; i++ )  {
    for( float f = 0; f<3000.0; f += 135.0 ) {
      char buf[15];
      sprintf( buf, "%.0f W", f );
      hb1.setValue( f, buf );
      hb2.setValue( f, buf );
      hb3.setValue( f, buf );
      hb4.setValue( f, buf );
      hb1.draw();
      hb2.draw();
      hb3.draw();
      hb4.draw();
      delay( 500 );
    }
    for( float f = 3000; f>=0; f -= 121.0 ) {
      char buf[15];
      sprintf( buf, "%.0f W", f );
      hb1.setValue( f, buf );
      hb2.setValue( f, buf );
      hb3.setValue( f, buf );
      hb4.setValue( f, buf );
      hb1.draw();
      hb2.draw();
      hb3.draw();
      hb4.draw();
      delay( 500 );
    }

  }
}


/**
 * Základní ukázka grafů s barevným rozlišením dat.
 */
void demo6_smallChart1() 
{
  painter->setFont( &malePismo );
  tft->setTextColor(EG_GREEN);

  ChartDatasource * data = new ChartDatasource(250);

/*
 * Zde je barevný profil. Hodnoty do 1440 budou bílé, mezi 1440 a 2160 žluté, 2160-2800 červené a vyšší budou fialové.
 */
  ChColorProfile chc1( 0.0, EG_WHITE );
  ChColorProfile chc2( 1440.0, EG_YELLOW );
  ChColorProfile chc3( 2160.0, EG_RED );
  ChColorProfile chc4( 2800.0, EG_MAGENTA );
  // musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně
  ChColorProfile *colors[] = { &chc1, &chc2, &chc3, &chc4, NULL };

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 20, (char*)"SmallChart - MODE_BAR");  

  /*
   * Tento graf je vyplněný a má barevně odlišené třídy hodnot (třeba: standardní běh, plný výkon, přetížení).
   */
  SmallChart bch1( tft );
  bch1.setRange( 0, 3000 );
  bch1.setPosition( 5, 40, 230, 60 );
  bch1.setDatasource( data );
  bch1.setColors( EG_BLACK, EG_BLUE, colors );
  // bch1.setSimpleColors( EG_BLACK, EG_BLUE, EG_RED );
  bch1.setOptions( SmallChart::CHART_MODE_BAR | SmallChart::CHART_BORDERS | SmallChart::CHART_COLORS_HBAR );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 110, (char*)"SmallChart - MODE_LINE");  
  
  /*
   * Tento graf je čárový (tenkou čárou) a má barevně odlišené třídy hodnot (třeba: standardní běh, plný výkon, přetížení).
   */
  SmallChart bch2( tft );
  bch2.setRange( 0, 3000 );
  bch2.setPosition( 5, 130, 230, 50 );
  bch2.setDatasource( data );
  bch2.setColors( EG_BLACK, EG_BLUE, colors );
  bch2.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_BOTTOM_BORDER | SmallChart::CHART_LEFT_BORDER );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 190, (char*)"SmallChart - MODE_LINE");  
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 205, (char*)"+ 2WIDTH + resolution=2");  

  /*
   * Tento graf je čárový (tenkou čárou) a má barevně odlišené třídy hodnot (třeba: standardní běh, plný výkon, přetížení).
   * Je použitá čára o tloušťce dvou bodů.
   * Je nastaveno měřítko "dva pixely na šířku pro jeden bod v datech", takže graf jede 2x rychleji
   */
  SmallChart bch3( tft );
  bch3.setRange( 0, 3000 );
  bch3.setPosition( 5, 225, 230, 50 );
  bch3.setResolution( 2 );
  bch3.setDatasource( data );
  bch3.setColors( EG_BLACK, EG_BLUE, colors );
  bch3.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_LINE_2WIDTH | SmallChart::CHART_BORDERS | SmallChart::CHART_BOTTOM_BORDER | SmallChart::CHART_LEFT_BORDER  );

  float f = 0;
  while( f<35 ) {
    for( int i=0; i<20; i++ ) {
      float v = sin(f) * 1500.0 + f*50 + 1000;
      f += 0.05;
      data->put( v );
    }

    bch1.draw();
    bch2.draw();
    bch3.draw();

    delay(500);
  }

}


/**
  Zde je ukázán rozdíl v tom, jak se renderuje graf kolem rámečku.
  Dolní border může mít roli "spodní osy", tj. nejnižší hodnota se vykresluje přes osu (pak je
  doporučené, aby byla barevně kontrastní), nebo může mít roli čistě rámečku a data se vykreslují
  nad ním. Dtto se týká horního okraje, pokud je použit - nejvyšší hodnoty mohou zasahovat až přes 
  něj a přepisovat ho, nebo naopak vždy končí pod ním.
  Přepis horního a dolního rámečku je defaultní stav.
  Pokud to nechcete, dejte do options:
  SmallChart::CHART_BOTTOM_NOT_MIN - nepřepisovat spodní rámeček
  SmallChart::CHART_TOP_NOT_MAX - nepřepisovat horní rámeček
  */
void demo7_smallChart_bar() 
{


  painter->setFont( &malePismo );
  tft->setTextColor(EG_GREEN);

  ChartDatasource * data = new ChartDatasource(250);

  ChColorProfile chc1( 0.0, EG_WHITE );
  ChColorProfile chc2( 1440.0, EG_YELLOW );
  ChColorProfile chc3( 2160.0, EG_RED );
  ChColorProfile chc4( 2800.0, EG_MAGENTA );
  // musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně
  ChColorProfile *colors[] = { &chc1, &chc2, &chc3, &chc4, NULL };

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 20, (char*)"Bar - přes border");  

  SmallChart bch1( tft );
  bch1.setRange( 0, 3000 );
  bch1.setPosition( 5, 40, 230, 60 );
  bch1.setResolution( 1 );
  bch1.setDatasource( data );
  bch1.setColors( EG_BLACK, EG_BLUE, colors );
  bch1.setOptions( SmallChart::CHART_MODE_BAR | SmallChart::CHART_BORDERS );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 110, (char*)"Bar - pouze v rámečku");  
  
  SmallChart bch2( tft );
  bch2.setRange( 0, 3000 );
  bch2.setPosition( 5, 130, 230, 60 );
  bch2.setResolution( 1 );
  bch2.setDatasource( data );
  bch2.setColors( EG_BLACK, EG_BLUE, colors );
  bch2.setOptions( SmallChart::CHART_MODE_BAR | SmallChart::CHART_BORDERS | SmallChart::CHART_BOTTOM_NOT_MIN | SmallChart::CHART_TOP_NOT_MAX );

  float f = 0;

  while( f<35 ) {

    for( int i=0; i<20; i++ ) {
      float v = sin(f) * 1500.0 + f*50 + 1000;
      f += 0.05;
      data->put( v );
    }

    bch1.draw();
    bch2.draw();

    delay(500);
  }

}

/**
  Zde je ukázán rozdíl v tom, jak se renderuje graf kolem rámečku.
  Dolní border může mít roli "spodní osy", tj. nejnižší hodnota se vykresluje přes osu (pak je
  doporučené, aby byla barevně kontrastní), nebo může mít roli čistě rámečku a data se vykreslují
  nad ním. Dtto se týká horního okraje, pokud je použit - nejvyšší hodnoty mohou zasahovat až přes 
  něj a přepisovat ho, nebo naopak vždy končí pod ním.
  Přepis horního a dolního rámečku je defaultní stav.
  Pokud to nechcete, dejte do options:
  SmallChart::CHART_BOTTOM_NOT_MIN - nepřepisovat spodní rámeček
  SmallChart::CHART_TOP_NOT_MAX - nepřepisovat horní rámeček

  Nejspodnější graf používá tlustší čáru, horní dva tenkou.
  */
void demo8_smallChart_line() 
{


  painter->setFont( &malePismo );
  tft->setTextColor(EG_GREEN);

  ChartDatasource * data = new ChartDatasource(250);

  ChColorProfile chc1( 0.0, EG_WHITE );
  ChColorProfile chc2( 1440.0, EG_YELLOW );
  ChColorProfile chc3( 2160.0, EG_RED );
  ChColorProfile chc4( 2800.0, EG_MAGENTA );
  // musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně
  ChColorProfile *colors[] = { &chc1, &chc2, &chc3, &chc4, NULL };

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 10, (char*)"Line - přes border + SimpleColors");  

  /**
  Zde ukazano, ze barvy neni nutne definovat jen tabulkou; je mozne mit jednoduchou konfiguraci jen se tremi barvami
  */
  SmallChart bch1( tft );
  bch1.setRange( 0, 3000 );
  bch1.setPosition( 5, 30, 230, 50 );
  bch1.setResolution( 1 );
  bch1.setDatasource( data );
  // pouziti setSimpleColors
  bch1.setSimpleColors( EG_BLACK, EG_BLUE, EG_WHITE );
  bch1.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_BORDERS );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 90, (char*)"Line - vevnitř");  
  
  SmallChart bch2( tft );
  bch2.setRange( 0, 3000 );
  bch2.setPosition( 5, 110, 230, 50 );
  bch2.setResolution( 1 );
  bch2.setDatasource( data );
  bch2.setColors( EG_BLACK, EG_BLUE, colors );
  bch2.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_BORDERS | SmallChart::CHART_BOTTOM_NOT_MIN | SmallChart::CHART_TOP_NOT_MAX );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 170, (char*)"Line - vevnitř + tlustá");  

  SmallChart bch3( tft );
  bch3.setRange( 0, 3000 );
  bch3.setPosition( 5, 190, 225, 50 );
  bch3.setResolution( 1 );
  bch3.setDatasource( data );
  bch3.setColors( EG_BLACK, EG_BLUE, colors );
  bch3.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_LINE_2WIDTH | SmallChart::CHART_BORDERS | SmallChart::CHART_BOTTOM_NOT_MIN | SmallChart::CHART_TOP_NOT_MAX  );

  float f = 0;

  while( f<35 ) {

    for( int i=0; i<20; i++ ) {
      float v = sin(f) * 1500.0 + f*50 + 1000;
      f += 0.05;
      data->put( v );
    }

    bch1.draw();
    bch2.draw();
    bch3.draw();

    delay(500);
  }

}



/**
 * Úvodní ukázka ke grafům.
 */
void demo9_smallChart() 
{
  painter->setFont( &malePismo );
  tft->setTextColor(EG_GREEN);

  ChartDatasource * data = new ChartDatasource(250);

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 20, (char*)"SmallChart - vyplněný graf");  

  SmallChart bch1( tft );
  bch1.setRange( 0, 3000 );
  bch1.setPosition( 5, 40, 230, 60 );
  bch1.setDatasource( data );
  bch1.setOptions( SmallChart::CHART_MODE_BAR | SmallChart::CHART_BORDERS | SmallChart::CHART_COLORS_HBAR );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 110, (char*)"SmallChart - čárový graf");  
  
  SmallChart bch2( tft );
  bch2.setRange( 0, 3000 );
  bch2.setPosition( 5, 130, 230, 60 );
  bch2.setDatasource( data );
  bch2.setOptions( SmallChart::CHART_MODE_LINE | SmallChart::CHART_BOTTOM_BORDER | SmallChart::CHART_LEFT_BORDER | SmallChart::CHART_LINE_2WIDTH  );

  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 210, (char*)"(základní ukázka, bez barev)");  


  float f = 0;
  while( f<35 ) {

    for( int i=0; i<3; i++ ) {
      float v = sin(f) * 1500.0 + f*50 + 1000;
      f += 0.05;
      data->put( v );
    }

    bch1.draw();
    bch2.draw();

    delay(40);
  }

}




void loop() {
  
  Serial.println( "6 chart1");
  tft->fillScreen(EG_BLACK);
  demo9_smallChart();
  // no delay

  Serial.println( "5 horizontal bar");
  tft->fillScreen(EG_BLACK);
  demo5_horizontalBar();
  // no delay


  Serial.println( "6 chart1");
  tft->fillScreen(EG_BLACK);
  demo6_smallChart1();
  // no delay

  Serial.println( "7 chart - bar");
  tft->fillScreen(EG_BLACK);
  demo7_smallChart_bar();
  // no delay

  Serial.println( "8 chart - line");
  tft->fillScreen(EG_BLACK);
  demo8_smallChart_line();
  // no delay


  Serial.println( "1 textovy blok");
  tft->fillScreen(EG_BLACK);
  demo1_zakladniTextovyBlok();
  delay( 15000 );

  Serial.println( "2 labely");
  tft->fillScreen(EG_BLACK);
  demo2_labely();
  delay( 15000 );

  Serial.println( "3 zmena radkovani");
  tft->fillScreen(EG_BLACK);
  demo3_zmenaRadkovani();
  delay( 15000 );

  Serial.println( "4 radkovani");
  tft->fillScreen(EG_BLACK);
  demo4_upravyRadkovaniFontu2();
  delay( 15000 );


}




/*
Using library Adafruit ST7735 and ST7789 Library at version 1.10.0 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_ST7735_and_ST7789_Library 
Using library Adafruit GFX Library at version 1.11.8 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_GFX_Library 
Using library Adafruit BusIO at version 1.14.4 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_BusIO 
Using library Wire at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\Wire 
Using library SPI at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPI 
*/