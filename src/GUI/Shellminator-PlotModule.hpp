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

//#include "Shellminator-DefaultSettings.hpp"
//#include "Shellminator-IO.hpp"
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
    /// You can put all the initialisation section here.
    /// @param parent_p Pointer to the caller terminal object.
    /// @param channel_p This is a pointer to a Stream object, which can be used to print the output
    ///                  data.
    /// @note Please use the `channel_p` object instead of `parent -> channel` method.
    ///       This object can be a regular channel __or a buffer accelerated one.__
    ///       The logic behind the buffering and flushing is handled by the caller terminal.
    void init( Shellminator* parent_p, Stream* channel_p ) override;

    /// Draw function.
    ///
    /// This function is called by the caller terminal if redraw
    /// is requested. If you need to draw your screen, the 
    /// requestRedraw function must be called on the parent terminal
    /// object. The reason for this is to save some CPU time and do
    /// not waste it to draw something unnecessarily.
    void draw( bool noClear = false ) override;

    /// Update function.
    ///
    /// The update function must be used to process events and not to
    /// draw the objects. It is called much frequently than, the draw
    /// function. All the event handling should be done in the update
    /// function, including key and mouse detection along with various
    /// events.
    /// @param width_p The width of the screen area in characters.
    /// @param height_p The height of the screen area in characters.
    void update( int width_p, int  height_p ) override;

    /// Set the color of the plot.
    ///
    /// With this function, you can set the color of the plot.
    /// @param color_p Specify a color from Shellminator::textColor_t enumeration.
    void setColor( Shellminator::textColor_t color_p );


private:

    /// Pointer to the data array.
    float* data = NULL;

    /// Size of the data array in elements.
    int dataSize = 0;

    bool redraw = true;

    unsigned long plotTimerStart;
    int plotRedrawPeriod;

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

    static const char clearCell[2];
    static const char dotCell[5];

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

