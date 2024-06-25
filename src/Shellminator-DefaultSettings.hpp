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

#ifndef SHELLMINATOR_DEFAULT_SETTINGS_HPP_
#define SHELLMINATOR_DEFAULT_SETTINGS_HPP_

#ifdef __has_include
  #if __has_include ("Shellminator-Settings.hpp")
    #include "Shellminator-DefaultSettings.hpp"
  #endif
#endif

#ifdef ESP32

  #ifndef SHELLMINATOR_USE_WIFI_CLIENT
    #define SHELLMINATOR_USE_WIFI_CLIENT
  #endif

  #ifndef SHELLMINATOR_ENABLE_QR_SUPPORT
    #define SHELLMINATOR_ENABLE_QR_SUPPORT
  #endif

  #ifndef SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
    #define SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
  #endif

  /* NOT WORKING YET!
  #ifndef SHELLMINATOR_ENABLE_PASSWORD_MODULE
  #define SHELLMINATOR_ENABLE_PASSWORD_MODULE
  #endif
  */
  
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

  #ifndef SHELLMINATOR_ENABLE_QR_SUPPORT
    #define SHELLMINATOR_ENABLE_QR_SUPPORT
  #endif

  #ifndef SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
    #define SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT
  #endif

  /* NOT WORKING YET!
  #ifndef SHELLMINATOR_ENABLE_PASSWORD_MODULE
  #define SHELLMINATOR_ENABLE_PASSWORD_MODULE
  #endif
  */

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

/// Definition of the maximum length of each command
#ifndef SHELLMINATOR_BUFF_LEN
  #define SHELLMINATOR_BUFF_LEN 30
#endif

/// Definition of the maximum length of the previous command memory
/// @warning Be careful with the The value of this definition. If it is to high your RAM will be eaten!
/// @note The total amount of RAM consumed by the object in bytes can be calculated as: \link SHELLMINATOR_BUFF_LEN \endlink * \link SHELLMINATOR_BUFF_DIM \endlink
#ifndef SHELLMINATOR_BUFF_DIM
  #define SHELLMINATOR_BUFF_DIM 5
#endif

/// -- Maximum length of the banner text --
#ifndef SHELLMINATOR_BANNER_LEN
  #define SHELLMINATOR_BANNER_LEN 10
#endif

/// -- Maximum length of the banner path text --
#ifndef SHELLMINATOR_BANNER_PATH_LEN
  #define SHELLMINATOR_BANNER_PATH_LEN 10
#endif

// -- Progress bar stuff --
#ifndef SHELLMINATOR_PROGRESS_BAR_SIZE
  #define SHELLMINATOR_PROGRESS_BAR_SIZE 40
#endif

#ifndef SHELLMINATOR_PROGRESS_BAR_TEXT_SIZE
  #define SHELLMINATOR_PROGRESS_BAR_TEXT_SIZE 40
#endif

#ifndef SHELLMINATOR_PROGRESS_BAR_CHARACTER_SIZE
  #define SHELLMINATOR_PROGRESS_BAR_CHARACTER_SIZE 10
#endif


/// Color of the startup logo
/// @note This macro has to be defined befor importing the Shellminator.hpp. If not then the default value will be RED.
#ifndef SHELLMINATOR_LOGO_FONT_STYLE
  #define SHELLMINATOR_LOGO_FONT_STYLE BOLD
#endif

/// Style of the startup logo
/// @note This macro has to be defined befor importing the Shellminator.hpp. If not then the default value will be BOLD.
#ifndef SHELLMINATOR_LOGO_COLOR
  #define SHELLMINATOR_LOGO_COLOR RED
#endif

/// If defined, enables the WebSocket Module.
/// @note This macro has to be defined befor importing the Shellminator.hpp. If not then the default value will be BOLD.
#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
  #ifndef SHELLMINATOR_WEBSOCKET_BUFFER_LEN
    #define SHELLMINATOR_WEBSOCKET_BUFFER_LEN 50
  #endif
#endif

// -- Plot module bar stuff --

#ifndef SHELLMINATOR_PLOT_NAME_SIZE
  #define SHELLMINATOR_PLOT_NAME_SIZE 20
#endif

#ifndef SHELLMINATOR_NUMBER_OF_PLOTS
  #define SHELLMINATOR_NUMBER_OF_PLOTS 3
#endif

#ifndef SHELLMINATOR_PLOT_BUFF_SIZE
  #define SHELLMINATOR_PLOT_BUFF_SIZE 300
#endif

#ifndef EVENT_BUFFER_SIZE
  #define EVENT_BUFFER_SIZE 5
#endif

#endif
