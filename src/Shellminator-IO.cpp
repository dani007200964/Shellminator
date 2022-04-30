/*
 * Created on June 18 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Commander-API project.
 * Modified 2022.02.06
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

size_t shellminatorArduinoSerialChannel::print( char c ){

	if( serialPort ) return serialPort -> print( c );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( char *str ){

	if( serialPort ) return serialPort -> print( str );
	return 0;

}

size_t shellminatorArduinoSerialChannel::print( const char *str ){

	if( serialPort ) return serialPort -> print( str );
	return 0;

}

#endif
