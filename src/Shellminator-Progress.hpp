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

class ShellminatorProgress{

public:

    ShellminatorProgress( Shellminator* shell_p );
    ShellminatorProgress( Stream* channel_p );

    void drawProgressBar( float percentage, const char* text, const char* done_p, const char* todo_p, const char* middle_p );
    void drawProgressBar( float percentage, const char* text, const char* done_p, const char* todo_p );
    void drawProgressBar( float percentage, const char* text );

    uint8_t doneColor = 0;
    uint8_t todoColor = 0;
    uint8_t middleColor = 0;

    uint8_t percentColor = 0;
    uint8_t percentStyle = 0;

    uint8_t textColor = 0;
    uint8_t textStyle = 0;

private:

    void drawProgressBarStream( Stream* channel_p, float percentage, const char* text, const char* done, const char* todo, const char* middle );
    void drawProgressBarShell( float percentage, const char* text, const char* done, const char* todo, const char* middle );

    Shellminator* shell = NULL;
    Stream* channel = NULL;

};

#endif