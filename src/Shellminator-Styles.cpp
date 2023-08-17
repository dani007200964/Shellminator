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

void Shellminator::hideCursor(){

  if( !enableFormatting ){
    return;
  }

  hideCursor( channel );

}

void Shellminator::hideCursor( Stream *stream_p ){

  if( stream_p == NULL ){

    return;

  }

  stream_p -> print( (const char*)"\033[?25l" );

}

void Shellminator::showCursor(){

  if( !enableFormatting ){
    return;
  }

  showCursor( channel );

}

void Shellminator::showCursor( Stream *stream_p ){

  if( stream_p == NULL ){

    return;

  }

  stream_p -> print( (const char*)"\033[?25h" );

}

void Shellminator::clear() {

  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[H" );  // VT100 Home command
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[J" );  // VT100 screen erase command

}

void Shellminator::setFormatFunc( Stream *stream_p, int firstArg, ... ){

    va_list list;
    int next;

    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( "\033[" );
    stream_p -> print( firstArg );

    va_start( list, firstArg );
    
    next = va_arg( list, int );
    while( next >= 0 ){
        stream_p -> print( ';' );
        stream_p -> print( next );
        next = va_arg( list, int );
    }

    stream_p -> print( 'm' );

    va_end( list );

}

void Shellminator::formatFunc( int firstArg, ... ){

    va_list args;

    va_start( args, firstArg );

    setFormatFunc( channel, firstArg, args );

    va_end( args );

}


