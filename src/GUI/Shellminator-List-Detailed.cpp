/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
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

#include "Shellminator-List-Detailed.hpp"

ShellminatorListDetailed::ShellminatorListDetailed(){
    optionsList = NULL;
    detailList = NULL;
    listSize = 0;
    instruction = NULL;
    selected = 0;
    drawOffset = 0;
    detailStart = 0;
    func = NULL;
}

ShellminatorListDetailed::ShellminatorListDetailed( const char* optionsList_p[], const char* detailList_p[], int listSize_p, const char* instruction_p ){

    if( listSize_p > 32 ){
        listSize_p = 32;
    }

    optionsList = optionsList_p;
    listSize = listSize_p;
    detailList = detailList_p;
    instruction = instruction_p;
    selected = 0;
    drawOffset = 0;
    detailStart = 0;
    func = NULL;
}

void ShellminatorListDetailed::init( Shellminator* parent_p, Stream* channel_p ){

    // Generic counter.
    int i;

    // Store temporary data.
    int tmp;

    // Save data to internal variables.
    parent = parent_p;
    channel = channel_p;
    selected = 0;
    drawOffset = 0;

    // Check if the detailList is configured incorrectly.
    if( detailList == NULL ){
        return;
    }

    // Reset the horizontal start position tracker.
    detailStart = 0;

    // Calculate the horizontal start position for the detail box.
    for( i = 0; i < listSize; i++ ){
        tmp = strlen( optionsList[ i ] ) + 5;
        if( tmp > detailStart ){
            detailStart = tmp;
        }
    }

}

void ShellminatorListDetailed::printExtra( int index ){

    // Generic counter.
    int i;

    // It will hold the number of characters in
    // the actual detail text.
    int detailLength;

    // It will increment every time, when a newline
    // character is found in the detail text.
    int verticalOffset = 0;

    // Check if parameters are configured incorrectly.
    // This case, clear the line to the end.
    if( detailList == NULL ){
        channel -> print( "\033[0K" );
        return;
    }

    if( detailStart == 0 ){
        channel -> print( "\033[0K" );
        return;
    }

    // We don't have to reprint the data every time, just
    // if the index is actually the selected element.
    if( index != selected ){
        return;
    }

    // Print as many elements as high the remaining screen is.
    for( i = 1; i < height; i++ ){

        Shellminator::setCursorPosition( channel, originX + detailStart, originY + i );
        channel -> print( "  \u2502\033[0K" );

    }

    // Calculate text length and set the cursor to start position.
    detailLength = strlen( detailList[ index ] );
    Shellminator::setCursorPosition( channel, originX + detailStart + 4, originY + 1 + verticalOffset );

    // Print as many lines as possible in the current screen size.
    for( i = 0; i < detailLength; i++ ){

        // If the current character is new line,
        // clear the rest of the line and increment
        // the verticalOffset variable.
        if( detailList[ index ][ i ] == '\n' ){
            //channel -> print( "\033[0K" );
            verticalOffset++;

            // If we ran out of screen space, we can stop.
            if( ( verticalOffset + 2 ) > height ){
                break;
            }

            // Set the cursor to the next line beginning.
            Shellminator::setCursorPosition( channel, originX + detailStart + 4, originY + 1 + verticalOffset );
            continue;
        }

        // Otherwise print the character.
        channel -> print( detailList[ index ][ i ] );

    }

    // Increment vertical offset to jump a line.
    verticalOffset++;

    // Clear every line remaining until the screen end.
    for( i = verticalOffset + 2; i <= height; i++ ){
        Shellminator::setCursorPosition( channel, originX + detailStart + 4, originY + i - 1 );
        channel -> print( "\033[0K" );
    }

}
