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
class ShellminatorBufferedPrinter{

public:

  /// Empty constructor.
  ///
  /// This is a simple, empty constructor.
  ShellminatorBufferedPrinter();

  /// Stream specific constructor.
  ///
  /// With this constructor, you can specify the Stream object.
  /// @param channel_p Pointer to a Stream object. This object will be used as output.
  ShellminatorBufferedPrinter( Stream* channel_p );

  /// Destructor.
  ///
  /// If any memory is allocated for the internal buffer, the
  /// destructor will deallocate this memory.
  ~ShellminatorBufferedPrinter();

  /// Allocate memory for the internal buffer.
  ///
  /// With this function, you can allocate memory for the internal buffer.
  /// @param bufferSize Allocated memory in characters. It must be at least 30 characters!
  /// @returns If the memory allocation is done it will return true. If it fails for some reason, it will return false.
  /// @warning To use the object, this function must be called in the init section.
  /// @note Usually 30-50 characters long buffer is enough for devices with small amount of memory.
  bool allocate( int bufferSize );

  /// Deallocate memory.
  ///
  /// If any memory is allocated for the internal buffer, the
  /// destructor will deallocate this memory.
  void deallocate();

  /// Set the output Stream.
  ///
  /// This function can be used to specify the output Stream object.
  /// @param channel_p Pointer to a Stream object. This object will be used as output.
  void setChannel( Stream* channel_p );

  /// Get the output Stream.
  ///
  /// This function can be used to get the address of the output Stream object.
  /// @returns Pointer to the used Stream object.
  Stream* getChannel();

  /// Regular printf like function.
  ///
  /// To put data to the internal buffer, you can use this function.
  /// It is a regular printf like function.
  /// @param fmt Format specifier.
  /// @param ... Variadic list.
  /// @returns The number of printed characters.
  /// @note If you finished printing, you need to use the flush function to put the data out to the specified Stream channel.
  int printf( const char *fmt, ... );

  /// Flush function.
  ///
  /// If you finished printing, you have to flush out the content in the internal buffer.
  /// This function flushes out all of the accumulated data in the internal buffer.
  void flush();

  /// Get the allocated memory.
  ///
  /// @returns This function returns the size of the allocated memory.
  /// @warning If it returns less than zero, that means the memory allocation failed or not done yet.
  int getBufferSize();

private:

  /// Clear the internal buffer.
  ///
  /// This value sets all of the elements in the internal buffer to '\0'.
  /// @note It only works, if the memory allocation is done.
  void clearBuffer();

  // It tracks how much memory is allocated for characters.
  int bufferSize = -1;

  // Pointer to the buffer. Malloc will use this address.
  char *acceleratorBuffer = NULL;
  
  // It points to the next free element's address in the buffer.
  char *acceleratorBufferPointer;

  // It indicates how much free characters are available in the buffer.
  uint32_t availableCharacters;

  // Pointer to the output Stream object.
  Stream* channel = NULL;

};

#endif