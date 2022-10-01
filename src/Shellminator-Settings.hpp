/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
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

#ifndef SHELLMINATOR_SETTINGS_HPP_
#define SHELLMINATOR_SETTINGS_HPP_

#ifdef ARDUINO

  #ifndef SHELLMINATOR_USE_ARDUINO_SERIAL

    #ifndef SHELLMINATOR_USE_ARDUINO_SERIAL
    #define SHELLMINATOR_USE_ARDUINO_SERIAL
    #endif

  #endif

  #if defined(ARDUINO_AVR_LEONARDO)

    #ifndef SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
    #define SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
    #endif

  #endif

  #if defined(ARDUINO_AVR_MICRO)

    #ifndef SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
    #define SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
    #endif

  #endif

#endif

#ifdef ESP32

  #ifndef SHELLMINATOR_USE_WIFI_CLIENT
  #define SHELLMINATOR_USE_WIFI_CLIENT
  #endif

  #ifndef SHELLMINATOR_ENABLE_SEARCH_MODULE
  #define SHELLMINATOR_ENABLE_SEARCH_MODULE
  #endif

  #ifndef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  #define SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  #endif

  #ifndef SHELLMINATOR_ENABLE_QR_SUPPORT
  #define SHELLMINATOR_ENABLE_QR_SUPPORT
  #endif

  #ifndef SHELLMINATOR_BUFF_LEN
  #define SHELLMINATOR_BUFF_LEN 50
  #endif

  #ifndef SHELLMINATOR_BUFF_DIM
  #define SHELLMINATOR_BUFF_DIM 20
  #endif

  #ifndef SHELLMINATOR_BANNER_LEN
  #define SHELLMINATOR_BANNER_LEN 20
  #endif

  #ifndef SHELLMINATOR_BANNER_PATH_LEN
  #define SHELLMINATOR_BANNER_PATH_LEN 20
  #endif

  #ifdef __has_include
    #if __has_include (<WebSocketsServer.h>)
  		#ifndef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
      	#define SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
  		#endif
    #endif
  #endif

#endif

#ifdef ESP8266

  #ifndef SHELLMINATOR_USE_WIFI_CLIENT
  #define SHELLMINATOR_USE_WIFI_CLIENT
  #endif

  #ifndef SHELLMINATOR_ENABLE_SEARCH_MODULE
  #define SHELLMINATOR_ENABLE_SEARCH_MODULE
  #endif

  #ifndef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  #define SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  #endif

  #ifndef SHELLMINATOR_ENABLE_QR_SUPPORT
  #define SHELLMINATOR_ENABLE_QR_SUPPORT
  #endif

  #ifndef SHELLMINATOR_BUFF_LEN
  #define SHELLMINATOR_BUFF_LEN 50
  #endif

  #ifndef SHELLMINATOR_BUFF_DIM
  #define SHELLMINATOR_BUFF_DIM 20
  #endif

  #ifndef SHELLMINATOR_BANNER_LEN
  #define SHELLMINATOR_BANNER_LEN 20
  #endif

  #ifndef SHELLMINATOR_BANNER_PATH_LEN
  #define SHELLMINATOR_BANNER_PATH_LEN 20
  #endif

  #ifdef __has_include
    #if __has_include (<WebSocketsServer.h>)
  		#ifndef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
      	#define SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
  		#endif
    #endif
  #endif

#endif

#ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
#define SHELLMINATOR_ACCELERATOR_BUFFER_LEN SHELLMINATOR_BANNER_LEN + SHELLMINATOR_BANNER_PATH_LEN + SHELLMINATOR_BUFF_LEN * 2 + 30
#endif

/// Definition of the maximum length of each command
#ifndef SHELLMINATOR_BUFF_LEN
#define SHELLMINATOR_BUFF_LEN 20
#endif

/// Definition of the maximum length of the previous command memory
/// @warning Be careful with the The value of this definition. If it is to high your RAM will be eaten!
/// @note The total amount of RAM consumed by the object in bytes can be calculated as: \link SHELLMINATOR_BUFF_LEN \endlink * \link SHELLMINATOR_BUFF_DIM \endlink
#ifndef SHELLMINATOR_BUFF_DIM
#define SHELLMINATOR_BUFF_DIM 5
#endif

/// Maximum length of the banner text
#ifndef SHELLMINATOR_BANNER_LEN
#define SHELLMINATOR_BANNER_LEN 10
#endif

#ifndef SHELLMINATOR_BANNER_PATH_LEN
#define SHELLMINATOR_BANNER_PATH_LEN 10
#endif

/// Color and style of the startup logo
/// @note This macro has to be defined befor importing the Shellminator.hpp. If not then the default value will be BOLD and RED.
#ifndef SHELLMINATOR_LOGO_FONT_STYLE
#define SHELLMINATOR_LOGO_FONT_STYLE BOLD
#endif

#ifndef SHELLMINATOR_LOGO_COLOR
#define SHELLMINATOR_LOGO_COLOR RED
#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
#define SHELLMINATOR_WEBSOCKET_BUFFER_LEN 50
#endif

#endif
