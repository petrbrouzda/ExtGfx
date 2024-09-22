#include "SmallChart.h"

// pokud se povoli, dumpuji se detailni informace o zalamovani
//  #define DUMP_DEBUG_INFO

#define MAX_COLORS_PER_CHART 20

SmallChart::SmallChart( Adafruit_GFX *display )
{
    this->display = display;
    this->pixelsPerDatapoint = 1;
    this->options = SmallChart::CHART_BORDERS | SmallChart::CHART_MODE_BAR;
    this->dfltColors[0] = &this->dfltCp;
    this->dfltColors[1] = NULL;
    this->colors = this->dfltColors;
    this->background = EG_BLACK;
    this->border = EG_BLUE;

    this->dirty = true;
}

void SmallChart::setRange(float minVal, float maxVal)
{
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->range = maxVal - minVal;
}

void SmallChart::setPosition( int x, int y, int w, int h )
{
    this->x = x;
    this->y = y;
    this->h = h;
    this->w = w;
}

void SmallChart::setResolution(int pixelsPerDatapoint)
{
    this->pixelsPerDatapoint = pixelsPerDatapoint;
}

void SmallChart::setColors(uint16_t background, uint16_t border, ChColorProfile **colors)
{
    this->background = background;
    this->border = border;
    this->colors = colors;
}

void SmallChart::setSimpleColors(uint16_t background, uint16_t border, uint16_t chart)
{
    this->background = background;
    this->border = border;
    this->colors = this->dfltColors;
    this->dfltCp.color = chart;
}

void SmallChart::setOptions(int options)
{
    this->options = options;
}

void SmallChart::setDatasource(ChartDatasource *datasource)
{
    this->dataSrc = datasource;
    // aby to hned po nastaveni bylo dirty
    this->dataVer = datasource->version - 1;
}

void SmallChart::setDirty()
{
    this->dirty = true;
}


int colorHbarSize[MAX_COLORS_PER_CHART];
int colorHbarColor[MAX_COLORS_PER_CHART];

void vykresliSvislouPruhovanouCaru( int x, int y, int negativeSize, Adafruit_GFX *display )
{
    int sz = -negativeSize;
    
    for( int i = 0; i<MAX_COLORS_PER_CHART; i++ ) {
        int currentSize = colorHbarSize[i];
        if( currentSize==-1 ) {
            break;
        }

        if( sz>=currentSize ) {
            display->drawFastVLine( x, y , -currentSize, colorHbarColor[i] );
            sz -= currentSize;
            y -= currentSize;
            if( sz==0 ) {
                break;
            }
        } else { 
            // sz < currentSize
            display->drawFastVLine( x, y , -sz, colorHbarColor[i] );
            break;
        }
    }
}


void SmallChart::draw(bool force)
{
    if( !force && !this->dirty && this->dataSrc->version==this->dataVer ) {
        // neni treba prekreslovat
        return;
    }

    // drawing coordinates
    int dx, dy, dw, dh, fillTop, fillBottom;

    dx = this->x;
    dy = this->y;
    dw = this->w;
    dh = this->h;
    fillTop = 0;
    fillBottom = 0;

    if( this->options & SmallChart::CHART_LEFT_BORDER ) {
        this->display->drawFastVLine( this->x, this->y, this->h, this->border );
        dx++;
        dw--;
    }
    if( this->options & SmallChart::CHART_RIGHT_BORDER ) {
        this->display->drawFastVLine( this->x + this->w-1, this->y, this->h, this->border );
        dw--;
    }
    if( this->options & SmallChart::CHART_TOP_BORDER ) {
        this->display->drawFastHLine( this->x, this->y, this->w, this->border );
        if( this->options & SmallChart::CHART_TOP_NOT_MAX ) {
            dy++;
            dh--;
        } else {
            fillTop = 1;
        }
    }
    if( this->options & SmallChart::CHART_BOTTOM_BORDER ) {
        this->display->drawFastHLine( this->x, this->y+this->h-1, this->w, this->border );
        if( this->options & SmallChart::CHART_BOTTOM_NOT_MIN ) {
            dh--;
        } else {
            fillBottom = 1;
        }
    }
    
    bool barChartSHorizontalnimiPruhy = (this->options & SmallChart::CHART_COLORS_HBAR) && (this->options & SmallChart::CHART_MODE_BAR);


    if( barChartSHorizontalnimiPruhy ) {
        // předpočteme si barevnou tabulku
        int prevSize = 0;
        for(int c=0; c<MAX_COLORS_PER_CHART; c++ ) {
            ChColorProfile * color = this->colors[c];
            if( color == NULL ) {
                // posledni polozka
                colorHbarSize[c] = -1;
                if( c>0 ) {
                    colorHbarSize[c-1] = 1000;
                }
                break;
            }
            colorHbarColor[c] = color->color;
            float val = color->valueFrom;
            if( c>0 ) {
                int sz = (int)(((val - this->minVal) / this->range * (dh-1)) + 0.5);
                colorHbarSize[c-1] = sz - prevSize;
                prevSize = sz;
            }
        }
    }


    #ifdef DUMP_DEBUG_INFO  
        if( this->options & SmallChart::CHART_MODE_LINE ) {
            Serial.printf( "Line chart, TOP_NOT_MAX %s, BOTTOM_NOT_MIN %s; ", 
                ( this->options & SmallChart::CHART_TOP_NOT_MAX ? "yes" : "no"),
                ( this->options & SmallChart::CHART_BOTTOM_NOT_MIN ? "yes" : "no")  
            );
        } else {
            Serial.printf( "Bar chart, TOP_NOT_MAX %s, BOTTOM_NOT_MIN %s; ", 
                ( this->options & SmallChart::CHART_TOP_NOT_MAX ? "yes" : "no"),
                ( this->options & SmallChart::CHART_BOTTOM_NOT_MIN ? "yes" : "no")  
            );
            Serial.printf( "x/dx=%d/%d y/dy=%d/%d;", this->x, dx, this->y, dy );
            Serial.printf( "h/dh=%d/%d w/dw=%d/%d;", this->h, dh, this->w, dw );
            Serial.printf( "fillTop %d; ", fillTop );
        }
        Serial.printf( "border L %s, R %s, T %s, B %s\n",
            ( this->options & SmallChart::CHART_LEFT_BORDER ? "yes" : "no"),
            ( this->options & SmallChart::CHART_RIGHT_BORDER ? "yes" : "no"),
            ( this->options & SmallChart::CHART_TOP_BORDER ? "yes" : "no"),
            ( this->options & SmallChart::CHART_BOTTOM_BORDER ? "yes" : "no")
        );
    #endif    

    if( this->options & SmallChart::CHART_MODE_LINE ) {
        // vymazeme cely payload a pak muzeme kreslit jen caru
        this->display->fillRect( dx, dy+fillTop, dw, dh-fillTop-fillBottom, this->background );
    }

    int offset = 0;
    int pixX = dx;

    for( int i=this->dataSrc->data->numElements(this->dataSrc->data)-1; i>=0 ; i-- ) {

        float v = * (float*)(this->dataSrc->data->peek(this->dataSrc->data, i));

        ChColorProfile * color = this->colors[0];
        if( !barChartSHorizontalnimiPruhy  ) {
            // najdeme barvu odpovídající aktuální hodnotě
            int c=0;
            while(1) {
                if( this->colors[c] == NULL ) {
                    // posledni polozka
                    break;
                }
                if( this->colors[c]->valueFrom > v ) {
                    // nalezena vyssi hodnota
                    break;
                }
                color = this->colors[c];
                c++;
            }
        }

        if( v > this->maxVal ) {
            v = this->maxVal;
        } else if( v < this->minVal ) {
            v = this->minVal;
        }

        if( offset >= dw ) {
            // vykresleny cely graf
            break;
        }

        int size = (int)(((v - this->minVal) / this->range * (dh-1)) + 0.5);

        bool prepisujemeSpodniBorder = (this->options & SmallChart::CHART_BOTTOM_BORDER) && !(this->options & SmallChart::CHART_BOTTOM_NOT_MIN);

        for( int j = 0; j < this->pixelsPerDatapoint; j++ ) {

            if( this->options & SmallChart::CHART_MODE_LINE ) {

                // line chart

                int pixY = dy + dh - size - 1;
                
                this->display->drawPixel( pixX, pixY, color->color );

                if( this->options & SmallChart::CHART_LINE_2WIDTH ) {
                    if( dh==(size+1) ) {
                        // jsme uplne nahore, musime pridat jeden bod navic dolu pod caru
                        this->display->drawPixel( pixX, dy+1, color->color );
                    } else {
                        // jinak pridame jeden bod nahoru nad caru
                        this->display->drawPixel( pixX, pixY-1, color->color );
                    }
                }
            } else {
                // bar chart

                int pixYstart = dy+dh -1;

                if( !barChartSHorizontalnimiPruhy ) {
                    if( prepisujemeSpodniBorder ) {
                        if( size==0 ) {
                            // graf jede po spodnim borderu
                            this->display->drawPixel( pixX, pixYstart , color->color );
                        } else {
                            // zaciname o radek vys, aby zustal videt border
                            this->display->drawFastVLine( pixX, pixYstart-1, -size, color->color );
                        }
                    } else {
                        this->display->drawFastVLine( pixX, pixYstart , -size, color->color );
                    }
                } else {
                    if( prepisujemeSpodniBorder ) {
                        if( size==0 ) {
                            // graf jede po spodnim borderu
                            this->display->drawPixel( pixX, pixYstart , colorHbarColor[0] );
                        } else {
                            // zaciname o radek vys, aby zustal videt border
                            vykresliSvislouPruhovanouCaru( pixX, pixYstart-1, -size, this->display );
                        }
                    } else {
                        vykresliSvislouPruhovanouCaru( pixX, pixYstart, -size, this->display );
                    }  
                }
                
                if( size < (dh-1) ) {
                    this->display->drawFastVLine( pixX, dy+fillTop, dh-size-1-fillTop, this->background );
                }
            }

            offset++;
            pixX++;

            if( offset >= dw ) {
                // vykresleny cely graf
                break;
            }
        }
    }

    if( offset < dw && (this->options & SmallChart::CHART_MODE_BAR) ) {
        this->display->fillRect( dx+offset, dy+fillTop, dw-offset, dh-1-fillTop, this->background );
    }

    this->dirty = false;
    this->dataVer = this->dataSrc->version;
}

ChartDatasource::ChartDatasource(int numItems)
{
    this->data = RingBuf_new(sizeof(float), numItems );
    this->version = 0;
}

void ChartDatasource::put(float item )
{
    if( this->data->isFull(this->data) ) {
        float tmp;
        this->data->pull(this->data, &tmp);
    }
    this->data->add( this->data, &item );
    this->version++;
}

ChColorProfile::ChColorProfile(float valueFrom, uint16_t color)
{
    this->valueFrom = valueFrom;
    this->color = color;
}
