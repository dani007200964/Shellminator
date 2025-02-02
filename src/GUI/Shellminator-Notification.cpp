/*
 * Created on June 23 2024
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2024.06.23
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

#include "Shellminator-Notification.hpp"

ShellminatorNotification::ShellminatorNotification(){
    text = NULL;
    textWidth = 0;
    textHeight = 0;
}

ShellminatorNotification::ShellminatorNotification( const char* text_p ){
    setText( text_p );
}

void ShellminatorNotification::setText( const char* text_p ){
    int i;
    int textWidthMax = 0;

    text = text_p;

    textWidth = 0;
    textHeight = 1;

    for( i = 0; i < strlen( text ); i++ ){

        if( text[ i ] == '\n' ){
            textHeight++;
            textWidth = 0;
            continue;
        }

        textWidth++;
        if( textWidth > textWidthMax ){
            textWidthMax = textWidth;
        }

    }

    textWidth = textWidthMax;
}

void ShellminatorNotification::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
    redraw = true;
}

void ShellminatorNotification::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    newEvent = parent -> readEvent();

    // In case of empty event, we can't do anything.
    if( newEvent.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

}

void ShellminatorNotification::draw( bool noClear ){

    // Generic counter.
    int i;
    int x;
    int y;
    int widthCounter;
    int heightCounter;

    int borderWidth;
    int borderHeight;

    int xStart;
    int yStart;

    // Check for the necessary parameters.
    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( text == NULL ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    borderWidth = textWidth + 4;
    borderHeight = textHeight + 2;

    if( borderWidth > width ){
        borderWidth = width;
    }

    if( borderHeight > height ){
        borderHeight = height;
    }

    xStart = originX + width / 2 - borderWidth / 2;
    yStart = originY + height / 2 - borderHeight / 2;

    parent -> format( channel, Shellminator::REGULAR, Shellminator::WHITE );

    // Set cursor to top left and print the instruction text.
    Shellminator::setCursorPosition( channel, xStart, yStart );

    channel -> print( __CONST_TXT__( "\u250C" ) );

    for( i = 0; i < borderWidth - 2; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" ) );
    }

    channel -> print( __CONST_TXT__( "\u2510" ) );


    i = 0;
    for( y = 0; y < borderHeight - 2; y++ ){
        Shellminator::setCursorPosition( channel, xStart, yStart + y + 1 );
        channel -> print( __CONST_TXT__( "\u2502 " ) );
        for( x = 0; x < borderWidth - 2; x++ ){
            if( text[ i ] == '\n' ){
                i++;
                break;
            }
            if( text[ i ] == '\0' ){
                break;
            }
            channel -> print( text[ i ] );
            i++;
        }
        Shellminator::setCursorPosition( channel, xStart + borderWidth - 2, yStart + y + 1 );
        channel -> print( __CONST_TXT__( " \u2502" ) );
    }

    // Set cursor to top left and print the instruction text.
    Shellminator::setCursorPosition( channel, xStart, yStart + borderHeight - 1 );

    channel -> print( __CONST_TXT__( "\u2514" ) );

    for( i = 0; i < borderWidth - 2; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" )  );
    }

    channel -> print( __CONST_TXT__( "\u2139" ) );

}

void ShellminatorNotification::forceRedraw(){
    redraw = true;
}