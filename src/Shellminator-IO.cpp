/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
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

#include "Shellminator-IO.hpp"


#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE

//----- Response for WebSocket Server class -----//

void shellminatorWebSocketChannel::select( WebSocketsServer *server_p, int8_t clientID_p ){

	server = server_p;
	clientID = clientID_p;

}

void shellminatorWebSocketChannel::push( uint8_t data ){

	buffer[ writePointer ] = data;
	writePointer++;
	if( writePointer >= SHELLMINATOR_WEBSOCKET_BUFFER_LEN ){
		writePointer = 0;
	}

}

void shellminatorWebSocketChannel::push( uint8_t* data, size_t size ){

	uint32_t i;

	for( i = 0; i < size; i++ ){

		buffer[ writePointer ] = data[ i ];
		writePointer++;
		if( writePointer >= SHELLMINATOR_WEBSOCKET_BUFFER_LEN ){
			writePointer = 0;
		}

	}

}

int shellminatorWebSocketChannel::available(){

	if( writePointer == readPointer ){
		return 0;
	}

	else if( writePointer > readPointer ){
		return writePointer - readPointer;
	}

	else{

		return SHELLMINATOR_WEBSOCKET_BUFFER_LEN - readPointer + writePointer;

	}

}

int shellminatorWebSocketChannel::read(){

	int ret;

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		ret = (uint8_t)buffer[ readPointer ];
		readPointer++;

		if( readPointer >= SHELLMINATOR_WEBSOCKET_BUFFER_LEN ){
			readPointer = 0;
		}

	}

	return ret;

}

int shellminatorWebSocketChannel::peek(){

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		return (uint8_t)buffer[ readPointer ];

	}

}

void shellminatorWebSocketChannel::flush(){

	// Todo Maybe clear the input buffer?

}

size_t shellminatorWebSocketChannel::write( uint8_t b ){

	if( server ){
		server -> sendTXT( clientID, &b, 1 );
		return 1;
	}
	return 0;

}

size_t shellminatorWebSocketChannel::write( const uint8_t *buffer, size_t size ){

	if( server ){
		server -> sendTXT( clientID, buffer, size );
		return 1;
	}
	return 0;

}

//---- print section ----//

size_t shellminatorWebSocketChannel::print( char c ){

	if( server ){
		server -> sendTXT( clientID, (uint8_t*)&c, 1 );
		return 1;
	}
	return 0;

}

size_t shellminatorWebSocketChannel::print( uint8_t b ){

	char outBuff[10];
	uint32_t dataSize;

	snprintf( outBuff, 10, "%u", (int)b );

	dataSize = strlen( outBuff );

	if( server ){
		server -> sendTXT( clientID, (uint8_t*)outBuff, dataSize );
		return dataSize;
	}
	return 0;

}

size_t shellminatorWebSocketChannel::print( char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );

	if( server ){
		server -> sendTXT( clientID, (uint8_t*)str, dataSize );
		return dataSize;
	}

	return 0;

}

size_t shellminatorWebSocketChannel::print( const char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );

	if( server ){
		server -> sendTXT( clientID, (uint8_t*)str, dataSize );
		return dataSize;
	}
	return 0;

}

int8_t shellminatorWebSocketChannel::getClientID(){

	return clientID;

}

#endif
