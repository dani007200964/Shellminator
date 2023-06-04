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
  acceleratorBuffer = NULL;
  bufferSize = -1;

}

ShellminatorBufferedPrinter::ShellminatorBufferedPrinter( Stream* channel_p ){

  channel = channel_p;
  acceleratorBuffer = NULL;
  bufferSize = -1;

}

bool ShellminatorBufferedPrinter::allocate( int bufferSize_p ){

  // We need at least 30 characters to work properly.
  if( bufferSize_p < 30 ){
    bufferSize = -1;
    return false;
  }

  bufferSize = bufferSize_p;

  availableCharacters = bufferSize;
  acceleratorBuffer = (char*)malloc( bufferSize * sizeof( char ) );
  acceleratorBufferPointer = (char*)acceleratorBuffer;

  /*
  char buff[ 200 ];
  snprintf( buff, sizeof( buff ), "allocated address: %p\r\n", acceleratorBuffer );
  channel -> print( buff );
  */

  if( acceleratorBuffer == NULL ){
    // Memory allocation failed!
    bufferSize = -1;
    return false;
  }

  clearBuffer();
  return true;

}

void ShellminatorBufferedPrinter::deallocate(){

  if( acceleratorBuffer != NULL ){

    free( (void*)acceleratorBuffer );

    /*
    char buff[ 200 ];
    snprintf( buff, sizeof( buff ), "free address: %p\r\n", acceleratorBuffer );
    channel -> print( buff );
    */

    acceleratorBuffer = NULL;
    bufferSize = -1;

  }

}

ShellminatorBufferedPrinter::~ShellminatorBufferedPrinter(){

  deallocate();

}


void ShellminatorBufferedPrinter::setChannel( Stream* channel_p ){

  channel = channel_p;

}

Stream* ShellminatorBufferedPrinter::getChannel(){

  return channel;

}

int ShellminatorBufferedPrinter::printf( const char *fmt, ... ){

  va_list args;

  int status;

  char* pointerBeforePrint;

  // Save the pointer position before any printing.
  // If the buffer gets full, we have to protect the
  // data already in the buffer.
  pointerBeforePrint = acceleratorBufferPointer; 

  // Start the variadic list.
  va_start( args, fmt );

  // If memory allocation failed with the constructor.
  if( ( bufferSize < 0 ) || ( channel == NULL ) ){

    // Close the variadic list and return.
    va_end( args );
    return 0;

  }

  // Try to print the whole data into the buffer.
  status = vsnprintf( acceleratorBufferPointer, availableCharacters, fmt, args );
  //wprintf( L"Available characters: %d\r\n", availableCharacters );

  // Check if we ran out of memory.
  if( status >= availableCharacters ){

    // We possibly ran out of buffer.
    // Flush the buffer and try again.
    flush();

    // Try again with empty buffer.
    status = vsnprintf( acceleratorBufferPointer, availableCharacters, fmt, args );

    // Check the memory again.
    if( status >= availableCharacters ){

      // If we got a problem with empty buffer, we can not print this data,
      // because the buffer is too small.
      
      // Reset the pointer and the counter for the next transaction.
      acceleratorBufferPointer = acceleratorBuffer;
      availableCharacters = bufferSize;

      va_end( args );
      return 0;

    }

  }

  acceleratorBufferPointer += status;
  availableCharacters -= status;


  va_end( args );

  return status;

}

void ShellminatorBufferedPrinter::flush(){

  // If memory allocation failed with the constructor.
  if( ( bufferSize < 0 ) || ( channel == NULL ) ){

    return;

  }

  // Terminate the string.
  *acceleratorBufferPointer = '\0';

  // Print the data.
  channel -> print( acceleratorBuffer );

  // Reset the pointer and the counter.
  acceleratorBufferPointer = acceleratorBuffer;
  availableCharacters = bufferSize;

  clearBuffer();

}

void ShellminatorBufferedPrinter::clearBuffer(){

  int i;

  // If memory allocation failed with the constructor.
  if( ( bufferSize < 0 ) || ( channel == NULL ) ){

    return;

  }

  for( i = 0; i < bufferSize; i++ ){

    acceleratorBuffer[ i ] = '\0';

  }

}

int ShellminatorBufferedPrinter::getBufferSize(){

  return bufferSize;

}
