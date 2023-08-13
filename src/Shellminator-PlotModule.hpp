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

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "System.h"
#endif

#include "Stream.h"

#include "Shellminator-DefaultSettings.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-BufferedPrinter.hpp"


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/// This class can create a simple plotter object.
///
/// With this class, you can easily create a plotter object,
/// which is capable of real time plotting.
/// It can be attached to the main terminal with the beginScreen function.
class ShellminatorPlot : public ShellminatorScreen{

public:

    /// Empty constructor.
    ShellminatorPlot();

    /// Constructor for user calculated float array.
    ///
    /// With this constructor you can create a plotter object which is using a
    /// user created dataset.
    /// @param data_p Pointer to a float array, which contains the data points to be printed.
    /// @param dataSize_p The size of the float array in elements( NOT IN BYTES! ).
    /// @param color_p Optionally, you can specify the color of the plot. Available colors:
    /// @param name_p The name of the plot.
    ///                *Shellminator::RED
    ///                *Shellminator::GREEN
    ///                *Shellminator::WHITE
    ShellminatorPlot( float* data_p, int dataSize_p, const char* name_p, int color_p = Shellminator::GREEN );

    /// Init function.
    ///
    /// This function is called by the host terminal, when the plot object gets registered to it.
    /// @param parent_p Pointer to the caller terminal object.
    void init( Shellminator* parent_p ) override;

    /// Draw function.
    ///
    /// This function is called by the host terminal, periodically.
    /// @param width_p The width of the screen area in characters.
    /// @param height_p The height of the screen area in characters.
    /// @todo Buffering!
    void draw( int width_p, int  height_p ) override;

    /// Origin of the top left corner.
    ///
    /// You can specify a custom origin point for the object with this function.
    /// @param x X coordinate of the new origin( starts from 1 ).
    /// @param y Y coordinate of the new origin( starts from 1 ).
    /// @note The origin of the object is always the top left corner.
    void setOrigin( int x, int y ) override;

private:

    /// Pointer to the data array.
    float* data = NULL;

    /// Size of the data array in elements.
    int dataSize = 0;

    /// Plot name.
    const char* name = NULL;

    /// Color code for the plot.
    int color = Shellminator::GREEN;

    /// Draw the scale for the plot.
    ///
    /// The scale is on the left side with this implementation.
    /// It can be overriden to create different kinds of plots.
    virtual void drawScale();

    /// Draw the plot data.
    ///
    /// This function renders the data points from the data array.
    /// It can be overriden to create different kinds of plots.
    virtual void drawPlot();

    /// Pointer to the caller terminal.
    Shellminator* parent = NULL;

    // This will be redirected to the parents channel by default.
    Stream* selectedChannel = NULL;

    /// Terminal width in characters.
    int width;

    /// Terminal height in characters.
    int height;

    /// This buffer holds a generated string that
    /// us used to print numbers to the scale.
    char valueTextBuffer[ 15 ];

    /// This variable holds the size of the largest
    /// element generated in the valueTextBuffer.
    int valueTextSizeMax = 0;

    /// This variable holds the size of the last
    /// elements text.
    int resultTextSize = 0;

    /// Holds the minimum value in the data array.
    float min;

    /// Holds the maximum value in the data array.
    float max;

    /// X coordinate of the origin.
    int originX = 1;

    /// Y coordinate of the origin.
    int originY = 1;

    /// Linear interpolation.
    ///
    /// Original [source](https://en.wikipedia.org/wiki/Linear_interpolation)
    /// @param v0 First corner value.
    /// @param v1 Second corner value.
    /// @param t Control value between 0.0 and 1.0.
    float lerp( float v0, float v1, float t );

    /// Floating point implementation of the map function.
    float mapFloat( float x, float inStart, float inStop, float outStart, float outStop );

};

#endif

