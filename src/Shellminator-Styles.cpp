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

void Shellminator::setTerminalCharacterColor( uint8_t style, uint8_t color ) {

  if( !enableFormatting ){

    return;

  }

  if( style == 0 ){
    style = REGULAR;
  }

  if( color == 0 ){
    style = WHITE;
  }

  // The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( style );
  channel -> print( ';' );
  channel -> print( color );
  channel -> print( 'm' );

}

void Shellminator::setTerminalCharacterColor( char* buff, uint8_t style, uint8_t color ){

  if( buff == NULL ){

    return;

  }

  if( !enableFormatting ){

    *buff='\0';
    return;

  }

  if( style == 0 ){
    style = REGULAR;
  }

  if( color == 0 ){
    style = WHITE;
  }

  sprintf( buff, "\033[%d;%dm", style, color );

}

void Shellminator::setTerminalCharacterColor( char* buff, uint8_t buffSize, uint8_t style, uint8_t color ){

  if( buff == NULL ){

    return;

  }

  if( style == 0 ){
    style = REGULAR;
  }

  if( color == 0 ){
    style = WHITE;
  }

  snprintf( buff, buffSize, "\033[%d;%dm", style, color );

}

void Shellminator::setTerminalCharacterColor( Stream *stream_p, uint8_t style, uint8_t color ){

  if( stream_p == NULL ){

    return;

  }

  if( style == 0 ){
    style = REGULAR;
  }

  if( color == 0 ){
    style = WHITE;
  }

  // The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  stream_p -> write( 27 );
  stream_p -> print( '[' );
  stream_p -> print( style );
  stream_p -> print( ';' );
  stream_p -> print( color );
  stream_p -> print( 'm' );

}

void Shellminator::hideCursor(){

  if( !enableFormatting ){

    return;

  }

  channel -> print( (const char*)"\033[?25l" );

}

void Shellminator::hideCursor( char* buff, uint8_t bufferSize ){

  if( buff == NULL ){

    return;

  }

  snprintf( buff, bufferSize, "\033[?25l" );

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

  channel -> print( (const char*)"\033[?25h" );

}

void Shellminator::showCursor( char* buff, uint8_t buffSize ){

  if( buff == NULL ){

    return;

  }

  snprintf( buff, buffSize, "\033[?25h" );

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
