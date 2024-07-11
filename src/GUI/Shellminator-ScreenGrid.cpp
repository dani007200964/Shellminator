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

#include "Shellminator-ScreenGrid.hpp"

ShellminatorScreenGrid::ShellminatorScreenGrid(){
    numOfRows = 0;
    numOfColumns = 0;

    cellWidth = 0;
    cellHeight = 0;

    head = NULL;
}

ShellminatorScreenGrid::ShellminatorScreenGrid( int numOfRows_p, int numOfColumns_p ){
    numOfRows = numOfRows_p;
    numOfColumns = numOfColumns_p;

    cellWidth = 0;
    cellHeight = 0;

    head = NULL;
}

void ShellminatorScreenGrid::init( Shellminator* parent_p, Stream* channel_p ){
    ShellminatorScreen* currentElement;

    parent = parent_p;
    channel = channel_p;
    redraw = true;

    currentElement = head;

    while( currentElement != NULL ){
        currentElement -> init( parent, channel );
        currentElement = currentElement -> nextElement;
    }

}

void ShellminatorScreenGrid::update( int width_p, int  height_p ){

    int calculatedOriginX;
    int calculatedOriginY;

    int calculatedWidth;
    int calculatedHeight;

    ShellminatorScreen* currentElement;

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    if( numOfRows < 1 ){
        return;
    }

    if( numOfColumns < 1 ){
        return;
    }

    width = width_p;
    height = height_p;

    cellWidth = width / numOfColumns;
    cellHeight = height / numOfRows;

    newEvent = parent -> readEvent();

    currentElement = head;
    while( currentElement != NULL ){
        calculatedOriginX = currentElement -> column * cellWidth + 1;
        calculatedOriginY = currentElement -> row * cellHeight;

        calculatedWidth = currentElement -> columnSpan * cellWidth;
        calculatedHeight = currentElement -> rowSpan * cellHeight;

        if( ( calculatedOriginX + calculatedWidth ) > width ){
            currentElement = currentElement -> nextElement;
            continue;
        }

        if( ( calculatedOriginY + calculatedHeight ) > height ){
            currentElement = currentElement -> nextElement;
            continue;
        }

        currentElement -> setOrigin( calculatedOriginX, calculatedOriginY );
        currentElement -> update( calculatedWidth, calculatedHeight );

        currentElement = currentElement -> nextElement;
    }


    // In case of empty event, we can't do anything.
    if( newEvent.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

}

void ShellminatorScreenGrid::draw( bool noClear ){

    int i;

    ShellminatorScreen* currentElement;

    // Check for the necessary parameters.
    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( numOfRows < 1 ){
        return;
    }

    if( numOfColumns < 1 ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    for( i = 1; i < height; i++ ){
        Shellminator::setCursorPosition( channel, 1, i + 1 );
        channel -> print( "\033[0K" );
    }

    currentElement = head;

    while( currentElement != NULL ){
        currentElement -> draw( true );
        currentElement = currentElement -> nextElement;
    }

}

void ShellminatorScreenGrid::addWidget( ShellminatorScreen* widget, int row_p, int column_p, int rowSpan_p, int columnSpan_p ){

    ShellminatorScreen* currentElement;

    if( row_p < 0 ){
        return;
    }

    if( column_p < 0 ){
        return;
    }

    if( rowSpan_p < 1 ){
        return;
    }

    if( columnSpan_p < 1 ){
        return;
    }

    if( head == NULL ){
        head = widget;
        head -> row = row_p;
        head -> column = column_p;
        head -> rowSpan = rowSpan_p;
        head -> columnSpan = columnSpan_p;
        head -> nextElement = NULL;
        return;
    }

    currentElement = head;

    while( currentElement -> nextElement != NULL ){
        currentElement = currentElement -> nextElement;
    }

    currentElement -> nextElement = widget;
    currentElement -> nextElement -> row = row_p;
    currentElement -> nextElement -> column = column_p;
    currentElement -> nextElement -> rowSpan = rowSpan_p;
    currentElement -> nextElement -> columnSpan = columnSpan_p;
    currentElement -> nextElement -> nextElement = NULL;

}


