/*
 * Created on Nov 11 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified Nov 11 2023
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

#ifndef __SHELLMINATOR_COLORIZER_HPP__
#define __SHELLMINATOR_COLORIZER_HPP__

#include <stdint.h>
#include "Stream.h"
#include "Shellminator-DefaultSettings.hpp"

class DefaultColorizer{

public:
    DefaultColorizer();
    virtual void printChar( Stream* response, char c ){ if( response ){ response -> print( c ); } }
    virtual void reset( Stream* response ) {}

protected:

    bool isCharacter( char c );
    bool isNumber( char c );

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