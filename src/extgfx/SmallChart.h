#ifndef _SMALLCHART___H_
#define _SMALLCHART___H_

/**
 * Nástroj pro jednoduché grafy.
 * Ukázka použití v ExtGfx.ino funkcích
 * - demo9_smallChart()
 * - demo6_smallChart1()
 * - demo7_smallChart_bar()
 * - demo8_smallChart_line()
 */


#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "TextPainter.h"
#include "BasicColors.h"

/*
Používáme upravenou verzi knihovny https://github.com/wizard97/ArduinoRingBuffer v lokálním podadresáři.
*/
#include "../RingBuf/RingBuf.h"



/**
 * Datový zdroj - ring buffer. Přes něj se vkládají data.
 */
class ChartDatasource {
    public:
        ChartDatasource( int numItems );
        void put( float item );
        RingBuf * data;
        int version;

};

/**
 * Barevný profil pro jeden rozsah hodnot.
 * Ukázka viz ExtGfx.ino funkce demo6_smallChart1()
 */
class ChColorProfile {
    public:
        ChColorProfile( float valueFrom, uint16_t color );

        /** data value limit */
        float valueFrom;

        /** color of bar  */
        uint16_t color;
};


/**
 * Nástroj pro jednoduché grafy.
 * Ukázka použití v ExtGfx.ino funkcích
 * - demo9_smallChart()
 * - demo6_smallChart1()
 * - demo7_smallChart_bar()
 * - demo8_smallChart_line()
 */
class SmallChart {

    public:
        SmallChart( Adafruit_GFX *display );

        /** Rozsah hodnot, které budou použity pro zobrazení od levé do pravé strany. Min musí být menší než max.*/
        void setRange(float minVal, float maxVal);
        /** Pozice a velikost widgetu. */
        void setPosition( int x, int y, int w, int h );
        /** Rozlišení - kolik px na jeden datový bod  */
        void setResolution( int pixelsPerDatapoint );
        /** Nastavení barevného profilu */
        void setColors( uint16_t background, uint16_t border, ChColorProfile **colors );
        /** Zjednodušené zadání barev pro jednobarevný graf */
        void setSimpleColors( uint16_t background, uint16_t border, uint16_t chart );
        /** Nastavení options - hodnot z enumu SmallChartOptions */
        void setOptions( int options );

        /** Nastavení zdroje dat */
        void setDatasource( ChartDatasource * datasource );

        /** Nastaví prvek k překreslení při příštím draw(), i když se nezměnily data. */
        void setDirty();

        /** Vykresli. Pokud se data nezměnila, nekreslí se, pokud není force=true. */
        void draw( bool force = false );

        /** Možné hodnoty pro options */
        enum SmallChartOptions { 
            /** vykreslit levý rámeček (osu Y) */
            CHART_LEFT_BORDER = 1,
            /** vykreslit pravý rámeček (pravou osu Y) */
            CHART_RIGHT_BORDER = 2,
            /** vykreslit horní rámeček */
            CHART_TOP_BORDER = 4,
            /** vykreslit spodní rámeček (osu X) */
            CHART_BOTTOM_BORDER = 8,
            /** vykreslit všechny rámečky  */
            CHART_BORDERS = 15,

            /** reřim grafu: vyplněný */
            CHART_MODE_BAR = 32,
            /** režim grafu: čára */
            CHART_MODE_LINE = 64,

            /** pro čárový graf: použije se čára dvojité tloušťky */
            CHART_LINE_2WIDTH = 128,

            /** nejnižší hodnota nepřepíše dolní border (border = rámeček, data začínají nad ním); pokud NENÍ nastaveno, nejnižší hodnota přepíše dolní border (border = osa Y v hodnotě 0) */
            CHART_BOTTOM_NOT_MIN = 256,

            /** nejvyšší hodnota nepřepíše horní border (border = rámeček, data začínají pod ním); pokud NENÍ nastaveno, nejvyšší hodnota přepíše dolní border  */
            CHART_TOP_NOT_MAX = 512,

            /** pro vyplněný graf: barvy budou vyplňovány horizontálně viz. demo6 (pokud není vyplněno, obarvuje se vertikálně, viz demo7) */
            CHART_COLORS_HBAR = 1024,
        };

    private:
        Adafruit_GFX *display;

        ChColorProfile **colors;
        uint16_t background;
        uint16_t border;

        int options;

        ChartDatasource * dataSrc;
        int dataVer;

        float minVal;
        float maxVal;
        int x; 
        int y; 
        int h; 
        int w;
        int pixelsPerDatapoint;

        bool dirty;

        /** vzdalenost mezi minVal a maxVal */
        float range;

        ChColorProfile dfltCp = { 0.0, EG_WHITE };
        ChColorProfile *dfltColors[2]; 
};

#endif
