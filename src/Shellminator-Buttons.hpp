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

#ifndef SHELLMINATOR_BUTTONS_HPP_
#define SHELLMINATOR_BUTTONS_HPP_

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
class ShellminatorButton : public ShellminatorScreen{

public:

    /// Empty constructor.
    ShellminatorButton();

    ShellminatorButton( const char* name_p );

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
    void draw() override;

    
    void update( int width_p, int  height_p ) override;

    void attachEvent( Shellminator::shellEvent_t event_p );
    void attachTriggerFunction( void(*func_p)(void) );
    void setRoundCorners();
    void setSquareCorners();
    void setColor( uint8_t color_p );

private:
    const char* name;

    bool triggered = false;

    bool redraw = true;


    Shellminator::shellEvent_t event;
    char eventText[ 5 ];
    int eventTextSize = 0;;

    int textWidth;

    void(*func)(void) = NULL;

    /// This will be redirected to the parents channel by default.
    Stream* selectedChannel = NULL;
    bool bufferingEnabled;

    uint8_t color = Shellminator::WHITE;

    static const char topLeftCornerRound[ 4 ];
    static const char topRightCornerRound[ 4 ];
    static const char bottomLeftCornerRound[ 4 ];
    static const char bottomRightCornerRound[ 4 ];

    static const char topLeftCornerSquare[ 4 ];
    static const char topRightCornerSquare[ 4 ];
    static const char bottomLeftCornerSquare[ 4 ];
    static const char bottomRightCornerSquare[ 4 ];

    static const char horizontalLineDefault[ 4 ];
    static const char verticalLineDefault[ 4 ];

    const char* topLeftCorner;
    const char* topRightCorner;
    const char* bottomLeftCorner;
    const char* bottomRightCorner;

    const char* horizontalLine;
    const char* verticalLine;

};

#endif