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

#include "Shellminator-BufferedPrinter.hpp"

ShellminatorBufferedPrinter::ShellminatorBufferedPrinter(){

  channel = NULL;
  buffer = NULL;
  bufferSize = 0;

}

ShellminatorBufferedPrinter::ShellminatorBufferedPrinter( Stream* channel_p, uint8_t* buffer_p, int bufferSize_p ){

    channel = channel_p;
    buffer = buffer_p;
    bufferSize = bufferSize_p;
    bufferPointer = 0;

    clearBuffer();

}

int ShellminatorBufferedPrinter::available(){
    return 0;
}

int ShellminatorBufferedPrinter::read(){
    return -1;
}

int ShellminatorBufferedPrinter::peek(){
    return -1;
}

void ShellminatorBufferedPrinter::clearBuffer(){
    int i;

    for( i = 0; i < bufferSize; i++ ){
        buffer[ i ] = '\0';
    }   
}

void ShellminatorBufferedPrinter::flush(){
    // Terminate the buffer.
    //buffer[ bufferPointer ] = '\0';
    //channel -> print( buffer );
    //bufferPointer = 0;
    /*
    int i;

    for( i = 0; i < bufferPointer; i++ ){
        channel -> write( buffer[ i ] );
    }
    */

    channel -> write( buffer, bufferPointer );
    bufferPointer = 0;

}

size_t ShellminatorBufferedPrinter::write( uint8_t b ){

    // Handle incorrect buffer size or channel.
    if( ( bufferSize < 1 ) || ( channel == NULL ) ){

        // This case we have to return.
        return 0;

    }

    // Save the data to the next free slot in the buffer.
    buffer[ bufferPointer ] = b;

    // Increment the buffer pointer.
    bufferPointer++;

    // If the pointer reaches the last possible element,
    // we have to flush the output buffer and reset the bufferPointer.
    if( bufferPointer >= ( bufferSize - 1 ) ){

        flush();

    }

    return 1;

}

size_t ShellminatorBufferedPrinter::write( const uint8_t *data, size_t size ){

    int i;

    // Handle incorrect buffer size or channel.
    if( ( bufferSize < 1 ) || ( channel == NULL ) ){

        // This case we have to return.
        return 0;

    }

    for( i = 0; i < size; i++ ){
        // Save the current character in the buffer to the next free slot in the buffer.
        buffer[ bufferPointer ] = data[ i ];

        // Increment the buffer pointer.
        bufferPointer++;

        if( bufferPointer >= ( bufferSize - 1 ) ){

            flush();

        }


    }

    return size;

}
