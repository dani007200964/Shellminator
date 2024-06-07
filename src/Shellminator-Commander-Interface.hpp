/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2024.06.10
 */

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

#ifndef __SHELLMINATOR_COMMANDER_INTERFACE_HPP__
#define __SHELLMINATOR_COMMANDER_INTERFACE_HPP__

// 'Smart' macro to include the supported
// external libraries.
#ifdef __has_include
    // Check for Commander-API library
    #if __has_include ( "Commander-API.hpp" )
        #include "Commander-API.hpp"
    #endif
#endif

//---- Commander-API support specific part ----//
#ifdef COMMANDER_API_VERSION

#include "Shellminator.hpp"

class ShellminatorCommanderInterface : public Shellminator{

public:
    ShellminatorCommanderInterface( Stream *stream_p ) : Shellminator{ stream_p }{}

    void attachCommander( Commander* commander_p );
    void printCommandParserHelp( Stream* channel_p, bool formatting_p ) override;

    /// Check the command with the command parser to get some useful information.
    void checkCommandFraction() override;

    bool hasCommandParser() override;

    void executeWithCommandParser() override;

    void autoCompleteWithCommandParser() override;

private:
    /// Pointer to a Commander object.
    Commander* commander = NULL;

};

#endif

#endif