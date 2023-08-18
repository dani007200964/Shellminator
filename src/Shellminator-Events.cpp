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

void Shellminator::pushEvent( shellEvent_t event ){
  
    // This solution uses a circular buffer, so it has two pointers.
    // The write pointer always tracks the next free slot in the buffer.
    // Store the new values there.
    eventBuffer[ eventBufferWritePtr ] = event;

    // Increment the write pointer.
    eventBufferWritePtr++;
    
    // Detect overflow.
    if( eventBufferWritePtr >= EVENT_BUFFER_SIZE ){
        // Handle overflow.
        eventBufferWritePtr = 0;
    }

}

int Shellminator::eventAvailable(){

    // This solution uses a circular buffer, so it has two pointers.
    // One for read and one for writing.
    // If they point to the same place, that means, there is no data available.
	if( eventBufferWritePtr == eventBufferReadPtr ){
		return 0;
	}

    // The first case is, when the Write pointer is ahead of the read pointer.
	else if( eventBufferWritePtr > eventBufferReadPtr ){
		return eventBufferWritePtr - eventBufferReadPtr;
	}

    // The second case is, when the Read pointer is 'ahead' of the write pointer.
    // It happens when overflow event occurs with the circular buffer.
	else{

		return EVENT_BUFFER_SIZE - eventBufferReadPtr + eventBufferWritePtr;

	}

}

Shellminator::shellEvent_t Shellminator::readEvent(){

    // This struct will be returned.
    shellEvent_t ret;

    // By default set the returned event to invalid.
    ret.eventCode = EVENT_CODE_EMPTY;

    // This solution uses a circular buffer, so it has two pointers.
    // One for read and one for writing.
    // If they point to the same place, that means, there is no data available.
    if( eventBufferWritePtr == eventBufferReadPtr ){

        // This case return with invalid event code.
        return ret;

    }

    // Any other case, that means it has at least one new item to be read.
    else{

        // Get the next data from the circular structure.
        ret = eventBuffer[ eventBufferReadPtr ];

    }

    // Return with the next element data.
    return ret;

}

void Shellminator::popEvent(){

    // This solution uses a circular buffer, so it has two pointers.
    // One for read and one for writing.
    // If they point to the same place, that means, there is no data available.
    if( eventBufferWritePtr == eventBufferReadPtr ){

        // This case return.
        return;

    }

    // Any other case, that means it has at least one new item to be read.
    else{

        // Increment the read pointer.
        eventBufferReadPtr++;

        // Check for overflow.
        if( eventBufferReadPtr >= EVENT_BUFFER_SIZE ){

            // Handle overflow.
            eventBufferReadPtr = 0;
        }

    }

}