#ifndef _HORIZONTALBAR___H_
#define _HORIZONTALBAR___H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "TextPainter.h"


/*
Draws a bar with text.
- 'value' means length of the bar; should be 0-100 (as per cent)

Colors:
- colorBar - color of bar
- colorBorder - color of border
- bgColor - color of empty space in right of bar

If bar is longer than text, text is centered in bar:

    +---border--------------------------------+
    |###########################              |
    |######## 60 % #############              |
    |###########################              |
    +---border--------------------------------+

	and colors are used:
	- colorTextOnBar - text color
	- colorBgOnBar - background box under the text

else it is printer after bar:

    +---border--------------------------------+
    |########                                 |
    |######## 17526 W                         |
    |########                                 |
    +---border--------------------------------+

	and colors are used:
	- colorTextOnBg - text color
	- bgColor /--colorBgOnBg--/ - background box under the text

*/

#define HB_MAX_TEXT_SIZE 100

class HbColorProfile {
    public:
        HbColorProfile( float valueFrom, uint16_t colorBar, uint16_t colorBorder, 
                        uint16_t colorTextOnBar,  uint16_t colorBgOnBar, uint16_t colorTextOnBg, 
                        uint16_t bgColor );

        /** data value limit */
        float valueFrom;

        /** color of bar  */
        uint16_t colorBar;
        /** color of border  */
        uint16_t colorBorder;

        /** If bar is longer than text - text color */
        uint16_t colorTextOnBar;
        /** If bar is longer than text - background box under the text */
        uint16_t colorBgOnBar;
        
        /** If bar is shorter than text - text color */
        uint16_t colorTextOnBg;

        /** color of empty space in right of bar  */
        uint16_t bgColor;

};


class HorizontalBar {
    public:
        HorizontalBar( Adafruit_GFX *display, TextPainter * painter );
        void setFont( TpFontConfig * font );
        void setRange(float minVal, float maxVal);
        void setPosition( int x, int y, int w, int h );
        void setValue( float val, char * text );

        /** pole barevných konfigurací pro rozsahy hodnot; musi byt zakoncene NULLem; hodnoty musí být seřazené vzestupně */
        void setColors( HbColorProfile **colors );
        void draw( bool force = false );

        void setDirty();

    private:
        Adafruit_GFX *display;
        TextPainter * painter;
        HbColorProfile **colors;
        float minVal;
        float maxVal;
        /** vzdalenost mezi minVal a maxVal */
        float range;
        int x; 
        int y; 
        int h; 
        int w;
        /** jiz znormalizovane = s jistotou mezi minVal a maxVal */
        float currentvalue;
        char currentText[HB_MAX_TEXT_SIZE+2];
        bool dirty;
        TpFontConfig * font;

};


#endif