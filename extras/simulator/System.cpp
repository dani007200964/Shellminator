/*
 * Created on April 08 2023
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.04.08
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

#include "System.h"

unsigned long millis(){

    double value = std::clock() / (double)(CLOCKS_PER_SEC / 1000);

    return (unsigned long) value;

}

void delay( uint32_t x ){

    #ifdef _WIN32
    Sleep( x );
    #endif

    #ifdef __EMSCRIPTEN__
    emscripten_sleep( x );
    #endif
    
}

void randomSeed( unsigned long seed ){

    if( seed != 0 ){
        srand( seed );
    }

}

long random( long howbig ){

    if( howbig == 0 ){
        return 0;
    }

    return rand() % howbig;

}

long random(long howsmall, long howbig ){

    if( howsmall >= howbig ){
        return howsmall;
    }

    long diff = howbig - howsmall;

    return random( diff ) + howsmall;

}

void systemInit(){

    #ifdef __EMSCRIPTEN__

    #endif

}