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

    // Reset all the counters and pointers.
    mouseEventBufferCntr = 0;
    mouseBufferWritePtr = 0;
    mouseBufferReadPtr = 0;
}

void Shellminator::mouseEnd(){

    // Deactivate mouse according to xterm documentation:
    // https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-Mouse-Tracking
    channel -> print( "\033[?1000l" );
    channel -> print( "\033[?1006l" );

}

void Shellminator::pushMouseEvent( uint8_t x, uint8_t y, uint8_t event ){
  
    // This solution uses a circular buffer, so it has two pointers.
    // The write pointer always tracks the next free slot in the buffer.
    // Store the new values there.
    mouseBuffer[ mouseBufferWritePtr ].x     = x;
    mouseBuffer[ mouseBufferWritePtr ].y     = y;
    mouseBuffer[ mouseBufferWritePtr ].event = event;
    
    // Increment the write pointer.
    mouseBufferWritePtr++;
    
    // Detect overflow.
    if( mouseBufferWritePtr >= MOUSE_BUFFER_SIZE ){
        // Handle overflow.
        mouseBufferWritePtr = 0;
    }

}

int Shellminator::mouseAvailable(){

    // This solution uses a circular buffer, so it has two pointers.
    // One for read and one for writing.
    // If they point to the same place, that means, there is no data available.
	if( mouseBufferWritePtr == mouseBufferReadPtr ){
		return 0;
	}

    // The first case is, when the Write pointer is ahead of the read pointer.
	else if( mouseBufferWritePtr > mouseBufferReadPtr ){
		return mouseBufferWritePtr - mouseBufferReadPtr;
	}

    // The second case is, when the Read pointer is 'ahead' of the write pointer.
    // It happens when overflow event occurs with the circular buffer.
	else{

		return MOUSE_BUFFER_SIZE - mouseBufferReadPtr + mouseBufferWritePtr;

	}

}

Shellminator::mouseEvent_t Shellminator::mouseRead(){

    // This struct will be returned.
    mouseEvent_t ret;

    // By default set the returned event to invalid.
    ret.event = MOUSE_INVALID;

    // This solution uses a circular buffer, so it has two pointers.
    // One for read and one for writing.
    // If they point to the same place, that means, there is no data available.
    if( mouseBufferWritePtr == mouseBufferReadPtr ){

        // This case return with invalid event code.
        return ret;

    }

    // Any other case, that means it has at least one new item to be read.
    else{

        // Get the next data from the circular structure.
        ret = mouseBuffer[ mouseBufferReadPtr ];

        // Increment the read pointer.
        mouseBufferReadPtr++;

        // Check for overflow.
        if( mouseBufferReadPtr >= MOUSE_BUFFER_SIZE ){

            // Handle overflow.
            mouseBufferReadPtr = 0;
        }

    }

    // Return with the next element data.
    return ret;

}

void Shellminator::ShellminatorMouseEventParserState( char new_char ){

    // Replace ';' to ' '. It will be easier to parse later.
    if( new_char == ';' ){
        new_char = ' ';
    }

    // Save the new character to the buffer and increment the pointer.
    mouseEventBuffer[ mouseEventBufferCntr ] = new_char;
    mouseEventBufferCntr++;

    // Detect if the terminatong m or M character received.
    if( ( new_char == 'm' ) || ( new_char == 'M' ) ){

        parseMouseData();

        // Reset the pointer.
        mouseEventBufferCntr = 0;

        // Reset to the default state.
        currentState = &Shellminator::ShellminatorDefaultState;
        return;

    }

    // Buffer overflow detection.
    if( mouseEventBufferCntr >= SHELLMINATOR_MOUSE_PARSER_BUFFER_SIZE ){

        // Reset the pointer.
        mouseEventBufferCntr = 0;

        // Reset to the default state.
        currentState = &Shellminator::ShellminatorDefaultState;

    }

}

void Shellminator::parseMouseData(){

  int xPos;
  int yPos;
  int event;

  bool release;

  // Overflow protection.
  if( mouseEventBufferCntr < 1 ){
    return;
  }

  // Detect if the button is pressed or released.
  release = mouseEventBuffer[ mouseEventBufferCntr - 1 ] == 'm';

  // Replace the end character with string terminator.
  mouseEventBuffer[ mouseEventBufferCntr - 1 ] = '\0';

  sscanf( mouseEventBuffer, "%d %d %d", &event, &xPos, &yPos );

  switch( event ){

    // Left mouse.
    case 0:
      event = release ? MOUSE_LEFT_RELEASED : MOUSE_LEFT_PRESSED;
      break;

    // Middle mouse.
    case 1:
      event = release ? MOUSE_MIDDLE_RELEASED : MOUSE_MIDDLE_PRESSED;
      break;

    // Right mouse.
    case 2:
      event = release ? MOUSE_RIGHT_RELEASED : MOUSE_RIGHT_PRESSED;
      break;

    // Mouse wheel up.
    case 64:
      event = MOUSE_WHEEL_UP;
      break;

    // Mouse wheel down.
    case 65:
      event = MOUSE_WHEEL_DOWN;
      break;

    // Any other means invalid.
    default:
      event = MOUSE_INVALID;
      break;

  }

  pushMouseEvent( xPos, yPos, event );

}
