#ifndef _SMALLCHART___H_
#define _SMALLCHART___H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "TextPainter.h"
#include "BasicColors.h"

#include "../RingBuf/RingBuf.h"

/*
https://github.com/wizard97/ArduinoRingBuffer
*/

class ChartDatasource {
    public:
        ChartDatasource( int numItems );
        void put( float item );
        RingBuf * data;
        int version;

};

class ChColorProfile {
    public:
        ChColorProfile( float valueFrom, uint16_t color );

        /** data value limit */
        float valueFrom;

        /** color of bar  */
        uint16_t color;
};

class SmallChart {

    public:
        SmallChart( Adafruit_GFX *display );

        /** Rozsah hodnot, které budou použity pro zobrazení od levé do pravé strany. Min musí být menší než max.*/
        void setRange(float minVal, float maxVal);
        /** Pozice a velikost widgetu. */
        void setPosition( int x, int y, int w, int h );
        /** Rozlišení - kolik px na jeden datový bod  */
        void setResolution( int pixelsPerDatapoint );

        void setColors( uint16_t background, uint16_t border, ChColorProfile **colors );

        void setSimpleColors( uint16_t background, uint16_t border, uint16_t chart );

        void setOptions( int options );

        void setDatasource( ChartDatasource * datasource );

        /** Nastaví prvek k překreslení při příštím draw(), i když se nezměnily data. */
        void setDirty();

        /** Vykresli. Pokud se data nezměnila, nekreslí se, pokud není force=true. */
        void draw( bool force = false );

        enum SmallChartOptions { 
            CHART_LEFT_BORDER = 1,
            CHART_RIGHT_BORDER = 2,
            CHART_TOP_BORDER = 4,
            CHART_BOTTOM_BORDER = 8,
            CHART_BORDERS = 15,

            CHART_MODE_BAR = 32,
            CHART_MODE_LINE = 64,

            CHART_LINE_2WIDTH = 128,

            /** nejnižší hodnota nepřepíše dolní border (border = rámeček, data začínají nad ním); pokud NENÍ nastaveno, nejnižší hodnota přepíše dolní border (border = osa Y v hodnotě 0) */
            CHART_BOTTOM_NOT_MIN = 256,

            /** nejvyšší hodnota nepřepíše horní border (border = rámeček, data začínají pod ním); pokud NENÍ nastaveno, nejvyšší hodnota přepíše dolní border  */
            CHART_TOP_NOT_MAX = 512,

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
