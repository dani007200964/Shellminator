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

#include "Shellminator-Settings.hpp"
#include "Shellminator-IO.hpp"

#ifdef ARDUINO
#include "Arduino.h"
#endif

#ifdef SHELLMINATOR_USE_WIFI_CLIENT
	#ifdef ESP8266
	#include <ESP8266WiFi.h>
	#endif

	#ifdef ESP32
	#include <WiFi.h>
	#endif
#endif

#ifdef __has_include
  #if __has_include ("Commander-API.hpp")
    #include "Commander-API.hpp"
  #endif
#endif

#ifdef COMMANDER_API_VERSION
#include "Commander-API.hpp"
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

///  +------  Costum configuration  ------+
///  |                                    |
///  |  This is where you have to config  |
///  |           your defines!            |
///  |                                    |
///  +------------------------------------+

#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

#include "qrcodegen.h"

#endif

/// Version of the module
#define SHELLMINATOR_VERSION "1.1.2"

/// Shellminator object
///
/// It can be used to interface with a <a href="https://en.wikipedia.org/wiki/VT100">VT100</a> compatible terminal like
/// <a href="https://www.putty.org/">PuTTY</a>, <a href="https://ttssh2.osdn.jp/index.html.en">Terra Term</a> or
/// <a href="https://linux.die.net/man/1/minicom">minicom</a>.
/// The module requires an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
class Shellminator {

public:

  /// VT100 color codes
  ///
  /// This enum holds all of the <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible color codes</a>.
  enum {
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
  enum {
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

  /// String that holds the version information
  static const char *version;

#ifdef SHELLMINATOR_USE_ARDUINO_SERIAL
  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object.
  /// @param serialPort_p pointer to an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
  Shellminator( HardwareSerial *serialPort_p );

  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object with an execution function.
  /// @param serialPort_p pointer to an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
  /// @param execution_fn_p function pointer to the execution function. It has to be a void return type, with one argument, and that argument is a char*type.
  Shellminator( HardwareSerial *serialPort_p, void( *execution_fn_p )( char* ) );
#endif

#ifdef SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object.
  /// @param serialPort_p pointer to an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
  Shellminator( Serial_ *serialPort_p );

  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object with an execution function.
  /// @param serialPort_p pointer to an <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">Arduino-like Serial object</a>.
  /// @param execution_fn_p function pointer to the execution function. It has to be a void return type, with one argument, and that argument is a char*type.
  Shellminator( Serial_ *serialPort_p, void( *execution_fn_p )( char* ) );
#endif

#ifdef SHELLMINATOR_USE_WIFI_CLIENT
  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object.
  /// @param resp pointer to a WiFiClient object.
  Shellminator( WiFiClient *resp );

  /// Shellminator Constructor
  ///
  /// Constructor for a Shellminator object with an execution function.
  /// @param resp pointer to a WiFiClient object.
  /// @param execution_fn_p function pointer to the execution function. It has to be a void return type, with one argument, and that argument is a char*type.
  Shellminator( WiFiClient *resp, void( *execution_fn_p )( char* ) );

	Shellminator( WiFiServer *server_p );

	Shellminator( WiFiServer *server_p, void( *execution_fn_p )( char* ) );

	void beginServer();

	void stopServer();

#endif

  /// Execution function adder function
  ///
  /// This function allows you to add or replace the execution function after the constructor.
  /// @param execution_fn_p function pointer to the execution function. It has to be a void return type, with one argument, and that argument is a char*type.
  void addExecFunc( void( *execution_fn_p )( char* ) );

  /// Shellminator initialization function
  ///
  /// This function initializes the object and prints the startup logo.
  /// @note The length of this string has to be less, or equal than SHELLMINATOR_BANNER_LEN. Leftover characters are truncated!
  /// @warning You have to call this function before all other member functions!
  /// @param banner_p this is equivalent to a user name in linux like terminals. It is just a visual thing.
  void begin( char* banner_p );

  /// Shellminator initialization function
  ///
  /// This function initializes the object and prints the startup logo.
  /// @note The length of this string has to be less, or equal than SHELLMINATOR_BANNER_LEN. Leftover characters are truncated!
  /// @warning You have to call this function before all other member functions!
  /// @param banner_p this is equivalent to a user name in linux like terminals. It is just a visual thing.
  void begin( const char* banner_p );

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
  /// @warning If the calling of this function is not frequent enough it cann cause buffer overflow in the Serial driver!
  void update();

	/// Bring some color into your code.
  ///
  /// This function changes the color and style of the terminal application characters.
  /// @warning Please use the color and style enumeration table from this application as parameter.
  /// @param style <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible font styles</a>
  /// @param color <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible color code</a>
  void setTerminalCharacterColor( uint8_t style, uint8_t color );

	/// Bring some color into your code.
  ///
  /// This function changes the color and style of the terminal application characters.
	/// The output goes to a buffer;
  /// @warning Please use the color and style enumeration table from this application as parameter.
	/// @param buff The result is generated to this buffer. It will be terminated with '\0' character.
  /// @param style <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible font styles</a>
  /// @param color <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible color code</a>
  void setTerminalCharacterColor( char* buff, uint8_t style, uint8_t color );

  /// Bring some color into your code.
  ///
  /// This function changes the color and style of the terminal application characters.
  /// This function can be used outside of a Shellminator object.
  /// @warning Please use the color and style enumeration table from this application as parameter.
  /// @param style Arduino Serial object to print the style code.
  /// @param style <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible font styles</a>
  /// @param color <a href="https://www.nayab.xyz/linux/escapecodes.html">VT100 compatible color code</a>
  static void setTerminalCharacterColor( HardwareSerial *serialPort, uint8_t style, uint8_t color );

  /// Draws the startup logo
  ///
  /// Draws the startup logo in the terminal
  void drawLogo();

  /// This function sets the banner text.
  ///
  /// It can be used when you want to change the banner text runtime.
  /// @param banner_p String that contains the new banner text.
  void setBannerText( char* banner_p );

	void setBannerPathText( char* bannerPath_p );

  /// This function attaches a logo to the terminal.
  ///
  /// The logo is just a character array.
  /// To create costum startup logo: https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino
  /// To make it to a c-string: https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
  /// Add '\r' to all line end.
  /// @param logo_p Pointer to the logo's address.
  void attachLogo( char* logo_p );

  /// This function attaches a logo to the terminal.
  ///
  /// The logo is just a character array.
  /// To create costum startup logo: https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino
  /// To make it to a c-string: https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
  /// Add '\r' to all line end.
  /// @param logo_p Pointer to the logo's address.
  void attachLogo( const char* logo_p );

  /// Override up arrow key behaviour.
  ///
  /// With this function you can attach a function that
  /// will be called every time when the up arrow key
  /// is pressed.
  /// @param func Pointer to the function that will be called on keypress.
  void overrideUpArrow( void( *func )( void ) );

  /// Override down arrow key behaviour.
  ///
  /// With this function you can attach a function that
  /// will be called every time when the down arrow key
  /// is pressed.
  /// @param func Pointer to the function that will be called on keypress.
  void overrideDownArrow( void( *func )( void ) );

  /// Override left arrow key behaviour.
  ///
  /// With this function you can attach a function that
  /// will be called every time when the left arrow key
  /// is pressed.
  /// @param func Pointer to the function that will be called on keypress.
  void overrideLeftArrow( void( *func )( void ) );

  /// Override right arrow key behaviour.
  ///
  /// With this function you can attach a function that
  /// will be called every time when the right arrow key
  /// is pressed.
  /// @param func Pointer to the function that will be called on keypress.
  void overrideRightArrow( void( *func )( void ) );

  /// Override abort key behaviour.
  ///
  /// With this function you can attach a function that
  /// will be called every time when the abort key is
  /// pressed. The default abort key is usually a Ctrl + C
  /// combo.
  /// @param func Pointer to the function that will be called on keypress.
  void overrideAbortKey( void( *func )( void ) );

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

	#ifdef SHELLMINATOR_USE_WIFI_CLIENT

  /// Disconnect WiFiClient telnet client
  void clientDisconnect();

	#endif

	void beep();

  /// This flag enables or disables character formatting.
  /// It can be usefull when VT100 format parser is not
  /// available on the host device.
  bool enableFormatting = true;

	bool mute = false;

  #ifdef COMMANDER_API_VERSION

  void attachCommander( Commander* commander_p );

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
  void generateQRText( char* text );

  /// This function generates a QR-code from text
  ///
  /// With this function you can create QR-codes from text data( links as well ) and
  /// show the QR-code in the terminal. It can be handy with links or error codes.
  /// @param text The text or link that you want to compress into a QR-code.
  /// @note The error correction is Medium by default.
  /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
  void generateQRText( const char* text );

  /// This function generates a QR-code from text
  ///
  /// With this function you can create QR-codes from text data( links as well ) and
  /// show the QR-code in the terminal. It can be handy with links or error codes.
  /// @param text The text or link that you want to compress into a QR-code.
  /// @param ecc Error correction level.
  /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
  void generateQRText( char* text, enum qrcodegen_Ecc ecc );

  /// This function generates a QR-code from text
  ///
  /// With this function you can create QR-codes from text data( links as well ) and
  /// show the QR-code in the terminal. It can be handy with links or error codes.
  /// @param text The text or link that you want to compress into a QR-code.
  /// @param ecc Error correction level.
  /// @warning To enable the QR-code support, please uncomment SHELLMINATOR_ENABLE_QR_SUPPORT definition at the configuration section.
  void generateQRText( const char* text, enum qrcodegen_Ecc ecc );

  #endif

private:

  /// Pointer to a string that holds the startup logo
  ///
  /// Simple text that holds the startup logo. You can create costum logos
  /// with a <a href="https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20">text to ASCII converter</a>.
  /// @warning Make sure that the generated string is c/c++ compatible!
  char *logo = NULL;

  /// This function-pointer stores the execution function pointer.
  /// This function will be called when a command recives.
  void( *execution_fn )( char* );

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
  char cmd_buff[ SHELLMINATOR_BUFF_DIM ][ SHELLMINATOR_BUFF_LEN + 1 ] = { { 0 } };

  /// This variable tracks the index of the previous command while you browsing the command history
  uint32_t cmd_buff_dim = 1;

  /// This variable tracks the end of the input message.
  uint32_t cmd_buff_cntr = 0;

  /// This variable tracks the location of the next character.
  uint32_t cursor = 0;

  /// This variable tracks the state of the VT100 decoder state-machine.
  uint32_t escape_state = 0;

  /// This character array stores the banner text.
  char banner[ SHELLMINATOR_BANNER_LEN ] = { '\0' };

	char bannerPath[ SHELLMINATOR_BANNER_PATH_LEN ] = "$";

	/// Size of the last printed banner in characters.
	/// It's used to accelerate the redrawing process.
	uint8_t lastBannerSize = 0;

  /// Function pointer for up arrow behaviour override.
  void( *upArrowOverrideFunc )( void )    = NULL;

  /// Function pointer for down arrow behaviour override.
  void( *downArrowOverrideFunc )( void )  = NULL;

  /// Function pointer for left arrow behaviour override.
  void( *leftArrowOverrideFunc )( void )  = NULL;

  /// Function pointer for right arrow behaviour override.
  void( *rightArrowOverrideFunc )( void ) = NULL;

  /// Function pointer for abort key behaviour override.
  void( *abortKeyFunc )( void )           = NULL;

  /// This function processes a new character
  ///
  /// This function handles every character that arrives from the terminal software.
  /// It called by the \link update \endlink function every time when the Serial buffer
  /// has some unprocessed characters.
  /// @param new_char This is the nex character that has to be processed.
  void process( char new_char );

  /// This function prints the banner text.
  void printBanner();

  /// This function insets a new character to the input buffer.
  void redrawLine();

  //---- Communication channels ----//

  /// Default communication channel;
  shellminatorChannel defaultChannel;

  #ifdef SHELLMINATOR_USE_ARDUINO_SERIAL
  /// Arduino Hardware Serial as communication channel.
  shellminatorArduinoSerialChannel arduinoSerialChannel;
  #endif

  #ifdef SHELLMINATOR_USE_ARDUINO_32U4_SERIAL
  /// Arduino Hardware Serial as communication channel.
  shellminatorArduino32U4SerialChannel arduino32U4SerialChannel;
  #endif

	#ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
	// It is used for the ESP32 and ESP8266.
	// They are very slow to send only one byte of data.
	char acceleratorBuffer[ SHELLMINATOR_ACCELERATOR_BUFFER_LEN ];
	char *acceleratorBufferPtr;
	#endif

  #ifdef SHELLMINATOR_USE_WIFI_CLIENT
  /// WiFi Client as communication channel.
  shellminatorWiFiClientChannel wifiChannel;

	WiFiServer *server = NULL;
  WiFiClient client;
	bool clientConnected = false;
	uint8_t telnetNegotiationState = 0;

	// https://www.omnisecu.com/tcpip/telnet-commands-and-options.php
	static const uint8_t TELNET_IAC_DONT_LINEMODE[ 3 ];
	static const uint8_t TELNET_IAC_WILL_ECHO[ 3 ];
	static const uint8_t TELNET_IAC_DONT_ECHO[ 3 ];
	static const uint8_t TELNET_IAC_WILL_SUPRESS_GO_AHEAD[ 3 ];
	static const uint8_t TELNET_IAC_DO_SUPRESS_GO_AHEAD[ 3 ];

  #endif

  /// Pointer to the communication class. By default
  /// it points to the default response handler.
	shellminatorChannel *channel = &defaultChannel;

  //---- Commander-API support specific part ----//
  #ifdef COMMANDER_API_VERSION

  /// Pointer to a Commander object.
  Commander* commander = NULL;

  /// Last time in ms when the input command was checked.
  uint32_t commandCheckTimerStart = 0;

  /// Flag that stores if the command was checked.
  bool commandChecked = false;

  /// Flag that stores that the command was
  /// found in Commander API-tree.
  bool commandFound = false;

  #endif

	#ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

	void historySearchBackward();
	void historySearchForward();
	void redrawHistorySearch();
	int substring( char* str1, char* str2 );

	bool inSearch = false;
	int32_t searchMatch;

	#endif

  // QR-code configuration specific parts.
  #ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

  uint8_t qr_data[ qrcodegen_BUFFER_LEN_MAX ];
  uint8_t qr_tempBuff[ qrcodegen_BUFFER_LEN_MAX ];

  #endif

};

#endif
