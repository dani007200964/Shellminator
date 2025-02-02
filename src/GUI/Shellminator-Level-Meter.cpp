/*
 * Created on Aug 13 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.08.13
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Shellminator-Level-Meter.hpp"


ShellminatorLevelMeter::ShellminatorLevelMeter(){
    redraw = true;
}

ShellminatorLevelMeter::ShellminatorLevelMeter( const char* name_p ){
    redraw = true;
    name = name_p;
}


void ShellminatorLevelMeter::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;

    if( name != NULL ){
        nameSize = strlen( name );
    }
}

void ShellminatorLevelMeter::setPercentage( float percentage_p ){

    if( parent == NULL ){
        return;
    }

    if( ( (int)( percentage_p * 10.0 ) ) != ( (int)( percentage * 10.0 ) ) ){
        percentage = percentage_p;
        redraw = true;
        parent -> requestRedraw();
    }
}

void ShellminatorLevelMeter::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    if( width != 5 ){
        width = 5;
    }

    // Try to read the next event in the eventBuffer.
    newEvent = parent -> readEvent();

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

}

void ShellminatorLevelMeter::setWarningPercentage( float percentage_p ){
    warningPercentage = percentage_p;
}

void ShellminatorLevelMeter::setErrorPercentage( float percentage_p ){
    errorPercentage = percentage_p;
}


void ShellminatorLevelMeter::draw( bool noClear ){

    // Ratio used to draw[ 0.0 -1.0 ].
    float ratio;

    // Generic counter
    int i;

    bool drawName = true;

    int totalSteps;
    int fullSteps;
    float stepPercentage;
    float fractionStep;
    int warningLimit = 1000;
    int errorLimit = 1000;

    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    if( name == NULL ){
        drawName = false;
    }

    // Copy percentage value to ratio.
    ratio = percentage;

    // Limit the ratio to a valid range.
    if( ratio > 100.0f ){
        ratio = 100.0f;
    }

    if( ratio < 0.0f ){
        ratio = 0.0f;
    }

    totalSteps = height - 2;
    fullSteps = (int)( (float)( ratio / 100.0 ) * totalSteps );
    stepPercentage = (float) 100.0 / totalSteps;
    fractionStep = ratio - ( (float)fullSteps * stepPercentage );
    fractionStep = ( fractionStep / stepPercentage ) * 8.0; // Make it between 0 and 8.

    if( warningPercentage > 0 ){
        warningLimit = (int)( (float)( warningPercentage / 100.0 ) * totalSteps );
    }

    if( errorPercentage > 0 ){
        errorLimit = (int)( (float)( errorPercentage / 100.0 ) * totalSteps );
    }

    // Draw Top side.
    Shellminator::setCursorPosition( channel, originX, originY );
    channel -> print( __CONST_TXT__( "\u250C\u2500\u2510  " ) );
    if( !noClear ){
        channel -> print( "\033[0K" );
    }

    for( i = 1; i < height - 1; i++ ){
        Shellminator::setCursorPosition( channel, originX, height - i );
    
        // Left border.
        channel -> print( __CONST_TXT__( "\u2502" ) );

        // Check for warning
        if( ( i > warningLimit ) && ( i <= errorLimit ) ){
            parent -> format( channel, warningColor );
        }

        else if( i >= errorLimit ){
            parent -> format( channel, errorColor );
        }

        else{
            parent -> format( channel, color );
        }

        // Empty cell.
        if( i > ( fullSteps + 1 ) ){
            channel -> print( ' ' );
        }

        // Fraction cell.
        else if(i == ( fullSteps + 1 ) ){

            switch( (int)fractionStep ){
                case 0:
                    channel -> print( __CONST_TXT__( " " ) );
                    break;
                case 1:
                    channel -> print( __CONST_TXT__( "\u2581" ) );
                    break;
                case 2:
                    channel -> print( __CONST_TXT__( "\u2582" ) );
                    break;
                case 3:
                    channel -> print( __CONST_TXT__( "\u2583" ) );
                    break;
                case 4:
                    channel -> print( __CONST_TXT__( "\u2584" ) );
                    break;
                case 5:
                    channel -> print( __CONST_TXT__( "\u2585" ) );
                    break;
                case 6:
                    channel -> print( __CONST_TXT__( "\u2586" ) );
                    break;
                case 7:
                    channel -> print( __CONST_TXT__( "\u2587" ) );
                    break;
                default:
                    channel -> print( __CONST_TXT__( "\u2588" ) );
                    break;

            }

        }

        // Full cell.
        else{
            channel -> print( __CONST_TXT__( "\u2588" ) );
        }

        parent -> format( channel, Shellminator::WHITE );

        // Right border.
        if( drawName ){

            if( i > nameSize ){
                drawName = false;
            }

        }

        if( drawName ){
            channel -> print( __CONST_TXT__( "\u2590" ) );
            parent -> format( channel, Shellminator::BACKGROUND, Shellminator::WHITE );
            channel -> print( name[ nameSize - i ] );
            channel -> print( ' ' );
            parent -> format( channel, Shellminator::REGULAR, Shellminator::WHITE );
        }

        else{
            channel -> print( __CONST_TXT__( "\u2502  " ) );
        }
        if( !noClear ){
            channel -> print( "\033[0K" );
        }

    }

    // Draw Bottom side.
    Shellminator::setCursorPosition( channel, originX, height );
    channel -> print( __CONST_TXT__( "\u2514\u2500\u2518  " ) );
    if( !noClear ){
        channel -> print( "\033[0K" );
    }

    if( name == NULL ){
        return;
    }

}

void ShellminatorLevelMeter::setColor( Shellminator::textColor_t color_p ){
    color = color_p;
}

void ShellminatorLevelMeter::setWarningColor( Shellminator::textColor_t color_p ){
    warningColor = color_p;
}

void ShellminatorLevelMeter::setErrorColor( Shellminator::textColor_t color_p ){
    errorColor = color_p;
}

void ShellminatorLevelMeter::forceRedraw(){
    redraw = true;
}

