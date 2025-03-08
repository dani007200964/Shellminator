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

#include "Shellminator.hpp"

void Shellminator::mouseBegin(){

    // Activate mouse according to xterm documentation:
    // https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-Mouse-Tracking
    channel -> print( "\033[?1000h" );
    channel -> print( "\033[?1006h" );

    // Reset the event buffer counter.
    mouseEventBufferCounter = 0;
}

void Shellminator::mouseEnd(){

    // Deactivate mouse according to xterm documentation:
    // https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-Mouse-Tracking
    channel -> print( "\033[?1000l" );
    channel -> print( "\033[?1006l" );

}

void Shellminator::ShellminatorMouseEventParserState( char new_char ){

    // Replace ';' to ' '. It will be easier to parse later.
    if( new_char == ';' ){
        new_char = ' ';
    }

    // Save the new character to the buffer and increment the pointer.
    mouseEventBuffer[ mouseEventBufferCounter ] = new_char;
    mouseEventBufferCounter++;

    // Detect if the terminating m or M character received.
    if( ( new_char == 'm' ) || ( new_char == 'M' ) ){

        // Parse the content of the buffer.
        parseMouseData();

        // Reset the pointer.
        mouseEventBufferCounter = 0;

        // Reset to the default state.
        currentState = &Shellminator::ShellminatorDefaultState;
        return;

    }

    // Buffer overflow detection.
    if( mouseEventBufferCounter >= SHELLMINATOR_MOUSE_PARSER_BUFFER_SIZE ){

        // Reset the pointer.
        mouseEventBufferCounter = 0;

        // Reset to the default state.
        currentState = &Shellminator::ShellminatorDefaultState;

    }

}

void Shellminator::parseMouseData(){

    int xPos;
    int yPos;
    int mouseCode;
    shellEvent_t newEvent;

    bool release;

    // Overflow protection.
    if( mouseEventBufferCounter < 1 ){
        return;
    }

    // Detect if the button is pressed or released.
    release = mouseEventBuffer[ mouseEventBufferCounter - 1 ] == 'm';

    // Replace the end character with string terminator.
    mouseEventBuffer[ mouseEventBufferCounter - 1 ] = '\0';

    sscanf( mouseEventBuffer, "%d %d %d", &mouseCode, &xPos, &yPos );

    switch( mouseCode ){

        // Left mouse.
        case 0:
        newEvent.eventCode = release ? EVENT_CODE_MOUSE_LEFT_RELEASED : EVENT_CODE_MOUSE_LEFT_PRESSED;
        break;

        // Middle mouse.
        case 1:
        newEvent.eventCode = release ? EVENT_CODE_MOUSE_MIDDLE_RELEASED : EVENT_CODE_MOUSE_MIDDLE_PRESSED;
        break;

        // Right mouse.
        case 2:
        newEvent.eventCode = release ? EVENT_CODE_MOUSE_RIGHT_RELEASED : EVENT_CODE_MOUSE_RIGHT_PRESSED;
        break;

        // Mouse wheel up.
        case 64:
        newEvent.eventCode = EVENT_CODE_MOUSE_WHEEL_UP;
        break;

        // Mouse wheel down.
        case 65:
        newEvent.eventCode = EVENT_CODE_MOUSE_WHEEL_DOWN;
        break;

        // Any other means invalid. In this case we have to stop here.
        default:
        return;

    }

    // Fill the rest of the event structure with the parsed data.
    newEvent.type = SHELL_EVENT_MOUSE;
    newEvent.data = 0;
    newEvent.x = xPos;
    newEvent.y = yPos;

    pushEvent( newEvent );

}
