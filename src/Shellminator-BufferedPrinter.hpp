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

#include "Shellminator-Settings.hpp"
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
#include <cstdlib>
#include <malloc.h>

class ShellminatorBufferedPrinter{

public:

  ShellminatorBufferedPrinter();
  ShellminatorBufferedPrinter( Stream* channel_p, int bufferSize_p );
  
  // I don't know why, but it crashes on windows
  // ~ShellminatorBufferedPrinter();

  void setChannel( Stream* channel_p );
  int printf( const char *fmt, ... );
  void flush();

  int getBufferSize();

private:

  void clearBuffer();

  int bufferSize = -1;

  char *acceleratorBuffer = NULL;
  char *acceleratorBufferPointer;
  uint32_t availableCharacters;

  Stream* channel = NULL;

};

#endif