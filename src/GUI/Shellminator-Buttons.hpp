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

#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-Helpers.hpp"

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

    /// Constructor with name.
    ///
    /// This constructor must be used to use the object properly.
    /// @param name_p This is a string, which will be displayed
    ///               on the center of the button.
    ShellminatorButton( const char* name_p );

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

    /// Attach optional event.
    ///
    /// Optionally, you can attach a keyboard event to the button.
    /// It could be handy when the host terminal emulator does not
    /// support keyboard reporting( sadly, Windows currently does
    /// this in the emulator... ).
    /// @param event_p A configured structure, that contains the event data.
    ///
    /// Example code:
    /// @code{cpp}
    /// // Create a structure to store the configuration.
    /// Shellminator::shellEvent_t buttonEvent;
    ///
    /// // Configure the event to make it trigger for the 's' key.
    /// buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    /// buttonEvent.data = (uint8_t)'s';
    /// @endcode
    void attachEvent( Shellminator::shellEvent_t event_p );

    /// Attach a callback.
    ///
    /// You can attach a callback function to the button.
    /// When a mouse click event is detected on the button,
    /// or the attached event triggers, this function will
    /// be called.
    /// @param func_p Pointer to a function, which will be called.
    ///
    /// Example callback function prototype: `void buttonEvent( ShellminatorScreen* parent );`
    /// A pointer to the caller button is passed to the callback. This way the callback
    /// function can access the caller button.
    void attachTriggerFunction( void(*func_p)(ShellminatorScreen*) );

    /// You can modify the color of the frame.
    void setColor( Shellminator::textColor_t color_p );

    void forceRedraw() override;

private:

    /// This variable stores a pointer to the name text.
    const char* name;

    /// @brief  This variable stores the name text width.
    int textWidth;

    /// This flag shows that redraw is necessary. In this
    /// implementation redraw is only required when the
    /// screen is resized.
    bool redraw = true;

    /// This variable stores the custom trigger event data.
    Shellminator::shellEvent_t event;

    /// This is a buffer to store the event key text.
    char eventText[ 6 ];

    /// This variable stores the event key text size.
    int eventTextSize = 0;

    /// Pointer to the event callback function.
    /// It it is null, the callback is disabled.
    void(*func)(ShellminatorScreen*) = NULL;

    /// Button frame color. Default is White.
    Shellminator::textColor_t color = Shellminator::WHITE;

    /// Check if the character is uppercase.
    /// @param c Character to be checked.
    /// @returns Returns true if the input character is uppercase.
    bool isUpper( char c );

    /// Check if the character is lowercase.
    /// @param c Character to be checked.
    /// @returns Returns true if the input character is lowercase.
    bool isLower( char c );

    /// Make the character uppercase.
    /// @param c Character to convert to uppercase.
    /// @returns Returns the uppercase  character if the input character was lowercase.
    ///          Otherwise returns the input character.
    char toUpper( char c );
    
    // For unit testing
    friend class ShellminatorButtonUT;
};

#endif