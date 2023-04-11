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

#include "testStream.hpp"

void testStream::pushTx( uint8_t data ){

	txBuffer[ txWritePointer ] = data;
	txWritePointer++;
	if( txWritePointer >= TEST_STREAM_BUFFER_SIZE ){
		txWritePointer = 0;
	}

}

void testStream::pushTx( uint8_t* data, size_t size ){

	uint32_t i;

	for( i = 0; i < size; i++ ){

		txBuffer[ txWritePointer ] = data[ i ];
		txWritePointer++;
		if( txWritePointer >= TEST_STREAM_BUFFER_SIZE ){
			txWritePointer = 0;
		}

	}

}

void testStream::pushRx( uint8_t data ){

	rxBuffer[ rxWritePointer ] = data;
	rxWritePointer++;
	if( rxWritePointer >= TEST_STREAM_BUFFER_SIZE ){
		rxWritePointer = 0;
	}

}

void testStream::pushRx( uint8_t* data, size_t size ){

	uint32_t i;

	for( i = 0; i < size; i++ ){

		rxBuffer[ rxWritePointer ] = data[ i ];
		rxWritePointer++;
		if( rxWritePointer >= TEST_STREAM_BUFFER_SIZE ){
			rxWritePointer = 0;
		}

	}

}

void testStream::clearTx(){

	uint32_t i;

	for( i = 0; i < TEST_STREAM_BUFFER_SIZE; i++ ){
		txBuffer[ i ] = 0;
	}

	txWritePointer = 0;
	txReadPointer = 0;

}

void testStream::clearRx(){

	uint32_t i;

	for( i = 0; i < TEST_STREAM_BUFFER_SIZE; i++ ){
		rxBuffer[ i ] = 0;
	}

	rxWritePointer = 0;
	rxReadPointer = 0;

}

int testStream::available(){

	if( rxWritePointer == rxReadPointer ){
		return 0;
	}

	else if( rxWritePointer > rxReadPointer ){
		return rxWritePointer - rxReadPointer;
	}

	else{

		return TEST_STREAM_BUFFER_SIZE - rxReadPointer + rxWritePointer;

	}

}

int testStream::read(){

	int ret;

	if( rxWritePointer == rxReadPointer ){

		return -1;

	}

	else{

		ret = (uint8_t)rxBuffer[ rxReadPointer ];
		rxReadPointer++;

		if( rxReadPointer >= TEST_STREAM_BUFFER_SIZE ){
			rxReadPointer = 0;
		}

	}

	return ret;

}

int testStream::peek(){

	if( rxWritePointer == rxReadPointer ){

		return -1;

	}

	else{

		return (uint8_t)rxBuffer[ rxReadPointer ];

	}

}

void testStream::flush(){

	// Todo Maybe clear the input buffer?

}

size_t testStream::write( uint8_t b ){

	pushTx( b );
	return 1;

}

size_t testStream::write( const uint8_t *buff, size_t size ){

    uint32_t i;
    for( i = 0; i < size; i++ ){

		pushTx( buff[ i ] );

    }
	return 1;

}

//---- print section ----//

size_t testStream::print( char c ){

	pushTx( c );
	return 1;

}

size_t testStream::print( uint8_t b ){

	char buff[ 20 ];

	snprintf( buff, sizeof( buff ), "%u", (int)b );
	pushTx( (uint8_t*)buff, strlen( buff ) );
	return 1;

}

size_t testStream::print( char *str ){

	uint32_t dataSize;

	dataSize = strlen( (const char*)str );
	pushTx( (uint8_t*)str, dataSize );

	return dataSize;

}

size_t testStream::print( const char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );
	pushTx( (uint8_t*)str, dataSize );

	return dataSize;

}