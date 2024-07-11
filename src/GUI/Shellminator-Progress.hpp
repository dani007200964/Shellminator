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

#ifndef SHELLMINATOR_PROGRESS_HPP_
#define SHELLMINATOR_PROGRESS_HPP_

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "System.h"
#endif

#include "Stream.h"

#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>


/// This class can create a simple plotter object.
///
/// With this class, you can easily create a plotter object,
/// which is capable of real time plotting.
/// It can be attached to the main terminal with the beginScreen function.
class ShellminatorProgress : public ShellminatorScreen{

public:

    ShellminatorProgress();

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

    /// Set progress text.
    ///
    /// You can give some information about the current process step.
    /// @param text_p Progress text.
    void setText( const char* text_p );

    /// Format options.
    ///
    /// You can specify a custom format to add some useful informations
    /// about the progress.
    /// @param format_p Format string.
    ///
    /// Format string option characters:
    /// -`s` : Seconds since start event.
    /// -`m` : Minutes since start event.
    /// -`t` : Time since start event[ mm:ss ].
    /// -`r` : Estimated remaining time[ mm:ss ].
    /// -` ` : Blank space will be printed.
    /// -`|` : Vertical separator will be printed.
    ///
    /// For example this format `ms| r` will produce this string: `0m 3s | 01:30 left`
    void setFormat( const char* format_p );

    /// Set current percentage.
    /// @param percentage_p Percentage value[ 0.0 - 100.0 ].
    void setPercentage( float percentage_p );

    /// Set current step.
    /// @param current Current step index.
    /// @param total Number of total steps.
    void setStep( int current, int total );

    /// You can modify the color of the bar.
    void setColor( Shellminator::textColor_t color_p );

    /// Start trigger event.
    ///
    /// Start event for timings.
    /// @note The timing functions will not work properly if
    /// this function is not called. The best practice is
    /// to call this function right before the progress start.
    void start();

private:

    /// This flag shows that redraw is necessary. In this
    /// implementation redraw is only required when the
    /// screen is resized.
    bool redraw = true;

    // Color of the bars.
    Shellminator::textColor_t color = Shellminator::WHITE;
    
    // Internal variable for the percentage value.
    float percentage = 0.0;
    
    // Pointer to the text.
    const char* text = NULL;

    // Pointer to the format string.
    const char* format = NULL;

    // To track the start time.
    unsigned long timerStart = 0;

};

#endif