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
    width = 0;
    height = 0;

}

ShellminatorButton::ShellminatorButton( const char* name_p, void(*func_p)(void), int width_p, int height_p ){

    name = name_p;
    func = func_p;

    width = width_p;
    height = height_p;

    originX = 1;
    originY = 1;

    event.eventCode = Shellminator::EVENT_CODE_EMPTY;

    textWidth = strlen( name_p );

    if( width < ( textWidth + 4 ) ){
        width = textWidth + 4;
    }

}

void ShellminatorButton::attachEvent( Shellminator::shellEvent_t event_p ){

    event = event_p;

    if( event_p.type == Shellminator::SHELL_EVENT_KEY ){
        eventText[ 0 ] = (char)event.data;
        eventText[ 1 ] = '\0';
        eventTextSize = 1;
    }

    else{
        event.type = Shellminator::SHELL_EVENT_EMPTY;
        eventTextSize = 0;
        return;
    }

}

void ShellminatorButton::setEventModeAuto(){
    eventAutoCheck = true;
}
void ShellminatorButton::setEventModeManual(){
    eventAutoCheck = false;
}

void ShellminatorButton::checkEvent( Shellminator::shellEvent_t event_p ){

    //triggered = true;

    if( event_p.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( event.type == Shellminator::SHELL_EVENT_KEY ){
        if( event_p.data == event.data ){
            triggered = true;
            return;
        }
    }

}

void ShellminatorButton::setOrigin( int x, int y ){
    originX = x;
    originY = y;

    if( originX < 1 ){
        originX = 1;
    }

    if( originY < 1 ){
        originY = 1;
    }
}

void ShellminatorButton::init( Shellminator* parent_p ){

    parent = parent_p;

}

/*
┌──────┐
│      │
└─────┤─┘
◖◖█
*/

void ShellminatorButton::draw( int width_p, int  height_p ){

    int i;
    int limit;
    Shellminator::shellEvent_t parentEvent;

    if( parent == NULL ){
        return;
    }

    if( name == NULL ){
        return;
    }

    // Check if 
    if( eventAutoCheck ){
        if( parent -> eventAvailable() ){
            parentEvent = parent -> readEvent();
            checkEvent( parentEvent );

            // Check if the event was a left mouse release.
            if( ( parentEvent.type == Shellminator::SHELL_EVENT_MOUSE ) && ( parentEvent.eventCode == Shellminator::EVENT_CODE_MOUSE_LEFT_RELEASED ) ){

                // Check if the event happened in rhe area of the button.
                if( ( parentEvent.x >= originX ) && ( parentEvent.y >= originY ) && ( parentEvent.x < ( originX + width ) ) && ( parentEvent.y < ( originY + height ) ) ){
                    triggered = true;
                }

            }
        }
    }

    parent -> setCursorPosition( originX, originY );
    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );

    // Print the top part.
    parent -> channel -> print( "\u250C" );
    for( i = 1; i < ( width - 1 ); i++ ){
        parent -> channel -> print( "\u2500" );
    }
    parent -> channel -> println( "\u2510" );

    // Print the middle section with the text.
    parent -> channel -> print( "\u2502" );
    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::BACKGROUND, Shellminator::WHITE );

    if( triggered ){
        Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::LOW_INTENSITY, Shellminator::WHITE );
    }

    limit = ( width - textWidth - 2 ) / 2;
    for( i = 0; i < limit; i++ ){
        parent -> channel -> print( ' ' );
    }

    parent -> channel -> print( name );

    limit = ( width - textWidth - 2 ) - limit;
    for( i = 0; i < limit; i++ ){
        parent -> channel -> print( ' ' );
    }

    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    parent -> channel -> println( "\u2502" );

    // Print the bottom part.
    parent -> channel -> print( "\u2514" );

    if( event.type != Shellminator::SHELL_EVENT_EMPTY ){
        limit = eventTextSize + 2;
    }

    else{
        limit = 1;
    }

    for( i = 1; ( i < width - limit ); i++ ){
        parent -> channel -> print( "\u2500" );
    }

    if( event.type != Shellminator::SHELL_EVENT_EMPTY ){
        parent -> channel -> print( "\u2524" );

        if( triggered ){
            Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::BACKGROUND, Shellminator::WHITE );
        }

        parent -> channel -> print( eventText );

        Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
        parent -> channel -> print( "\u2502" );
    }

    else{
        parent -> channel -> print( "\u2518" );
    }

    triggered = false;

}