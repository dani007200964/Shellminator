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

#ifndef SHELLMINATOR_PLOT_MODULE_HPP_
#define SHELLMINATOR_PLOT_MODULE_HPP_

#include "Shellminator-DefaultSettings.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator.hpp"
#include "Shellminator-BufferedPrinter.hpp"

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

class ShellminatorPlot{

public:

  ShellminatorPlot( Shellminator* shell_p, float* y, int y_size, int color = Shellminator::GREEN );

  bool begin();

  bool addPlot( float* y, int y_size, int color = 0 );

  void draw( bool redraw = false );


private:

  uint8_t colorTable[ 6 ]{
    Shellminator::GREEN,
    Shellminator::RED,
    Shellminator::BLUE,
    Shellminator::YELLOW,
    Shellminator::MAGENTA,
    Shellminator::CYAN    
  };

  ShellminatorBufferedPrinter bufferedPrinter;

  void drawScale();
  void drawPlot( uint8_t index );

  float lerp( float v0, float v1, float t );
  float mapFloat( float x, float inStart, float inStop, float outStart, float outStop );

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  ShellminatorBufferedPrinter bufferedPrinter;

  #endif


  int terminalSizeX;
  int terminalSizeY;

  float min;
  float max;

  int yTextSize;
  int resultTextSize;

  Shellminator* shell = NULL;

  float* yDataF[ SHELLMINATOR_NUMBER_OF_PLOTS ];
  int* yDataI[ SHELLMINATOR_NUMBER_OF_PLOTS ];

  int plotColor[ SHELLMINATOR_NUMBER_OF_PLOTS ];

  int yDataSize;

  uint8_t numberOfPlots = 0;

  char plotName[ SHELLMINATOR_PLOT_NAME_SIZE ] = "Plot";

};

#endif

