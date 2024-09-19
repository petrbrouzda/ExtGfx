#ifndef _TEXTPAINTER___H_
#define _TEXTPAINTER___H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "../gfxlatin2/gfxlatin2.h"

/** velikost pracovního buffer, delší text bude oříznut na tuto délku */
#define BUFFER_SIZE 512

/** maximální délka slova, delší se vždy zalomí */
#define MAX_WORD_SIZE 20

/** takhle krátká a kratší slova se nikdy nezalamují */
#define MAX_DELKA_SLOVA_CO_SE_NEZALAMUJE 4


// zakladni barvy

#define	TP_BLACK   0x0000
#define	TP_BLUE    0x001F
#define	TP_RED     0xF800
#define	TP_GREEN   0x07E0
#define TP_CYAN    0x07FF
#define TP_MAGENTA 0xF81F
#define TP_YELLOW  0xFFE0
#define TP_WHITE   0xFFFF


class TpFontConfig {
    public:
        TpFontConfig( const GFXfont * font, int lineHeightOffset = 0, int charWidthOffset = 0, int firstLineHeightOffset = 0 );

        const GFXfont * font;
        int lineHeightOffset;
        int charWidthOffset;
        int firstLineHeightOffset;
};

#define FP_MAX_SIZE -1

class TextPainter {

    public:
        /** 
         * hyphenation = rozdělování slov na konci řádku;
         * convertUtf8to8bit = podpora pro češtinu podle https://github.com/petrbrouzda/fontconvert8-iso8859-2
         */
        TextPainter( Adafruit_GFX* display, bool hyphenation = true, bool convertUtf8to8bit = true );


        /* nastaví použitý font a řádkování */
        void setFont( TpFontConfig * fontConfig );


        /** Tisk textového pole: nastaví pozici levého horního rohu pole a velikost pole */
        void startText( int x, int y, int width = FP_MAX_SIZE, int height = FP_MAX_SIZE );

        /**
		 * Tisk textového pole: 
         * 
         * Vytiskne UTF-8 (pokud je zapnuto convertUtf8to8bit) text na displej 
         * vcetne skoro korektniho zalamovani slov (pokud je zapnuto wrapWords).
		 * 
         * Pokud je zapnuto convertUtf8to8bit, obsluhuje češtinu v UTF-8. V tom případě
		 * je potreba pouzit zpracovani fontu dle https://github.com/petrbrouzda/fontconvert8-iso8859-2 !!!
		 * 
		 * Prvni radek se tiskne od pozice [X+x_offset, Y], ktera je LEVY HORNI roh prvniho pismene. 
		 * x_offset se pouzije jen pro prvni radek, dalsi radek se tiskne na [X, Y+vyskaRadku] 
		 * 
		 * Zalamovani radek je omezene nastavenym bounding boxem - pri zavolani setPos(x,y) se 
		 * nastavi bounding box.
		 * 
		 * Vraci aktualni pozici X, kterou je mozne pouzit jako x_offset pro dalsi volani (tj. ne absolutní
         * pozici X, ale vzdálenost od levé hrany textového pole).
		 * Necha nastavenou posX = puvodni posX; nastavi posY = posledni radek s textem.
		 */ 
        int printText( const char * text, int x_offset = 0 );

        /** Tisk textového pole: Přeskočí na další řádek */
        void textLf( int offset=0 );

        /** Tisk textového pole: zapíná/vypíná rozdělování slov na konci řádku */
        void setHyphenation( bool hyphenation );        



        /** ekvivalent getTextBounds() pracujici s UTF-8 textem  (pokud je zapnuto convertUtf8to8bit)
			@brief  Helper to determine size of a string with current font/size.
					Pass string and a cursor position, returns UL corner and W,H.
			@param  str  The ASCII string to measure
			@param  x    The current cursor X
			@param  y    The current cursor Y
			@param  x1   The boundary X coordinate, returned by function
			@param  y1   The boundary Y coordinate, returned by function
			@param  w    The boundary width, returned by function
			@param  h    The boundary height, returned by function
		*/
		void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1,
                   int16_t *y1, uint16_t *w, uint16_t *h);


        /** Možná zarovnání pro printLabel() */
        enum HorizontalAlign { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

        /** Vytiskne jednořádkový text. Y je horní okraj textu. 
         * 
         * Zarovnání vlevo = X je levá hrana textu.
         * Zarovnání doprostřed = X je střed textu.
         * Zarovnání vpravo = X je pravá hrana textu.
         * 
         * Text by neměl být širší než zbývající místo na displeji, knihovna to neřeší!
        */
        void printLabel( TextPainter::HorizontalAlign ha, int x, int y, char * text );



    private:
        Adafruit_GFX* display;

        /** rozdělování slov na konci řádku */
        bool hyphenation; 
        bool convertUtf8to8bit;

        /** sirka mezery, nastavi se automaticky z fontu, ale je mozne zmenit */
        int sirkaMezery;

        /** vyska radku, nastavi se automaticky z fontu, ale je mozne zmenit */
        int vyskaRadku;

        /** o kolik zacne prvni radek pod Y pozici */
        int vyskaPrvnihoRadku;
        
        /** pozice kurzoru */
        int posX;
        /** pozice kurzoru */
        int posY;

        /** zacatek tiskoveho boxu */
        int textBoxY;

        /** pro tisk textu: sirka obdelniku, do ktereho se smi vypsat text */
        int boundingBoxWidth;

        /** pro tisk textu: vyska obdelniku, do ktereho se smi vypsat text */
        int textMaxY;

        TpFontConfig * fontConfig;

        // ++++ parser textu
        bool jeSamohlaska( char c );
        char samohlasky[60];
        char * curPos;
        char oneWord[MAX_WORD_SIZE+2];
        char delimiter;
        void initParser( char * text );
        boolean getNextWord();
        // --- parser textu
};

#endif