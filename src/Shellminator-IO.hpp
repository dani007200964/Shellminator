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

class shellminatorChannel{

public:
  virtual int    available()                               	{ return 0;  }
  virtual int    read()                                    	{ return -1; }
  virtual int    peek()                                    	{ return 0;  }
  virtual void   flush()                                   	{ return;    }
  virtual size_t write( uint8_t b )                        	{ return 0;  }
  virtual size_t print( char c )                           	{ return 0;  }
  virtual size_t print( uint8_t b )                         { return 0;  }
  virtual size_t print( char *str )                        	{ return 0;  }
  virtual size_t print( const char *str )                  	{ return 0;  }

};

#ifdef SHELLMINATOR_USE_ARDUINO_SERIAL

class shellminatorArduinoSerialChannel : public shellminatorChannel{

public:
  void select( HardwareSerial *serialPort_p );

  int    available() override;
	int    read() override;
	int    peek() override;
	void   flush() override;
	size_t write( uint8_t b ) override;
	size_t print( char c ) override;
  size_t print( uint8_t b );
	size_t print( char *str ) override;
	size_t print( const char *str ) override;

  HardwareSerial* getSerialObject();

private:
  HardwareSerial *serialPort = NULL;

};

#endif

#endif
