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

#include "Shellminator-Commander-Interface.hpp"

//---- Commander-API support specific part ----//
#ifdef COMMANDER_API_VERSION

void ShellminatorCommanderInterface::printCommandParserHelp( Stream* channel_p, bool formatting_p ){
    if( commander != NULL ){
        commander -> printHelp( channel_p, formatting_p );
    }
}

void ShellminatorCommanderInterface::attachCommander( Commander* commander_p ){
    commander = commander_p;
}

void ShellminatorCommanderInterface::checkCommandFraction(){
    // By default it is empty, because no command parser is available.
}

void ShellminatorCommanderInterface::executeWithCommandParser(){
    // By default it is empty, because no command parser is available.
}

void ShellminatorCommanderInterface::autoCompleteWithCommandParser(){
    // By default it is empty, because no command parser is available.    
}

bool ShellminatorCommanderInterface::hasCommandParser(){
    // By default it returns false, because no command parser is available.
    if( commander != NULL ){
        return true;
    }

    return false;
}

#endif