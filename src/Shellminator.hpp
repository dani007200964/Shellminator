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

#ifndef SHELLMINATOR_HPP_
#define SHELLMINATOR_HPP_

#include "Shellminator-DefaultSettings.hpp"
#include "Shellminator-IO.hpp"

#ifdef ARDUINO
#include "Arduino.h"
#else
#include "System.h"
#endif

#ifdef __AVR__

#include <avr/pgmspace.h>

#endif

#include "Stream.h"

#ifdef SHELLMINATOR_USE_WIFI_CLIENT
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
#include <WebSocketsServer.h>
#endif

#endif

#ifdef SHELLMINATOR_ENABLE_PASSWORD_MODULE

#include "external/sha256/terminal_sha256.h"

#endif

#include "Shellminator-BufferedPrinter.hpp"
#include "Shellminator-Screen.hpp"

#ifdef __has_include
#if __has_include("Commander-API.hpp")
#include "Commander-API.hpp"
#endif
#endif

#ifdef COMMANDER_API_VERSION
#include "Commander-API.hpp"
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

///  +------  Custom configuration  ------+
///  |                                    |
///  |  This is where you have to config  |
///  |           your defines!            |
///  |                                    |
///  +------------------------------------+

#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

#include "external/nayuki-qrcodegen/qrcodegen.h"

#endif

// Clever solution to handle constant string data.
// Thank you ondras12345!
#ifndef __CONST_TXT__
    #if defined(ARDUINO) && defined(__AVR__)
        #define __CONST_TXT__(s) F(s)
    #else
        #define __CONST_TXT__(s) (const char*)(s)
    #endif
#endif

#define SHELLMINATOR_HELP_TEXT   "\r\n"                                                     \
    "\033[1;31m----\033[1;32m Shortcut Keys \033[1;31m----\033[0;37m\r\n"                   \
    "\r\n"                                                                                  \
    "\033[1;31mCtrl-A\033[1;32m : Jumps the cursor to the beginning of the line.\r\n"       \
    "\033[1;31mCtrl-E\033[1;32m : Jumps the cursor to the end of the line.\r\n"             \
    "\033[1;31mCtrl-D\033[1;32m : Log Out.\r\n"                                             \
    "\033[1;31mCtrl-R\033[1;32m : Reverse-i-search.\r\n"                                    \
    "\033[1;31mPg-Up\033[1;32m  : History search backwards and auto completion.\r\n"        \
    "\033[1;31mPg-Down\033[1;32m: History search forward and auto completion.\r\n"          \
    "\033[1;31mHome\033[1;32m   : Jumps the cursor to the beginning of the line.\r\n"       \
    "\033[1;31mEnd\033[1;32m    : Jumps the cursor to the end of the line.\r\n"             \
    "\r\n"                                                                                  \


/// Version of the module
#define SHELLMINATOR_VERSION "1.1.2"

#define SHELLMINATOR_MOUSE_PARSER_BUFFER_SIZE 12

/// Basic text formatting.
///
/// You can use this macro to modify the style or color of the printed text.
/// It can be used without a Shellminator object.
/// @note It will only work with VT100 compatible terminal emulators. Sadly Arduino Serial
///       monitor does not support these features.
///
/// This macro is made to make the usage of @ref setFormatFunc function safe.
/// @note please use this macro instead of @ref setFormatFunc.
/// @param streamObject Pointer to a Stream object.
/// @param ... Format specifiers. You can give as many specifier as you like.
///
/// Example: @code{cpp} Shellminator::setFormat( &Serial, Shellminator::BOLD, Shellminator::BLINKING, Shellminator::YELLOW ); @endcode
#define setFormat( streamObject, ... ) setFormatFunc( (streamObject), __VA_ARGS__, -1 );

/// Basic text formatting.
///
/// You can use this macro to modify the style or color of the printed text.
/// It can only be used with a Shellminator object.
/// @note It will only work with VT100 compatible terminal emulators. Sadly Arduino Serial
///       monitor does not support these features.
///
/// This macro is made to make the usage of @ref formatFunc function safe.
/// @note please use this macro instead of @ref formatFunc.
/// @param ... Format specifiers. You can give as many specifier as you like.
///
/// Example: @code{cpp} shell.format( Shellminator::BOLD, Shellminator::BLINKING, Shellminator::YELLOW ); @endcode
#define format( ... ) formatFunc( __VA_ARGS__, -1 );

/// Shellminator object
///
/// It can be used to interface with a <a href="https://en.wikipedia.org/wiki/VT100">VT100</a> compatible terminal like
/// <a href="https://www.putty.org/">PuTTY</a>, <a href="https://ttssh2.osdn.jp/index.html.en">Terra Term</a> or
/// <a href="https://linux.die.net/man/1/minicom">minicom</a>.
/// The module requires an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
class Shellminator
{

public:
    /// VT100 color codes
    ///
    /// This enum holds all of the <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible color codes</a>.
    enum textColor_t
    {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37
    };

    /// VT100 font sytles
    ///
    /// This enum holds all of the <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible font styles</a>.
    enum textStyle_t
    {
        REGULAR = 0,
        BOLD = 1,
        LOW_INTENSITY = 2,
        ITALIC = 3,
        UNDERLINE = 4,
        BLINKING = 5,
        REVERSE = 6,
        BACKGROUND = 7,
        INVISIBLE = 8
    };

    /// String, that holds the version information
    static const char *version;

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

    /// Constructor for WiFi Server
    ///
    /// This constructor only works on ESP32, and ESP8266.
    /// It is used to create a telnet based terminal.
    /// @param server_p Pointer to a WiFiServer object.
    Shellminator(WiFiServer *server_p);

    /// Start WiFi Server
    ///
    /// Use this function to start the WiFiServer object.
    void beginServer();

    /// Stop WiFi Server
    ///
    /// Use this function to stop the WiFiServer object.
    void stopServer();

    void setClientTimeout(uint16_t clientTimeout_p);

#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE

    Shellminator(WebSocketsServer *wsServer_p);

    Shellminator(WebSocketsServer *wsServer_p, uint8_t serverID_p);

    void webSocketPush(uint8_t data);

    void webSocketPush(uint8_t *data, size_t size);

    void websocketDisconnect();

#endif

    /// Shell object constructor
    ///
    /// This is a simple constructor for a Shellminator object.
    /// @param stream_p Pointer to a Stream object which will be used for communication.
    Shellminator(Stream *stream_p);

    /// Enable buffering to gain speed.
    ///
    /// With this function, a buffer can be attached to the object.
    /// It can be used to accelerate the printing process. Because this
    /// library meant to work on low power devices, with limited amount of
    /// dynamic memory, all rendering happens in place. It has one downside
    /// tough. The printing not happens in one time. Small independent printing
    /// actions renders the final result. The frontend on a PC can't handle this
    /// very well, if the data is coming fast. A much better solution is to
    /// collect the printed data into a buffer, and flush it, when we are finished.
    /// You can achieve this functionality with this function.
    /// @param buffer Pointer to a buffer. It has to be uint8_t type.
    /// @param bufferSize The size of the buffer in elements.
    /// @returns When the buffering is enabled successfully, it will return true.
    /// @note On low-end devices like an AVR, it might be too much. I suggest to use this
    ///       functionality on systems that has at least 10kBytes of dynamic memory.
    bool enableBuffering( uint8_t* buffer, int bufferSize );

    /// Register a function callback for command execution.
    ///
    /// With this function you can attach an external function to the object
    /// This function will be called when a command is typed and the return
    /// key is pressed.
    /// @param execution_fn_p Function pointer to the execution function.
    ///
    /// The execution function prototype must be like this:
    /// @code{cpp} void myExecFunc( char* command, Shellminator* caller ); @endcode
    ///
    /// You can use any name you like, but the arguments and the return type has to
    /// be the same. Practical example for impementation:
    ///
    /// @code{cpp}
    /// void myExecFunc( char* command, Shellminator* caller ){
    ///     caller -> channel -> print( "Hurray! I got this: " );
    ///     caller -> channel -> println( command );
    /// }
    /// @endcode
    void attachExecFunc(void (*execution_fn_p)(char *, Shellminator *));

    /// Shellminator initialization function
    ///
    /// This function initializes the object and prints the startup logo.
    /// @note The length of this string has to be less, or equal than SHELLMINATOR_BANNER_LEN.
    /// Leftover characters are truncated!
    /// @warning You have to call this function before all other member functions!
    /// @param banner_p this is equivalent to a user name in linux like terminals. It is just a visual thing.
    void begin(const char *banner_p);

    /// Sends a backspace
    ///
    /// This function makes a backspace in the terminal application. Basically it deletes the last character
    /// in the terminal screen.
    void sendBackspace();

    /// Clear screen
    ///
    /// This function clears the terminal screen.
    void clear();

    /// Update function
    ///
    /// This function handles all of the communication related stuff between the code and the terminal application.
    /// @warning This function has to be called periodically.
    ///          If the calling of this function is not frequent enough it can cause buffer overflow in the Serial driver!
    void update();

    /// Basic text formatting.
    ///
    /// You can use this function to modify the style or color of the printed text. This function
    /// can be accessed from outside of an object.
    /// @note It will only work with VT100 compatible terminal emulators. Sadly Arduino Serial
    ///       monitor does not support these features.
    /// @param stream_p Pointer to a Stream object.
    /// @param firstArg The first format specifier.
    /// @param ... All other format specifiers. __The last argument must be a negative integer number!__
    /// @warning There is a dedicated macro for this function, called @ref setFormat. Please
    ///          use this macro to avoid problems.
    static void setFormatFunc( Stream *stream_p, int firstArg, ... );

    /// Basic text formatting.
    ///
    /// You can use this function to modify the style or color of the printed text. This function
    /// can only be accessed with an object.
    /// @note It will only work with VT100 compatible terminal emulators. Sadly Arduino Serial
    ///       monitor does not support these features.
    /// @param firstArg The first format specifier.
    /// @param ... All other format specifiers. __The last argument must be a negative integer number!__
    /// @note It will only do anything if the formatting is enabled on the corresponding object.
    ///       Please check @ref enableFormatting for more information.
    /// @warning There is a dedicated macro for this function, called @ref format. Please
    ///          use this macro to avoid problems.
    void formatFunc( int firstArg, ... );

    /// Hide the cursor.
    ///
    /// This function disables the cursor on the host terminal. This function can only be accessed
    /// with an object.
    /// @note It will only do anything if the formatting is enabled on the corresponding object.
    ///       Please check @ref enableFormatting for more information.
    void hideCursor();

    /// Hide the cursor.
    ///
    /// This function disables the cursor on the host terminal. This function can be accessed
    /// outside of the class.
    /// @param stream_p Pointer to a Stream object.
    static void hideCursor(Stream *stream_p);

    /// Show the cursor.
    ///
    /// This function enables the cursor on the host terminal. This function can only be accessed
    /// with an object.
    /// @note It will only do anything if the formatting is enabled on the corresponding object.
    ///       Please check @ref enableFormatting for more information.
    void showCursor();

    /// Show the cursor.
    ///
    /// This function enables the cursor on the host terminal. This function can be accessed
    /// outside of the class.
    /// @param stream_p Pointer to a Stream object.
    static void showCursor(Stream *stream_p);

    /// Get the position of the cursor.
    ///
    /// This function can be used to query the current location
    /// of the cursor. The result will be passed back on the arguments.
    /// @param x Pointer to an integer. The horizontal coordinate will be
    ///          written to the variable where this pointer points.
    /// @param y Pointer to an integer. The horizontal coordinate will be
    ///          written to the variable where this pointer points.
    /// @param timeout You can specify a maximum time to wait for the answer
    ///                in milliseconds. the default value is 100ms.
    /// @returns If the query was successful it will return true. Otherwise
    ///          the value in the variables corresponding to x and y is
    ///          not usable.
    ///
    /// The screen coordinate system works like this:
    /// ![Screen Coordinate System](screen_space.png)
    /// The __origin__ is on the top left and it's coordinate is 1;1.
    /// The screen size can vary.
    bool getCursorPosition( int *x, int *y, uint32_t timeout = 250 );

    /// Set the position of the cursor.
    ///
    /// This function can be used to set the cursor location to a specified
    /// coordinate. This function can only be accessed with an object.
    /// @param x New X-coordinate of the cursor.
    /// @param y New Y-coordinate of the cursor.
    ///
    /// The screen coordinate system works like this:
    /// ![Screen Coordinate System](screen_space.png)
    /// The __origin__ is on the top left and it's coordinate is 1;1.
    /// The screen size can vary.
    /// @note Setting an invalid cursor position is not possible, it is
    ///       protected by the terminal interface and this function as well.
    void setCursorPosition( int x, int y );

    /// Set the position of the cursor.
    ///
    /// This function can be used to set the cursor location to a specified
    /// coordinate. This function can be accessed outside of the class.
    /// @param channel_p Pointer to a Stream object.
    /// @param x New X-coordinate of the cursor.
    /// @param y New Y-coordinate of the cursor.
    ///
    /// The screen coordinate system works like this:
    /// ![Screen Coordinate System](screen_space.png)
    /// The __origin__ is on the top left and it's coordinate is 1;1.
    /// The screen size can vary.
    /// @note Setting an invalid cursor position is not possible, it is
    ///       protected by the terminal interface and this function as well.
    static void setCursorPosition( Stream* channel_p, int x, int y );

    bool getTerminalSize( int *width, int *height );

    /// This is a helper function for pointer casting.
    ///
    /// It is designed to be used with Commander-API, when you have to cast
    /// a void* to Shellminator*.
    /// @param ptr Pointer to a Shellminator object in void*.
    /// @warning The address stored in ptr has to be a valid addres for a Shellminator object.
    /// @returns Casted pointer.
    /// @todo track the created objects in an internal static array and compare their pointer
    ///       values with the given one.
    static Shellminator *castVoidToShellminator( void *ptr );

    /// Draws the startup logo
    ///
    /// Draws the startup logo in the terminal
    void drawLogo();

    /// This function prints the banner text.
    void printBanner();

    /// Print command history.
    void printHistory();

    /// Print help text.
    void printHelp();

    /// This function sets the banner text.
    ///
    /// It can be used when you want to change the banner text runtime.
    /// @param banner_p String that contains the new banner text.
    ///
    /// ![Banner Text Elements](banner_text.png)
    void setBannerText(const char *banner_p);

    /// This function sets the banner path text.
    ///
    /// It can be used when you want to change the banner path text runtime.
    /// @param banner_p String that contains the new banner text.
    /// @note the default banner path text is `$`
    ///
    /// ![Banner Text Elements](banner_text.png)
    void setBannerPathText(const char *bannerPath_p);

    /// This function attaches a logo to the terminal.
    ///
    /// The logo is just a character array.
    /// To create costum startup logo: https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino
    /// To make it to a c-string: https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
    /// Add '\r' to all line end.
    /// @param logo_p Pointer to the logo's address.
    void attachLogo(const char *logo_p);

#ifdef __AVR__
    /// This function attaches a logo to the terminal.
    ///
    /// The logo is just a character array.
    /// To create custom startup logo: https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino
    /// To make it to a c-string: https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
    /// Add '\r' to all line end.
    /// @param logo_p Pointer to the logo's address.
    void attachLogo(__FlashStringHelper *progmemLogo_p);
#endif

    /// Override up arrow key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the up arrow key
    /// is pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideUpArrow(void (*func)(void));

    /// Override down arrow key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the down arrow key
    /// is pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideDownArrow(void (*func)(void));

    /// Override left arrow key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the left arrow key
    /// is pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideLeftArrow(void (*func)(void));

    /// Override right arrow key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the right arrow key
    /// is pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideRightArrow(void (*func)(void));

    /// Override abort key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the abort key is
    /// pressed. The default abort key is usually a Ctrl + C
    /// combo.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideAbortKey(void (*func)(void));

    /// Override Page-Up key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the Page-Up key is
    /// pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overridePageUpKey(void (*func)(void));

    /// Override Page-Down key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the Page-Down key is
    /// pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overridePageDownKey(void (*func)(void));

    /// Override Home key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the Home key is
    /// pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideHomeKey(void (*func)(void));

    /// Override End key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the End key is
    /// pressed.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideEndKey(void (*func)(void));

    /// Override Logout key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the Logout key is
    /// pressed. The default Logout key is usually a Ctrl + D
    /// combo.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideLogoutKey(void (*func)(void));

    /// Override Search key behaviour.
    ///
    /// With this function you can attach a function that
    /// will be called every time when the Logout key is
    /// pressed. The default Search key is usually a Ctrl + R
    /// combo.
    /// @param func Pointer to the function that will be called on keypress.
    void overrideSearchKey(void (*func)(void));

    /// Reset up arrow key functionality to default.
    ///
    /// This function resets the up arrow functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeUpArrow();

    /// Reset down arrow key functionality to default.
    ///
    /// This function resets the down arrow functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeDownArrow();

    /// Reset left arrow key functionality to default.
    ///
    /// This function resets the left arrow functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeLeftArrow();

    /// Reset right arrow key functionality to default.
    ///
    /// This function resets the right arrow functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeRightArrow();

    /// Reset abort key functionality to default.
    ///
    /// This function resets the abort key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeAbortKey();

    /// Reset Page-Up key functionality to default.
    ///
    /// This function resets the Page-Up key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freePageUpKey();

    /// Reset Page-Down key functionality to default.
    ///
    /// This function resets the Page-Down key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freePageDownKey();

    /// Reset Home key functionality to default.
    ///
    /// This function resets the Home key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeHomeKey();

    /// Reset End key functionality to default.
    ///
    /// This function resets the End key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeEndKey();

    /// Reset Logout key functionality to default.
    ///
    /// This function resets the Logout key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeLogoutKey();

    /// Reset Search key functionality to default.
    ///
    /// This function resets the Search key functionality
    /// to default. If you want to detach the override
    /// function for the key, you have to call this function.
    void freeSearchKey();

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

    /// Disconnect WiFiClient telnet client
    void clientDisconnect();

#endif

#ifdef SHELLMINATOR_ENABLE_PASSWORD_MODULE

    void enablePasswordProtection(uint8_t *passwordHashAddress_p);
    void enablePasswordProtection(const uint8_t *passwordHashAddress_p);
    void enablePasswordProtection(char *passwordHashAddress_p);
    void enablePasswordProtection(const char *passwordHashAddress_p);
    void disablePasswordProtection();
    bool checkPassword(uint8_t *pwStr);
    bool checkPassword(const uint8_t *pwStr);
    bool checkPassword(char *pwStr);
    bool checkPassword(const char *pwStr);

#endif

    /// Register Screen object to draw.
    ///
    /// This function can be used to register a Screen object to the terminal.
    /// After the Screen object is registered, it will take over the control against
    /// the terminal interface.
    /// @param screen_p Pointer to a screen object.
    /// @param updatePeriod Optionally, you can specify the screen refresh time in milliseconds.
    //                      I recommend to don't go below 150ms.
    /// @note To close the Screen, you have to press the return or the abort( ctrl+c ) key.
    ///
    /// @note To close the Screen session from code, you can use the @ref endScreen function.
    void beginScreen( ShellminatorScreen* screen_p, int updatePeriod = 250 );

    /// Abort Screen session.
    ///
    /// With this function you can abort a registered Screen session.
    void endScreen();

    /// Redraw request from a Screen object.
    ///
    /// This function is used by the attached Screen object. The Screen object can
    /// signal the terminal interface with this function, to call the draw function
    /// in the next drawing session. The drawing only happens, when an event triggers it,
    /// to save some CPU time. This way the Screen drawing is much efficient.
    void requestRedraw();

    /// Shell Event enumeration.
    typedef enum{
       SHELL_EVENT_EMPTY,       ///< This is used to handle default or empty values. If this value is assigned to an event, it won't do anything.
       SHELL_EVENT_RESIZE,      ///< If the object detects a resize event, it will be available to the Screen object with this flag.
       SHELL_EVENT_MOUSE,       ///< To identify mouse related events.
       SHELL_EVENT_KEY,         ///< To identify simple key events like: `A`, `b`... @note Case sensitive!
       SHELL_EVENT_CODED_KEY    ///< To identify special, coded keys like: `Up Arrow`, `HOME`...
    }shellEventType_t;

    /// Coded event enumeration.
    typedef enum{
        EVENT_CODE_EMPTY,                   ///< This is used to handle default or empty values. If this value is assigned to an event, it won't do anything.
        EVENT_CODE_MOUSE_LEFT_PRESSED,      ///< Left Mouse Button Pressed
        EVENT_CODE_MOUSE_LEFT_RELEASED,     ///< Left Mouse Button Released
        EVENT_CODE_MOUSE_RIGHT_PRESSED,     ///< Right Mouse Button Pressed
        EVENT_CODE_MOUSE_RIGHT_RELEASED,    ///< Right Mouse Button Released
        EVENT_CODE_MOUSE_MIDDLE_PRESSED,    ///< Middle Mouse Button Pressed
        EVENT_CODE_MOUSE_MIDDLE_RELEASED,   ///< Middle Mouse Button Released
        EVENT_CODE_MOUSE_WHEEL_UP,          ///< Mouse Wheel Scrolled Up
        EVENT_CODE_MOUSE_WHEEL_DOWN,        ///< Mouse Wheel Scrolled Down
        EVENT_CODE_UP_ARROW,                ///< Up Arrow Pressed
        EVENT_CODE_DOWN_ARROW,              ///< Down Arrow Pressed
        EVENT_CODE_LEFT_ARROW,              ///< Left Arrow Pressed
        EVENT_CODE_RIGHT_ARROW,             ///< Right Arrow Pressed
        EVENT_CODE_HOME,                    ///< Home Button Pressed
        EVENT_CODE_END                      ///< End Button Pressed
    }eventCodes_t;

    /// Shell event structure.
    ///
    /// This structure holds the necessary fields
    /// to store and decode shell events. It is
    /// used to communicate with the attached Screen
    /// objects.
    typedef struct{
        shellEventType_t type;      ///< Identifies the type of the event.
        eventCodes_t eventCode;     ///< Stores the event code in case of `SHELL_EVENT_CODED_KEY` or `SHELL_EVENT_MOUSE` type.
        uint8_t data;               ///< In case of `SHELL_EVENT_KEY` type, stores the corresponding character to the pressed key. @note Case sensitive!
        uint8_t x;                  ///< In case of `SHELL_EVENT_MOUSE` type, stores the X-coordinate of the mouse event.
        uint8_t y;                  ///< In case of `SHELL_EVENT_MOUSE` type, stores the Y-coordinate of the mouse event.
    }shellEvent_t;

    /// The events are stored in this buffer.
    ///
    /// It uses a circular structure to store the events.
    /// Usually this buffer should not has to be big.
    shellEvent_t eventBuffer[ EVENT_BUFFER_SIZE ];

    /// Write position for the eventBuffers circular structure.
    uint8_t eventBufferWritePtr;

    /// Read position for the eventBuffers circular structure.
    uint8_t eventBufferReadPtr;

    /// Get the number of events available for reading from the eventBuffer.
    /// @return The number of events available to read.
    int eventAvailable();

    /// Read an event.
    ///
    /// With this function, you can read an event from the event buffer.
    /// Unlike Arduino-like read methods, this won't pop the event from
    /// the buffer after reading. This way a complex Screen layout is
    /// much easier to make.
    /// @return The next available event from the event buffer.
    /// @note If there is no available event in the buffer, the returned
    ///       event type will be `SHELL_EVENT_EMPTY`.
    shellEvent_t readEvent();

    /// Remove the current element from the buffer.
    ///
    /// It has to be used after calling the update function of the attached Screen object.
    /// This way an event won't get parsed multiple times.
    void popEvent();

    /// Stores the width of the terminal in characters.
    int terminalWidth = 1;

    /// Stores the height of the terminal in characters.
    int terminalHeight = 1;

    /// This buffer is used to parse the mouse coordinates form the
    /// host terminals answer.
    char mouseEventBuffer[ SHELLMINATOR_MOUSE_PARSER_BUFFER_SIZE ];

    /// This variable traks the next free characters location in the mouseEventBuffer.
    uint8_t mouseEventBufferCounter = 0;

    /// Enable mouse reports.
    ///
    /// With this function you can enable X-term like
    /// mouse reporting on the host terminal.
    /// @note Sadly it won't work on the Windows emulator @emoji :disappointed:.
    ///       However it works with Xterm.js and PuTTY.
    void mouseBegin();
    void mouseEnd();

    /// Wait for specific keypress
    ///
    /// It can be useful with a simple prompt like waiting for Y or N characters.
    /// @param source Pointer to a source stream. The function will wait for a key to arrive on this channel.
    /// @param key Expected key. For example in a Yes/No prompt you expect 'Y' or 'y' to arrive.
    /// @param timeout Timeout in ms. If it is 0, that means no timeout. [ optional, 0 by default ]
    /// @returns True if the expected key is arrived.
    static bool waitForKey(Stream *source, char key, uint32_t timeout = 0);

    /// Wait for specific keypress
    ///
    /// It can be useful with a simple prompt like waiting for Y or N characters.
    /// It can be useful, for multiple key detection. For example upper and lower case detection as well.
    /// @param source Pointer to a source stream. The function will wait for a key to arrive on this channel.
    /// @param keys Character array. Array of characters for match detection.
    /// @param timeout Timeout in ms. If it is 0, that means no timeout. [ optional, 0 by default ]
    /// @returns True if any of the expected keys match.
    static bool waitForKey(Stream *source, char *keys, uint32_t timeout = 0);

    /// Input prompt.
    ///
    /// It is a simple prompt for user input.
    void input( char *buffer, int bufferSize, const char *instruction, void(*callback)(char*, int, Shellminator*), bool secret = false );
    

    /// Select list.
    ///
    /// It is a simple input prompt to select elements from a list. There are two working modes:
    /// * __Single element selection__ - In this mode you can navigate with the __UP / DOWN arrow keys__.
    ///                              If you selected the right option, just press __enter__ to finish.
    /// * __Multiple element selection__ - In this mode you can navigate with the __UP / DOWN arrow keys__,
    ///                                and you can select each element in the list with the __space key__.
    ///                                If you finished, just press the __enter key__.
    ///
    /// To abort this prompt the following keys can be used:
    /// * __Ctrl-C__
    /// * __Backspace__
    ///
    /// @param source Pointer to a source stream. The function will wait for a key to arrive on this channel.
    /// @param lineText Character array. You can specify the prompt instructions here.
    /// @param numberOfElements The number of elements in the list.
    /// @param list The list of the options( string array )
    /// @param timeout Timeout in ms. If it is 0, that means no timeout.
    /// @param selection bool array to store the status of the selected elements. The size of this array should
    ///                  be equal to the size of the list. If an element is selected, the corresponding element
    ///                  in this array will be true.
    ///                  __To enable multiple element selection mode, this pointer has to point to a valid array!__
    ///                  [ optional, NULL by default( single element mode ) ]
    /// @returns In single element mode, it will return the index of the selected element. In multiple element
    ///          mode, it will return the number of selected elements from the list. If timeout or abort event
    ///          occurs, it will return -1.
    static int selectList(Stream *source, char *lineText, int numberOfElements, char *list[], uint32_t timeout, bool *selection = NULL);

    /// Generate a beep sound on the terminal device.
    void beep();

    /// This flag enables or disables character formatting.
    /// It can be useful when VT100 format parser is not
    /// available on the host device.
    bool enableFormatting = true;

    /// If set, the buzzer will be silent.
    bool mute = false;

    /// Default communication channel;
    shellminatorDefaultChannel defaultChannel;

    /// Pointer to the communication class. By default
    /// it points to the default response handler.
    Stream *channel = &defaultChannel;

    /// Get the buffered printer object address.
    ///
    /// With this function the bufferedPrinter object can be accessed
    /// from outside.
    /// @returns If it returns NULL that means, the buffered printer is not enabled, or the memory allocation for it is failed.
    ///          In this case it is not usable. If it is not NULL, you can use it.
    ShellminatorBufferedPrinter* getBufferedPrinter();

#ifdef COMMANDER_API_VERSION

    void attachCommander(Commander *commander_p);

#endif

// Configuration for QR code specific parts.
#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

    /// This function generates a QR-code from text
    ///
    /// With this function you can create QR-codes from text data( links as well ) and
    /// show the QR-code in the terminal. It can be handy with links or error codes.
    /// @param text The text or link that you want to compress into a QR-code.
    /// @note The error correction is Medium by default.
    /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
    void generateQRText(char *text);

    /// This function generates a QR-code from text
    ///
    /// With this function you can create QR-codes from text data( links as well ) and
    /// show the QR-code in the terminal. It can be handy with links or error codes.
    /// @param text The text or link that you want to compress into a QR-code.
    /// @note The error correction is Medium by default.
    /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
    void generateQRText(const char *text);

    /// This function generates a QR-code from text
    ///
    /// With this function you can create QR-codes from text data( links as well ) and
    /// show the QR-code in the terminal. It can be handy with links or error codes.
    /// @param text The text or link that you want to compress into a QR-code.
    /// @param ecc Error correction level.
    /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
    void generateQRText(char *text, enum qrcodegen_Ecc ecc);

    /// This function generates a QR-code from text
    ///
    /// With this function you can create QR-codes from text data( links as well ) and
    /// show the QR-code in the terminal. It can be handy with links or error codes.
    /// @param text The text or link that you want to compress into a QR-code.
    /// @param ecc Error correction level.
    /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
    void generateQRText(const char *text, enum qrcodegen_Ecc ecc);

#endif

    void autoDetectTerminal();

    friend class ShellminatorProgress;

private:
    // It can be used to accelerate the data sending process.
    // With this, the output will be rendered onec without flickering.
    ShellminatorBufferedPrinter bufferedPrinter;

    // If memory allocation is failed for the buffer, this flag will be false.
    bool bufferMemoryAllocated = false;

    ShellminatorScreen* screen = NULL;
    unsigned long screenTimerStart;
    int screenUpdatePeriod;
    bool screenRedraw;

    // State-machine functions.
    /// @todo Finish the documentation for state-machine part.
    void ShellminatorDefaultState(char new_char);
    void ShellminatorEnterKeyState();
    void ShellminatorBackspaceState();
    void ShellminatorBeginningOfLineState();
    void ShellminatorEndOfLineState();
    void ShellminatorLogoutState();
    void ShellminatorReverseSearchState();
    void ShellminatorClearScreenState();
    void ShellminatorAutoCompleteState();
    void ShellminatorAbortState();
    void ShellminatorEscapeCharacterState(char new_char);
    void ShellminatorEscapeBracketState(char new_char);
    void ShellminatorUpArrowKeyState();
    void ShellminatorDownArrowKeyState();
    void ShellminatorLeftArrowKeyState();
    void ShellminatorRightArrowKeyState();
    void ShellminatorHomeKeyState();
    void ShellminatorHomeKeyState(char new_char);
    void ShellminatorEndKeyState();
    void ShellminatorEndKeyState(char new_char);
    void ShellminatorDelKeyState();
    void ShellminatorDelKeyState(char new_char);
    void ShellminatorPageUpKeyState();
    void ShellminatorPageUpKeyState(char new_char);
    void ShellminatorPageDownKeyState();
    void ShellminatorPageDownKeyState(char new_char);
    void ShellminatorProcessRegularCharacter(char new_char);
    void ShellminatorMouseEventParserState(char new_char);

    void (Shellminator::*currentState)(char) = &Shellminator::ShellminatorDefaultState;

    /// Pointer to a string that holds the startup logo
    ///
    /// Simple text that holds the startup logo. You can create custom logos
    /// with a <a href="https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20">text to ASCII converter</a>.
    /// @warning Make sure that the generated string is c/c++ compatible!
    char *logo = NULL;

    void pushEvent( shellEvent_t event );
    void parseMouseData();

#ifdef __AVR__
    __FlashStringHelper *progmemLogo = NULL;
#endif

    /// This function-pointer stores the execution function pointer.
    /// This function will be called when a command recives.
    void (*execution_fn)( char*, Shellminator* );

    /// Text buffer
    ///
    /// This array stores the incoming and the previous commands.
    /// The 0th element always reserved to hold the incoming data.
    /// All other elements are holds the previous commands. Every new command
    /// shifts the elements towards the higher index, and removes the highest index element.
    /// To navigate between the previous commands you can use the UP and DOWN arrows
    /// on the keyboard. To specify the 'memory' of the interface you have to configure
    /// the \link SHELLMINATOR_BUFF_DIM \endlink definition.
    /// @warning The value of the \link SHELLMINATOR_BUFF_DIM \endlink definition has to be at least 2!
    /// @note Be careful with the \link The value of the \endlink definition. If it is to high your RAM will be eaten!
    char cmd_buff[SHELLMINATOR_BUFF_DIM][SHELLMINATOR_BUFF_LEN + 1] = {{0}};

    /// This variable tracks the index of the previous command while you browsing the command history
    uint32_t cmd_buff_dim = 1;

    /// This variable tracks the end of the input message.
    uint32_t cmd_buff_cntr = 0;

    /// This variable tracks the location of the next character.
    uint32_t cursor = 0;

    /// This variable tracks the state of the VT100 decoder state-machine.
    uint32_t escape_state = 0;

    /// This character array stores the banner text.
    char banner[SHELLMINATOR_BANNER_LEN] = {'\0'};

    char bannerPath[SHELLMINATOR_BANNER_PATH_LEN] = "$";

    /// Size of the last printed banner in characters.
    /// It's used to accelerate the redrawing process.
    uint8_t lastBannerSize = 0;

    /// Function pointer for up arrow behaviour override.
    void (*upArrowOverrideFunc)(void) = NULL;

    /// Function pointer for down arrow behaviour override.
    void (*downArrowOverrideFunc)(void) = NULL;

    /// Function pointer for left arrow behaviour override.
    void (*leftArrowOverrideFunc)(void) = NULL;

    /// Function pointer for right arrow behaviour override.
    void (*rightArrowOverrideFunc)(void) = NULL;

    /// Function pointer for abort key behaviour override.
    void (*abortKeyFunc)(void) = NULL;

    /// Function pointer for Page-Up key behaviour override.
    void (*pageUpKeyFunc)(void) = NULL;

    /// Function pointer for Page-Down key behaviour override.
    void (*pageDownKeyFunc)(void) = NULL;

    /// Function pointer for Home key behaviour override.
    void (*homeKeyFunc)(void) = NULL;

    /// Function pointer for End key behaviour override.
    void (*endKeyFunc)(void) = NULL;

    /// Function pointer for Logout key behaviour override.
    void (*logoutKeyFunc)(void) = NULL;

    /// Function pointer for Search key behaviour override.
    void (*searchKeyFunc)(void) = NULL;

    /// This function processes a new character
    ///
    /// This function handles every character that arrives from the terminal software.
    /// It called by the \link update \endlink function every time when the Serial buffer
    /// has some unprocessed characters.
    /// @param new_char This is the nex character that has to be processed.
    void process(char new_char);

    /// This function insets a new character to the input buffer.
    void redrawLine();

    //---- Communication channels ----//

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

    WiFiServer *server = NULL;
    WiFiClient client;
    bool clientConnected = false;
    uint8_t telnetNegotiationState = 0;
    uint16_t clientTimeout = 1000;

    // https://www.omnisecu.com/tcpip/telnet-commands-and-options.php
    static const uint8_t TELNET_IAC_DONT_LINEMODE[3];
    static const uint8_t TELNET_IAC_WILL_ECHO[3];
    static const uint8_t TELNET_IAC_DONT_ECHO[3];
    static const uint8_t TELNET_IAC_WILL_SUPRESS_GO_AHEAD[3];
    static const uint8_t TELNET_IAC_DO_SUPRESS_GO_AHEAD[3];

#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE

    WebSocketsServer *wsServer = NULL;
    uint8_t serverID;
    shellminatorWebSocketChannel webSocketChannel;

#endif

//---- Commander-API support specific part ----//
#ifdef COMMANDER_API_VERSION

    /// Pointer to a Commander object.
    Commander *commander = NULL;

    /// Last time in ms when the input command was checked.
    uint32_t commandCheckTimerStart = 0;

    /// Flag that stores if the command was checked.
    bool commandChecked = false;

    /// Flag that stores that the command was
    /// found in Commander API-tree.
    bool commandFound = false;

#endif

    void historySearchBackward();
    void historySearchForward();
    void redrawHistorySearch();
    int substring(char *str1, char *str2);

    bool inSearch = false;
    int32_t searchMatch;

    bool inputActive = false;
    int inputInstuctionSize = 0;
    char* inputDestinationBuffer;
    int inputDestinationBufferSize;
    bool inputSecretMode;
    void(*inputCallback)(char*, int, Shellminator*);


#ifdef SHELLMINATOR_ENABLE_PASSWORD_MODULE

    SHA256_CTX passwordHashCtx;
    uint8_t passwordHashBuffer[SHA256_BLOCK_SIZE];
    uint8_t *passwordHashAddress = NULL;

#endif

// QR-code configuration specific parts.
#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

    uint8_t qr_data[qrcodegen_BUFFER_LEN_MAX];
    uint8_t qr_tempBuff[qrcodegen_BUFFER_LEN_MAX];

#endif
};

#endif
