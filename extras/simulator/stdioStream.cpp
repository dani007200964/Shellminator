/*
 * Created on April 08 2023
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.04.08
*/

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

#include "stdioStream.hpp"

void stdioStream::push( uint8_t data ){

	buffer[ writePointer ] = data;
	writePointer++;
	if( writePointer >= STDIO_STREAM_BUFFER_SIZE ){
		writePointer = 0;
	}

}

void stdioStream::push( uint8_t* data, size_t size ){

	uint32_t i;

	for( i = 0; i < size; i++ ){

		buffer[ writePointer ] = data[ i ];
		writePointer++;
		if( writePointer >= STDIO_STREAM_BUFFER_SIZE ){
			writePointer = 0;
		}

	}

}

void stdioStream::update(){

    int key;

    while( kbhit() ){

        key = getch();

        // According to Microsoft documentation it have to be checked for special commands.
        // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch?view=msvc-140
        if( ( key == 0x00 ) || ( key == 0xE0 ) ){

            key = getch();

            switch( key ){
                
                // Left Arrow
                case 75:
                    push( (uint8_t*)"\033[D", 3 );
                break;

                // Right Arrow
                case 77:
                    push( (uint8_t*)"\033[C", 3 );
                break;
                
                // Up Arrow
                case 72:
                    push( (uint8_t*)"\033[A", 3 );
                break;
                
                // Down Arrow
                case 80:
                    push( (uint8_t*)"\033[B", 3 );
                break;
                
                // Del
                case 83:
                    push( (uint8_t*)"\033[3~", 4 );
                break;
                
                // Page Up
                case 73:
                    push( (uint8_t*)"\033[5~", 4 );
                break;

                // Page Down
                case 81:
                    push( (uint8_t*)"\033[6~", 4 );
                break;

            }

            // printf( "Special character: %d\r\n", key );

        }

        // Otherwise, it is a regular character
        else{

            push( key );

        }

    }

}

int stdioStream::available(){

    update();

	if( writePointer == readPointer ){
		return 0;
	}

	else if( writePointer > readPointer ){
		return writePointer - readPointer;
	}

	else{

		return STDIO_STREAM_BUFFER_SIZE - readPointer + writePointer;

	}

}

int stdioStream::read(){

	int ret;

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		ret = (uint8_t)buffer[ readPointer ];
		readPointer++;

		if( readPointer >= STDIO_STREAM_BUFFER_SIZE ){
			readPointer = 0;
		}

	}

	return ret;

}

int stdioStream::peek(){

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		return (uint8_t)buffer[ readPointer ];

	}

}

void stdioStream::flush(){

	// Todo Maybe clear the input buffer?

}

size_t stdioStream::write( uint8_t b ){

    wprintf( L"%c", (char)b );
	return 1;

}

size_t stdioStream::write( const uint8_t *buff, size_t size ){

    uint32_t i;
    for( i = 0; i < size; i++ ){

        wprintf( L"%c", buff[ i ] );

    }
	return size;

}

size_t stdioStream::write( const char *str ){

  	mbstowcs( vOut, str, 10000 );

  	wprintf( L"%ls", vOut );
	return strlen( str );

}