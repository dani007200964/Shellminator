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

// Created for forward loading.
class Shellminator;

/// This is an abstract object for graphics element creation.
///
/// You can inherit from this object to create graphical object for the terminal. 
class ShellminatorScreen{

public:

    /// Draw function.
    ///
    /// This function is called by the host terminal, periodically. You have to put all
    /// the drawing stuff in this function.
    /// @param width_p The width of the screen area in characters.
    /// @param height_p The height of the screen area in characters.
    virtual void draw( int width, int  height ){ /* To make the linker happy... */ }

    /// Init function.
    ///
    /// This function is called by the host terminal, when the plot object gets registered to it.
    /// You can put all the initialisation section here.
    /// @param parent_p Pointer to the caller terminal object.
    virtual void init( Shellminator* parent_p ){ /* To make the linker happy... */ }

    /// Origin of the top left corner.
    ///
    /// You can specify a custom origin point for the object with this function.
    /// @param x X coordinate of the new origin( starts from 1 ).
    /// @param y Y coordinate of the new origin( starts from 1 ).
    /// @note The origin of the object is always the top left corner.
    virtual void setOrigin( int x, int y ){ /* To make the linker happy... */ }

};

#endif