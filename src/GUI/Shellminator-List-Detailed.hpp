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


#ifndef SHELLMINATOR_LIST_DETAILED_HPP_
#define SHELLMINATOR_LIST_DETAILED_HPP_

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "System.h"
#endif

#include "Stream.h"

#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-List.hpp"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

class ShellminatorListDetailed : public ShellminatorList{

public:

    /// Empty constructor.
    ShellminatorListDetailed();

    ShellminatorListDetailed( const char* optionsList_p[], const char* detailList_p[], int listSize_p, const char* instruction_p );

    /// Init function.
    ///
    /// This function is called by the host terminal, when the plot object gets registered to it.
    /// You can put all the initialisation section here.
    /// @param parent_p Pointer to the caller terminal object.
    /// @param channel_p This is a pointer to a Stream object, which can be used to print the output
    ///                  data.
    /// @note Please use the `channel_p` object instead of `parent -> channel` method.
    ///       This object can be a regular channel __or a buffer accelerated one.__
    ///       The logic behind the buffering and flushing is handled by the caller terminal.
    void init( Shellminator* parent_p, Stream* channel_p ) override;

    void printExtra( int index ) override;

protected:

    int detailStart = 0;
    const char** detailList = NULL;

};

#endif