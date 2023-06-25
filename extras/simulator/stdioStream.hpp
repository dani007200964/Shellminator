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

#ifndef STDIO_STREAM_HPP__
#define STDIO_STREAM_HPP__

#include "Stream.h"

#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdint.h>

#include <fcntl.h>

#define STDIO_STREAM_BUFFER_SIZE 1024

class stdioStream : public Stream{

private:

  void push( uint8_t data );
  void push( uint8_t* data, size_t size );

  char buffer[ STDIO_STREAM_BUFFER_SIZE ];
	uint32_t readPointer = 0;
	uint32_t writePointer = 0;

public:

  void update();

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

  size_t write( const uint8_t *buff, size_t size ) override;

  size_t write( const char *str ) override;

private:
  wchar_t vOut[ 10000 ];



};


#endif