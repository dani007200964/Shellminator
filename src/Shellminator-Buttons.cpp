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

/*
┌──────┐
│      │
└─────┤─┘
◖◖█

╭──────╮
│      │
╰────┤ ╯


*/

const char ShellminatorButton::topLeftCornerRound[ 4 ]     = "\u256D";
const char ShellminatorButton::topRightCornerRound[ 4 ]    = "\u256E";
const char ShellminatorButton::bottomLeftCornerRound[ 4 ]  = "\u2570";
const char ShellminatorButton::bottomRightCornerRound[ 4 ] = "\u256F";

const char ShellminatorButton::topLeftCornerSquare[ 4 ]     = "\u250C";
const char ShellminatorButton::topRightCornerSquare[ 4 ]    = "\u2510";
const char ShellminatorButton::bottomLeftCornerSquare[ 4 ]  = "\u2514";
const char ShellminatorButton::bottomRightCornerSquare[ 4 ] = "\u2518";

const char ShellminatorButton::horizontalLineDefault[ 4 ] = "\u2500";
const char ShellminatorButton::verticalLineDefault[ 4 ]   = "\u2502";

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

    // The default corner type is square.
    topLeftCorner = topLeftCornerSquare;
    topRightCorner = topRightCornerSquare;
    bottomLeftCorner = bottomLeftCornerSquare;
    bottomRightCorner = bottomRightCornerSquare;

    horizontalLine = horizontalLineDefault;
    verticalLine = verticalLineDefault;

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
        eventText[ 0 ] = (char)event.data;
        eventText[ 1 ] = '\0';
        eventTextSize = 1;
    }

    // Otherwise, we can not enable trigger event functionality.
    else{
        event.type = Shellminator::SHELL_EVENT_EMPTY;
        eventTextSize = 0;
        return;
    }

}

void ShellminatorButton::attachTriggerFunction( void(*func_p)(void) ){
    func = func_p;
}

void ShellminatorButton::setRoundCorners(){
    topLeftCorner     = topLeftCornerRound;
    topRightCorner    = topRightCornerRound;
    bottomLeftCorner  = bottomLeftCornerRound;
    bottomRightCorner = bottomRightCornerRound;
}

void ShellminatorButton::setSquareCorners(){
    topLeftCorner     = topLeftCornerSquare;
    topRightCorner    = topRightCornerSquare;
    bottomLeftCorner  = bottomLeftCornerSquare;
    bottomRightCorner = bottomRightCornerSquare;
}

void ShellminatorButton::setColor( uint8_t color_p ){
    color = color_p;
}

void ShellminatorButton::init( Shellminator* parent_p ){
    parent = parent_p;
    
    bufferingEnabled = true;
    selectedChannel = parent -> getBufferedPrinter();

    if( selectedChannel == NULL ){
        selectedChannel = parent -> channel;
        bufferingEnabled = false;
    }
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
            triggered = true;
            parent -> requestRedraw();
            redraw = true;
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

            triggered = true;
            parent -> requestRedraw();
            redraw = true;
            return;
        }

    }

}

void ShellminatorButton::draw(){

    // Generic counter.
    int i;

    // The upper limit of the for loops calculated
    // to this variable.
    int limit;

    if( parent == NULL ){
        return;
    }

    if( name == NULL ){
        return;
    }

    if( selectedChannel == NULL ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    // print the parts, that has to be drawn every time.
    // Middle line section.
    Shellminator::setCursorPosition( selectedChannel, originX, originY + 1 );

    // Set color to the specified one.
    //Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::REGULAR, color );

    // In case of trigger event, set it to low intensity.
    if( triggered ){
        // Create inverted text.
        //Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::BACKGROUND, color );
        Shellminator::setFormat( selectedChannel, Shellminator::LOW_INTENSITY, color );
    }

    else{
        Shellminator::setFormat( selectedChannel, Shellminator::REGULAR, color );
    }

    // Print the middle section with the text.
    selectedChannel -> print( verticalLine );

    // Calculate text starting position.
    // It has to be centered.
    limit = ( width - textWidth ) / 2;

    // Print as many spaces as needed, to make the text centered.
    // We print spaces to clear the unwanted garbage from the line.
    for( i = 1; i < limit; i++ ){
        selectedChannel -> print( ' ' );
    }

    // Print the name text.
    selectedChannel -> print( name );

    // Calculate how many characters left until line end.
    limit = width - ( limit + textWidth );

    // Print as many spaces as needed, to make the text centered.
    // We print spaces to clear the unwanted garbage from the line.
    for( i = 1; i < limit; i++ ){
        selectedChannel -> print( ' ' );
    }

    // print the right border.
    selectedChannel -> print( verticalLine );

    // Set cursor to origin.
    Shellminator::setCursorPosition( selectedChannel, originX, originY );

    // Print the top part.
    selectedChannel -> print( topLeftCorner );
    for( i = 1; i < ( width - 1 ); i++ ){
        selectedChannel -> print( horizontalLine );
    }
    selectedChannel -> print( topRightCorner );

    // Bottom line.
    Shellminator::setCursorPosition( selectedChannel, originX, originY + 2 );

    // Print the bottom part.
    selectedChannel -> print( bottomLeftCorner );

    // Check if a key event is attached. In this case the triggering key
    // will be displayed on the bottom right.
    if( event.type != Shellminator::SHELL_EVENT_EMPTY ){
        limit = eventTextSize + 2;
    }

    // Otherwise a regular corner will be printed.
    else{
        limit = 1;
    }

    // Print as many horizontal characters as needed.
    for( i = 1; ( i < width - limit ); i++ ){
        selectedChannel -> print( horizontalLine );
    }

    // Check if a key name has to be displayed.
    if( event.type != Shellminator::SHELL_EVENT_EMPTY ){
        selectedChannel -> print( "\u2524" );

        // Print the triggering key name.
        selectedChannel -> print( eventText );

        // Set the format back to regular an finish the line.
        Shellminator::setFormat( selectedChannel, Shellminator::REGULAR, color );
        selectedChannel -> print( "\u2502" );
    }

    else{
        // Set the format back to regular an finish the line.
        selectedChannel -> print( "\u2518" );
    }

    if( bufferingEnabled ){
        selectedChannel -> flush();
    }

    if( triggered ){

        if( func != NULL ){
            func();
        }

        parent -> requestRedraw();
        redraw = true;

        triggered = false;
    }

}