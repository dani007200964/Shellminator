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

#include "Shellminator-List.hpp"

ShellminatorList::ShellminatorList(){

    optionsList = NULL;
    listSize = 0;
    instruction = NULL;
    selected = 0;
    drawOffset = 0;
    func = NULL;
}

ShellminatorList::ShellminatorList( const char* optionsList_p[], int listSize_p, const char* instruction_p ){

    if( listSize_p > 32 ){
        listSize_p = 32;
    }

    optionsList = optionsList_p;
    listSize = listSize_p;
    instruction = instruction_p;
    selected = 0;
    drawOffset = 0;
    func = NULL;
}

void ShellminatorList::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
    selected = 0;
    drawOffset = 0;
    redraw = true;
}

void ShellminatorList::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    if( height > ( listSize + 1 ) ){
        height = listSize + 1;
    }

    newEvent = parent -> readEvent();

    // In case of empty event, we can't do anything.
    if( newEvent.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_CODED_KEY ){

        if( newEvent.eventCode == Shellminator::EVENT_CODE_UP_ARROW ){
            selected--;
            if( ( selected < drawOffset ) && ( drawOffset > 0 ) ){
                drawOffset--;
            }
            if( selected < 0 ){
                selected = 0;
            }
            redraw = true;
            parent -> requestRedraw();
            return;
        }

        if( newEvent.eventCode == Shellminator::EVENT_CODE_DOWN_ARROW ){
            selected++;
            if( ( selected > ( height - 2 + drawOffset ) ) && ( drawOffset < ( listSize - ( height - 1 ) ) ) ){
                drawOffset++;
            }
            if( selected >= listSize ){
                selected = listSize - 1;
            }
            redraw = true;
            parent -> requestRedraw();
            return;
        }

        if( newEvent.eventCode == Shellminator::EVENT_CODE_RETURN ){
            if( func == NULL ){
                return;
            }
            func( optionsList, listSize, selected, this );
            return;
        }

    }

}

void ShellminatorList::attachCallback( void(*func_p)(const char*[], int, int, ShellminatorScreen*) ){
    func = func_p;
}

void ShellminatorList::printExtra( int index, bool noClear ){
    if( !noClear ){
        channel -> print( "\033[0K" );
    }
}


void ShellminatorList::draw( bool noClear ){

    // Generic counter.
    int i;

    // Used to safe array indexing.
    int index;

    // Check for the necessary parameters.
    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( optionsList == NULL ){
        return;
    }

    if( listSize == 0 ){
        return;
    }

    if( instruction == NULL ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    
    redraw = false;

    parent -> format( channel, Shellminator::REGULAR, Shellminator::WHITE );

    // Set cursor to top left and print the instruction text.
    Shellminator::setCursorPosition( channel, originX, originY );
    channel -> print( instruction );
    if( !noClear ){
        channel -> print( "\033[0K" );
    }

    // Print as many elements as high the remaining screen is.
    for( i = 1; i < height; i++ ){

        // Calculate the index of the current option.
        index = ( i - 1 ) + drawOffset;

        // Set cursor to the current options location.
        Shellminator::setCursorPosition( channel, originX, originY + i );

        // Check if the current option is the selected one.
        if( index == selected ){
            // If selected change the format to
            // background to highlight the selection.
            // Also print some graphics to the beginning.
            parent -> format( channel, Shellminator::BACKGROUND, Shellminator::WHITE );
            channel -> print( "\u2BA9 " );
        }

        else{
            // Otherwise, just print blank spaces.
            channel -> print( "  " );
        }

        // If we are not at the top of the list,
        // we have to indicate that, we have more
        // options 'hidden' upwards.
        if( ( i == 1 ) && ( drawOffset > 0 ) ){
            channel -> print( "\u2191 " );
        }

        // If we are not at the bottom of the list,
        // we have to indicate that, we have more
        // options 'hidden' downwards.
        else if( ( i == ( height - 1 ) ) && ( drawOffset < ( listSize - ( height - 1 ) ) ) ){
            channel -> print( "\u2193 " );
        }

        // Otherwise, we just print blank spaces.
        else{
            channel -> print( "  " );            
        }

        // Print the option with a blak space at the end.
        channel -> print( optionsList[ index ] );
        channel -> print( ' ' );

        // Check if the current option is the selected one.
        if( index == selected ){
            // If selected, we have to change back the formatting to regular.
            parent -> format( channel, Shellminator::REGULAR, Shellminator::WHITE );
        }

        printExtra( index, noClear );

    }

}