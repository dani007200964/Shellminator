/*
 * Created on July 07 2024
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified July 07 2024
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

#ifndef SHELLMINATOR_HELPERS_HPP_
#define SHELLMINATOR_HELPERS_HPP_

#include "Shellminator-DefaultSettings.hpp"
#include <string.h>

//---- Network Related Defines ----
#ifdef SHELLMINATOR_USE_WIFI_CLIENT
    #ifdef ESP8266
        #include <ESP8266WiFi.h>
        #define CLIENT_STATE ( client.status() == ESTABLISHED )
    #endif

    #ifdef ESP32
        #include <WiFi.h>
        #define CLIENT_STATE client.connected()
    #endif

    #ifdef ARDUINO_UNOWIFIR4
        #include <WiFi.h>
        #define CLIENT_STATE client.connected()
    #endif

    #ifdef ARDUINO_RASPBERRY_PI_PICO_W
        #include <WiFi.h>
        #define CLIENT_STATE client.connected()
    #endif
#endif

//---- Text Related Defines ----

// Clever solution to handle constant string data.
// Thank you ondras12345!
#ifndef __CONST_TXT__
    #if defined(ARDUINO) && defined(__AVR__)
        #define v(s) F(s)
    #else
        #define __CONST_TXT__(s) (const char*)(s)
    #endif
#endif

int indexOf( const char* data, const char x );
bool startsWith( const char* original, const char* key );
void shiftStringLeft( char* str );
int strcicmp( const char* p1, const char* p2 );
void tailEnd( char* str );

#endif