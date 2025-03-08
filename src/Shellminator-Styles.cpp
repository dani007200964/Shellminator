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

    hideCursor( channel );

}

void Shellminator::hideCursor( Stream *stream_p ){

    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( __CONST_TXT__( "\033[?25l" ) );

}

void Shellminator::showCursor(){

    showCursor( channel );

}

void Shellminator::showCursor( Stream *stream_p ){

    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( __CONST_TXT__( "\033[?25h" ) );

}

void Shellminator::clear() {

  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  channel -> print( __CONST_TXT__( "\033[H\033[J" ) );  // VT100 Home command

}

void Shellminator::setFormat( Stream *stream_p, int firstArg ){
    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( __CONST_TXT__( "\033[" ) );
    stream_p -> print( firstArg );
    stream_p -> print( 'm' );
}

void Shellminator::setFormat( Stream *stream_p, int firstArg, int secondArg ){
    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( __CONST_TXT__( "\033[" ) );
    stream_p -> print( firstArg );
    stream_p -> print( ';' );
    stream_p -> print( secondArg );
    stream_p -> print( 'm' );
}

void Shellminator::setFormat( Stream *stream_p, int firstArg, int secondArg, int thirdArg ){
    if( stream_p == NULL ){
        return;
    }

    stream_p -> print( __CONST_TXT__( "\033[" ) );
    stream_p -> print( firstArg );
    stream_p -> print( ';' );
    stream_p -> print( secondArg );
    stream_p -> print( ';' );
    stream_p -> print( thirdArg );
    stream_p -> print( 'm' );
}

void Shellminator::format( Stream *stream_p, int firstArg ){
    if( enableFormatting ){
        setFormat( stream_p, firstArg );
    }
}

void Shellminator::format( Stream *stream_p, int firstArg, int secondArg ){
    if( enableFormatting ){
        setFormat( stream_p, firstArg, secondArg );
    }
}

void Shellminator::format( Stream *stream_p, int firstArg, int secondArg, int thirdArg ){
    if( enableFormatting ){
        setFormat( stream_p, firstArg, secondArg, thirdArg );
    }
}


