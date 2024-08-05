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

#include "Commander-Caller-Interface.hpp"
#include "Shellminator.hpp"
#include "Shellminator-Helpers.hpp"

class ShellminatorCaller : public CommandCaller{

public:
    ShellminatorCaller();
    void beep() override;
    void setBannerText( const char* text_p ) override;
    void setPathText( const char* text_p ) override;
    void clear() override;
    void setShell( Shellminator* shell_p );

private:
    Shellminator* shell = NULL;
};

class ShellminatorCommanderInterface : public Shellminator{

public:
    ShellminatorCommanderInterface( Stream *stream_p ) : Shellminator{ stream_p }{ caller.setShell( this ); }

    void attachCommander( Commander* commander_p );
    void printCommandParserHelp( Stream* channel_p, bool formatting_p ) override;

    /// Check the command with the command parser to get some useful information.
    void checkCommandFraction() override;

    bool hasCommandParser() override;

    void executeWithCommandParser() override;

    void autoCompleteWithCommandParser() override;

    void attachAutoCompleteBuffer( char* buffer_p, int buffer_size_p );

private:

    char* autoCompleteBuffer = NULL;
    int autoCompleteBufferSize = 0;

    /// Pointer to a Commander object.
    Commander* commander = NULL;

    ShellminatorCaller caller;

};

class CommanderColorizer : public DefaultColorizer{

public:
    CommanderColorizer();
    void printChar( Stream* response, char c ) override;
    void reset( Stream* response ) override;

protected:

    typedef enum{
        DEFAULT_STATE = 0,
        FIRST_DASH_STATE,
        SECOND_DASH_STATE,
        WAIT_FOR_WHITESPACE,
        WAIT_TOKEN_END,
        WAIT_STRING_END,
        ENV_VAR_START
    }colorizerState_t;

    colorizerState_t stateBuffer[ SHELLMINATOR_BUFF_LEN + 1 ];
    int bufferCntr = 0;

    colorizerState_t currentState = DEFAULT_STATE;

    void defaultStateFuncion( Stream* response, char c );
    void firstDashStateFuncion( Stream* response, char c );
    void secondDashStateFuncion( Stream* response, char c );
    void waitForWhitespaceStateFuncion( Stream* response, char c );
    void waitTokenEndStateFunction( Stream* response, char c );
    void waitStringEndStateFunction( Stream* response, char c );
    void envVarStartStateFunction( Stream* response, char c );

    void printBackwardError( Stream* response );

};

#endif

#endif