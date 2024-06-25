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

#include "Shellminator-Buttons.hpp"


ShellminatorButton::ShellminatorButton(){

    name = NULL;
    func = NULL;
    width = 1;
    height = 1;

}

ShellminatorButton::ShellminatorButton( const char* name_p ){

    if( name_p == NULL ){
        return;
    }

    // Save the parameters to internal variables.
    name = name_p;

    // Set the origin to default.
    originX = 1;
    originY = 1;

    width = 1;
    height = 1;

    func = NULL;

    // Set the eventCode to empty. This means
    // that the button wont trigger to any event.
    event.eventCode = Shellminator::EVENT_CODE_EMPTY;

    // Calculate the text size.
    textWidth = strlen( name_p );

}

void ShellminatorButton::attachEvent( Shellminator::shellEvent_t event_p ){

    // Save the trigger event struct.
    event = event_p;

    // If trigger event is a regular key, save the data to internal variables.
    if( event_p.type == Shellminator::SHELL_EVENT_KEY ){
        if( isUpper( (char)event.data ) ){
            eventText[ 0 ] = 0xE2;
            eventText[ 1 ] = 0x86;
            eventText[ 2 ] = 0x91;
            eventText[ 3 ] = ' ';
            eventText[ 4 ] = (char)event.data;
            eventText[ 5 ] = '\0';
            eventTextSize = 3;
        }

        else if( isLower( (char)event.data ) ){
            eventText[ 0 ] = toUpper( (char)event.data );
            eventText[ 1 ] = '\0';
            eventTextSize = 1;
        }

        else{
            event.type = Shellminator::SHELL_EVENT_EMPTY;
            eventTextSize = 0;
            return;
        }

    }

    // Otherwise, we can not enable trigger event functionality.
    else{
        event.type = Shellminator::SHELL_EVENT_EMPTY;
        eventTextSize = 0;
        return;
    }

}

void ShellminatorButton::attachTriggerFunction( void(*func_p)(ShellminatorScreen*) ){
    func = func_p;
}

void ShellminatorButton::setColor( Shellminator::textColor_t color_p ){
    color = color_p;
    redraw = true;
}

void ShellminatorButton::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
}

void ShellminatorButton::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    // If the user specified width is too small,
    // overwrite it with valid configuration.
    if( width < ( textWidth + 4 ) ){
        width = textWidth + 4;
    }

    // The height must be exactly 3.
    if( height != 3 ){
        height = 3;
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

    // If the event is key related, we have to check if the key
    // matches with the attached key.
    if( ( newEvent.type == Shellminator::SHELL_EVENT_KEY ) && ( newEvent.type == event.type ) ){
        if( newEvent.data == event.data ){
            if( func != NULL ){
                func( this );
            }
            return;
        }
    }

    // Check if the event was a left mouse release.
    if( ( newEvent.type == Shellminator::SHELL_EVENT_MOUSE ) && ( newEvent.eventCode == Shellminator::EVENT_CODE_MOUSE_LEFT_RELEASED ) ){

        // Check if the event happened in the area of the button.
        if( ( newEvent.x >= originX ) &&
            ( newEvent.y >= originY ) &&
            ( newEvent.x < ( originX + width ) ) &&
            ( newEvent.y < ( originY + height ) ) ){

            if( func != NULL ){
                func( this );
            }
            return;
        }

    }

}

void ShellminatorButton::draw(){

    // Generic counter.
    int i;

    int printedEventTextSize = 0;

    // The upper limit of the for loops calculated
    // to this variable.
    int limit;

    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( name == NULL ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    // Top line section.
    Shellminator::setCursorPosition( channel, originX, originY );

    parent -> format_m( channel, Shellminator::REGULAR, color );

    channel -> print( __CONST_TXT__( "\u250C" ) );

    for( i = 2; i < width; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" ) );
    }

    channel -> print( __CONST_TXT__( "\u2510" ) );

    // Middle line section.
    Shellminator::setCursorPosition( channel, originX, originY + 1 );

    channel -> print( __CONST_TXT__( "\u2502" ) );

    if( eventTextSize > 0 ){
        printedEventTextSize = eventTextSize + 6;
    }
    limit = ( width - ( textWidth + 3 + printedEventTextSize ) ) / 2;
    for( i = 1; i < limit; i++ ){
        channel -> print( ' ' );
    }

    parent -> format_m( channel, Shellminator::BOLD, Shellminator::WHITE );

    channel -> print( __CONST_TXT__( "\u2BA9  " ) );
    channel -> print( name );

    if( eventTextSize > 0 ){
        channel -> print( __CONST_TXT__( "  [ " ) );
        channel -> print( eventText );
        channel -> print( __CONST_TXT__( " ]" ) );
    }

    parent -> format_m( channel, Shellminator::REGULAR, color );

    limit = width - ( limit + textWidth + 3 + printedEventTextSize );
    for( i = 1; i < limit; i++ ){
        channel -> print( ' ' );
    }

    channel -> print( __CONST_TXT__( "\u2502" ) );

    // Bottom line section.
    Shellminator::setCursorPosition( channel, originX, originY + 2 );

    channel -> print( __CONST_TXT__( "\u2514" ) );

    for( i = 2; i < width; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" ) );
    }

    channel -> print( __CONST_TXT__( "\u2518" ) );

}

bool ShellminatorButton::isUpper( char c ){
    return ( c >= 'A' ) && ( c <= 'Z' );
}

bool ShellminatorButton::isLower( char c ){
    return ( c >= 'a' ) && ( c <= 'z' );
}

char ShellminatorButton::toUpper( char c ){
    if( isLower( c ) ){
        // Check ASCII table to decode this magic.
        return c - ( 'a' - 'A' );
    }
    return c;
}

