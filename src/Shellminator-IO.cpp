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

#ifdef SHELLMINATOR_USE_ARDUINO_SERIAL

//----- Response for Arduino Serial Class -----//

void shellminatorArduinoSerialChannel::select( HardwareSerial *serialPort_p ){

	serialPort = serialPort_p;

}

int shellminatorArduinoSerialChannel::available(){

	if( serialPort ) return serialPort -> available();
	return 0;

}

int shellminatorArduinoSerialChannel::read(){

	if( serialPort ) return serialPort -> read();
	return -1;

}

int shellminatorArduinoSerialChannel::peek(){

	if( serialPort ) return serialPort -> peek();
	return -1;

}

void shellminatorArduinoSerialChannel::flush(){

	if( serialPort ) return serialPort -> flush();

}

size_t shellminatorArduinoSerialChannel::write( uint8_t b ){


	if( serialPort ) return serialPort -> write( b );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( uint8_t b ){


	if( serialPort ) return serialPort -> print( b );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( char c ){

	if( serialPort ) return serialPort -> print( (char)c );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( char *str ){

	if( serialPort ) return serialPort -> print( (char*)str );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( const char *str ){

	if( serialPort ) return serialPort -> print( (char*)str );
	return 0;

}

HardwareSerial* shellminatorArduinoSerialChannel::getSerialObject(){

	return serialPort;

}


#endif

#ifdef SHELLMINATOR_USE_ARDUINO_32U4_SERIAL

//----- Response for Arduino Serial Class -----//

void shellminatorArduino32U4SerialChannel::select( Serial_ *serialPort_p ){

	serialPort = serialPort_p;

}

int shellminatorArduino32U4SerialChannel::available(){

	if( serialPort ) return serialPort -> available();
	return 0;

}

int shellminatorArduino32U4SerialChannel::read(){

	if( serialPort ) return serialPort -> read();
	return -1;

}

int shellminatorArduino32U4SerialChannel::peek(){

	if( serialPort ) return serialPort -> peek();
	return -1;

}

void shellminatorArduino32U4SerialChannel::flush(){

	if( serialPort ) return serialPort -> flush();

}

size_t shellminatorArduino32U4SerialChannel::write( uint8_t b ){


	if( serialPort ) return serialPort -> write( b );
	return 0;

}

size_t shellminatorArduino32U4SerialChannel::print( uint8_t b ){


	if( serialPort ) return serialPort -> print( b );
	return 0;

}

size_t shellminatorArduino32U4SerialChannel::print( char c ){

	if( serialPort ) return serialPort -> print( (char)c );
	return 0;

}

size_t shellminatorArduino32U4SerialChannel::print( char *str ){

	if( serialPort ) return serialPort -> print( (char*)str );
	return 0;

}

size_t shellminatorArduino32U4SerialChannel::print( const char *str ){

	if( serialPort ) return serialPort -> print( (char*)str );
	return 0;

}

Serial_* shellminatorArduino32U4SerialChannel::getSerialObject(){

	return serialPort;

}


#endif

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

//----- Response for WiFi Client Class -----//

void shellminatorWiFiClientChannel::select( WiFiClient *client_p ){

	client = client_p;

}

int shellminatorWiFiClientChannel::available(){

	if( client ) return client -> available();
	return 0;

}

int shellminatorWiFiClientChannel::read(){

	if( client ) return client -> read();
	return -1;

}

int shellminatorWiFiClientChannel::peek(){

	if( client ) return client -> peek();
	return -1;

}

void shellminatorWiFiClientChannel::flush(){

	if( client ) return client -> flush();

}

size_t shellminatorWiFiClientChannel::write( uint8_t b ){

	if( client ) return client -> write( b );
	return 0;

}

//---- print section ----//

size_t shellminatorWiFiClientChannel::print( char c ){

	if( client ) return client -> print( c );
	return 0;

}

size_t shellminatorWiFiClientChannel::print( uint8_t b ){

	if( client ) return client -> print( b );
	return 0;

}

size_t shellminatorWiFiClientChannel::print( char *str ){

	if( client ) return client -> print( str );
	return 0;

}

size_t shellminatorWiFiClientChannel::print( const char *str ){

	if( client ) return client -> print( str );
	return 0;

}

WiFiClient* shellminatorWiFiClientChannel::getClientObject(){

	return client;

}

#endif









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

		push( data[ i ] );

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

	if( server ) return server -> sendTXT( clientID, &b, 1 );
	return 0;

}

//---- print section ----//

size_t shellminatorWebSocketChannel::print( char c ){

	if( server ) return server -> sendTXT( clientID, (uint8_t*)&c, 1 );
	return 0;

}

size_t shellminatorWebSocketChannel::print( uint8_t b ){

	char outBuff[10];
	uint32_t dataSize;

	snprintf( outBuff, 10, "%u", (int)b );

	dataSize = strlen( outBuff );

	if( server ) return server -> sendTXT( clientID, (uint8_t*)outBuff, dataSize );
	return 0;

}

size_t shellminatorWebSocketChannel::print( char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );

	if( server ) return server -> sendTXT( clientID, (uint8_t*)str, dataSize );
	return 0;

}

size_t shellminatorWebSocketChannel::print( const char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );

	if( server ) return server -> sendTXT( clientID, (uint8_t*)str, dataSize );
	return 0;

}

WebSocketsServer* shellminatorWebSocketChannel::getServerObject(){

	return server;

}

int8_t shellminatorWebSocketChannel::getClientID(){

	return clientID;

}

#endif
