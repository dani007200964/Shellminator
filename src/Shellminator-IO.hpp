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

#ifndef SHELLMINATOR_IO_HPP_
#define SHELLMINATOR_IO_HPP_

#include "Shellminator-Settings.hpp"

#ifdef ARDUINO
#include "Arduino.h"
#endif

#ifdef SHELLMINATOR_USE_WIFI_CLIENT
	#ifdef ESP8266
	#include <ESP8266WiFi.h>
	#endif

	#ifdef ESP32
	#include <WiFi.h>
	#endif
#endif

/// Shellminator channel class
///
/// Shellminator uses channels to communicate with
/// external peripherials like Serial port or WiFi Client.
/// To make it easy to change the communication source,
/// virtual functions are used. This is the base class for
/// these virtual functions, and they don't do anytging
/// usefull. To make them work, every function in this class
/// has to be overrided to every peripherial, where it has
/// to be used.
class shellminatorChannel{

public:

  /// Available bytes in the channel.
  ///
  /// @returns The available bytes in the channel. Because it is the base class, it returns 0.
  virtual int    available()                               	{ return 0;  }

  /// Read one byte form the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will be removed from the channel. Because it is the base class, it returns -1.
  virtual int    read()                                    	{ return -1; }

  /// Peek the firtst byte from the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel. Because it is the base class, it returns 0.
  virtual int    peek()                                    	{ return 0;  }

  /// Flush the channel.
  virtual void   flush()                                   	{ return;    }

  /// Write one byte to the channel.
  ///
  /// @param b The value that has to be written to the channel.
  /// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
  virtual size_t write( uint8_t b )                        	{ return 0;  }

  /// Print one character to the channel.
  ///
  /// @param c The character that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  virtual size_t print( char c )                           	{ return 0;  }

  /// Print one byte to the channel.
  ///
  /// @param b The value that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  virtual size_t print( uint8_t b )                         { return 0;  }

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  virtual size_t print( char *str )                        	{ return 0;  }

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  virtual size_t print( const char *str )                  	{ return 0;  }

};

#ifdef SHELLMINATOR_USE_ARDUINO_SERIAL

/// Shellminator channel class for Arduino Serial objects.
class shellminatorArduinoSerialChannel : public shellminatorChannel{

public:

  /// Select Serial Port.
  ///
  /// Select a serial port to communicate with.
  /// @note This function has to be called before other memeber functions!
  void select( HardwareSerial *serialPort_p );

  /// Available bytes in the channel.
  ///
  /// @returns The available bytes in the channel.
  int    available() override;

  /// Read one byte form the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will be removed from the channel.
	int    read() override;

  /// Peek the firtst byte from the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel.
	int    peek() override;

  /// Flush the channel.
	void   flush() override;

  /// Write one byte to the channel.
  ///
  /// @param b The value that has to be written to the channel.
  /// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
	size_t write( uint8_t b ) override;

  /// Print one character to the channel.
  ///
  /// @param c The character that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( char c ) override;

  /// Print one byte to the channel.
  ///
  /// @param b The value that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( uint8_t b );

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( char *str ) override;

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( const char *str ) override;

  /// Get the address of the chosen Serial Port.
  ///
  /// @returns The address of the previously chosen Serial Port object.
  HardwareSerial* getSerialObject();

private:
  HardwareSerial *serialPort = NULL;

};

#endif

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

class shellminatorWiFiClientChannel : public shellminatorChannel{

public:

  /// Select WiFi Client.
  ///
  /// Select a WiFi Client to communicate with.
  /// @note This function has to be called before other memeber functions!
  void select( WiFiClient *client_p );

  /// Available bytes in the channel.
  ///
  /// @returns The available bytes in the channel.
  int    available() override;

  /// Read one byte form the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will be removed from the channel.
	int    read() override;

  /// Peek the firtst byte from the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel.
	int    peek() override;

  /// Flush the channel.
	void   flush() override;

  /// Write one byte to the channel.
  ///
  /// @param b The value that has to be written to the channel.
  /// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
	size_t write( uint8_t b ) override;

  /// Print one character to the channel.
  ///
  /// @param c The character that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( char c ) override;

  /// Print one byte to the channel.
  ///
  /// @param b The value that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( uint8_t b );

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( char *str ) override;

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
	size_t print( const char *str ) override;

  /// Get the address of the chosen WiFi Client.
  ///
  /// @returns The address of the previously chosen WiFi Client object.
  WiFiClient* getClientObject();

private:
  WiFiClient *client = NULL;

};

#endif

#endif
