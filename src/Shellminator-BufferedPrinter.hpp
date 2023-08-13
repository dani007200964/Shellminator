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

#ifndef SHELLMINATOR_BUFFERED_PRINTER_HPP_
#define SHELLMINATOR_BUFFERED_PRINTER_HPP_

#include "Shellminator-DefaultSettings.hpp"
#include "Shellminator-IO.hpp"

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "System.h"
#endif

#include "Stream.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/// ShellminatorBufferedPrinter object
///
/// It can be used to accelerate the rendering process on the host terminal interface.
/// Terminal emulators usually works faster, when the data arrives in one large piece.
/// Otherwise some annoying effect can be observed like flickering or cursor jumping.
/// This object aims to fix this problem. It is a simple buffered printer.
class ShellminatorBufferedPrinter : public Stream{

public:

  /// Empty constructor.
  ///
  /// This is a simple, empty constructor.
  ShellminatorBufferedPrinter();

  /// Stream specific constructor.
  ///
  /// With this constructor, you can specify the Stream object.
  /// @param channel_p Pointer to a Stream object. This object will be used as output.
  ShellminatorBufferedPrinter( Stream* channel_p, char* buffer_p, int bufferSize_p );

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

  size_t write( const char *str ) override;

private:

  /// Clear the internal buffer.
  ///
  /// This value sets all of the elements in the internal buffer to '\0'.
  /// @note It only works, if the memory allocation is done.
  void clearBuffer();

  // Pointer to the output Stream object.
  Stream* channel = NULL;

  char* buffer = NULL;
  int bufferPointer = 0;

  int bufferSize = 0;

};

#endif