#include "Adafruit_ST7789.h" 
#include "Adafruit_ST77xx.h"
#include <SPI.h>

#include "src/fonts/PragatiNarrow-Regular16pt8b.h"
#include "src/extgfx/TextPainter.h"

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

void setup() {
  Serial.begin(115200);
  delay(3000); 

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
    tft->fillScreen(TP_BLACK);
    tft->setTextColor(TP_WHITE);
  // ----- nastaveni displeje

  painter = new TextPainter( tft );
}


void zakladniTextovyBlok()
{
  // --- ukázka tisku textového bloku s rozdělováním slov na konci řádků

  // rozměry textového bloku
  int x = 20;
  int y = 25;
  int w = 200;
  int h = 127;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, TP_MAGENTA );

  // definice fontu; velikost řádku snížíme o 9 bodů, jinak jsou řádky moc vysoké
  // a první řádku ještě přitáhneme nahoru o 4 body
  TpFontConfig malePismo( &PragatiNarrow_Regular16pt8b, -9, 0, -4 );
  painter->setFont( &malePismo );

  tft->setTextColor(TP_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 5, (char*)"Blok textu s rozdělováním slov");
  
  // začneme textový blok a zapíšeme do něj
  painter->startText( x, y, w, h );
  tft->setTextColor(TP_WHITE);
  int offset = painter->printText( (char*)"Text s češtinou, který by se měl zalamovat na konci řádku a je ve fialovém rámečku.");
  tft->setTextColor(TP_YELLOW);
  // použijeme offset vrácený z předešlého printText() jako druhý parametr a budeme pokračovat na stejné řádce dál
  painter->printText( (char*)" A za něj přidáme žlutý text.", offset );
  tft->setTextColor(TP_WHITE);
  // odřádkování
  painter->textLf();
  offset = painter->printText( (char*)"Nová řádka." );
  painter->printText( (char*)" Tenhle text už se nám do rámečku nevejde: Ó, náhlý déšť teď zvýřil prach a čilá laň s houfcem gazel běží k úkrytům.", offset );


  // --- druhý textový blok bez rozdělování slov

  // rozměry textového bloku
  x = 20;
  y = 180;
  w = 200;
  h = 52;

  // uděláme kolem něj fialový rámeček
  tft->drawRect( x-1, y-1, w+2, h+2, TP_MAGENTA );

  tft->setTextColor(TP_GREEN);
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 160, (char*)"Blok textu bez rozdělování");

  // vypneme rozdělování slov
  painter->setHyphenation( false );

  painter->startText( x, y, w, h );
  tft->setTextColor(TP_WHITE);
  offset = painter->printText( (char*)"Text s češtinou, který by se měl zalamovat na konci řádku a je ve fialovém rámečku.");

  // zase ho zapneme
  painter->setHyphenation( true );


  // --- třetí blok: ukázky tisku labelů

  tft->drawFastVLine( 4, 245, 55, TP_YELLOW );
  tft->drawFastVLine( 236, 245, 55, TP_YELLOW );
  tft->drawFastVLine( 120, 245, 55, TP_YELLOW );

  tft->setTextColor(TP_RED);
  painter->printLabel( TextPainter::ALIGN_LEFT, 5, 245, (char*)"Zarovnané vlevo");
  painter->printLabel( TextPainter::ALIGN_CENTER, 120, 262, (char*)"Zarovnané na střed");
  painter->printLabel( TextPainter::ALIGN_RIGHT, 235, 280, (char*)"Zarovnané vpravo");

}

void loop() {
  
  Serial.println( "Zakladni box s textem");
  tft->fillScreen(TP_BLACK);
  zakladniTextovyBlok();

  delay( 15000 );

}


/*
Using library Adafruit ST7735 and ST7789 Library at version 1.10.0 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_ST7735_and_ST7789_Library 
Using library Adafruit GFX Library at version 1.11.8 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_GFX_Library 
Using library Adafruit BusIO at version 1.14.4 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Adafruit_BusIO 
Using library Wire at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\Wire 
Using library SPI at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.11\libraries\SPI 
*/