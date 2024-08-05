/*
 * Created on Aug 11 2023
 *
 * Copyright (c) 2020-2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.08.11
*/

/*
MIT License

Copyright (c) 2020-2023 Daniel Hajnal

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

#ifndef SHELLMINATOR_SCREEN_HPP__
#define SHELLMINATOR_SCREEN_HPP__

#include "Shellminator.hpp"
#include "Shellminator-Helpers.hpp"
#include "Stream.h"

// Created for forward loading.
class Shellminator;

/// This is an abstract object for graphics element creation.
///
/// You can inherit from this object to create graphical object for the terminal. 
class ShellminatorScreen{

public:

    /// Draw function.
    ///
    /// This function is called by the caller terminal if redraw
    /// is requested. If you need to draw your screen, the 
    /// requestRedraw function must be called on the parent terminal
    /// object. The reason for this is to save some CPU time and do
    /// not waste it to draw something unnecessarily.
    virtual void draw( bool noClear = false ){ /* To make the linker happy... */ }

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
    virtual void init( Shellminator* parent_p, Stream* channel_p ){ /* To make the linker happy... */ }

    /// Origin of the top left corner.
    ///
    /// You can specify a custom origin point for the object with this function.
    /// @param x X coordinate of the new origin( starts from 1 ).
    /// @param y Y coordinate of the new origin( starts from 1 ).
    /// @note The origin of the object is always the top left corner.
    virtual void setOrigin( int x, int y ){
        originX = x;
        originY = y;
        if( originX < 1 ){ originX = 1; }
        if( originY < 1 ){ originY = 1; }
    }

    /// Update function.
    ///
    /// The update function must be used to process events and not to
    /// draw the objects. It is called much frequently than, the draw
    /// function. All the event handling should be done in the update
    /// function, including key and mouse detection along with various
    /// events.
    /// @param width_p The width of the screen area in characters.
    /// @param height_p The height of the screen area in characters.
    virtual void update( int width_p, int  height_p ){ /* To make the linker happy... */ }

    /// Return the coordinate of the left column next to the object.
    int left(){ return originX - 1; }

    /// Return the coordinate of the right column next to the object.
    int right(){ return originX + width; }

    /// Return the coordinate of the upper row next to the object.
    int up(){ return originY - 1; }

    /// Return the coordinate of the lower row next to the object.
    int down(){ return originY + height; }
    
    Shellminator* getParent(){ return parent; }

    /// It is used by the grid layout. It specifies
    /// the row of the widget.
    int row = 1;

    /// It is used by the grid layout. It specifies
    /// the column of the widget.
    int column = 1;

    /// It is used by the grid layout. It specifies
    /// how many columns widget occupies.
    int columnSpan = 1;

    /// It is used by the grid layout. It specifies
    /// how many rows widget occupies.
    int rowSpan = 1;

    /// It is used by the grid layout. The purpose of this is
    /// to create a 'chained' list of the used elements in the
    // grid layout.
    ShellminatorScreen* nextElement = NULL;

protected:

    /// Pointer to the caller terminal object.
    Shellminator* parent = NULL;

    Stream* channel = NULL;

    /// Actual width of the object.
    int width = 1;

    /// Actual height of the object.
    int height = 1;

    /// X coordinate of the origin.
    int originX = 1;

    /// Y coordinate of the origin.
    int originY = 1;

};

#endif