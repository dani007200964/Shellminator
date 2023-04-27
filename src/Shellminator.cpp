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

#include "Shellminator.hpp"

#ifdef __has_include
  #if __has_include ("Commander-API.hpp")
    #include "Commander-API.hpp"
  #endif
#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE
#include <WebSocketsServer.h>
#endif

const char *Shellminator::version = SHELLMINATOR_VERSION;

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

#ifdef ESP32
  #define CLIENT_STATE client.connected()
#endif

#ifdef ESP8266
  #define CLIENT_STATE ( client.status() == ESTABLISHED )
#endif

const uint8_t Shellminator::TELNET_IAC_DONT_LINEMODE[]          = { 255, 254, 34 };
const uint8_t Shellminator::TELNET_IAC_WILL_ECHO[]              = { 255, 251, 1 };
const uint8_t Shellminator::TELNET_IAC_DONT_ECHO[]              = { 255, 254, 1 };
const uint8_t Shellminator::TELNET_IAC_WILL_SUPRESS_GO_AHEAD[]  = { 255, 251, 3 };
const uint8_t Shellminator::TELNET_IAC_DO_SUPRESS_GO_AHEAD[]    = { 255, 253, 3 };

Shellminator::Shellminator( WiFiServer *server_p ){

  server = server_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = NULL;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator( WiFiServer *server_p, void( *execution_fn_p )( char* ) ){

  server = server_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator( WiFiServer *server_p, void( *execution_fn_p )( char*, Shellminator* ) ){

  server = server_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // passing execution_fn_p to execution_fn_with_parrent
  execution_fn = NULL;
  execution_fn_with_parrent = execution_fn_p;

}

void Shellminator::beginServer(){

  if( server ){

    server -> begin();
    server -> setNoDelay( true );

  }

}

void Shellminator::stopServer(){

  if( server ){

    server -> stop();

  }

}

void Shellminator::setClientTimeout( uint16_t clientTimeout_p ){

  clientTimeout = clientTimeout_p;

}

#endif

#ifdef __AVR__

const char Shellminator::helpText[] PROGMEM = {
  "\r\n"
  "---- Shortcut Keys ----\r\n"
  "\r\n"
  "Ctrl-A : Jumps the cursor to the beginning of the line.\r\n"
  "Ctrl-E : Jumps the cursor to the end of the line.\r\n"
  "Ctrl-D : Log Out.\r\n"
  "Ctrl-R : Reverse-i-search.\r\n"
  "Pg-Up  : History search backwards and auto completion.\r\n"
  "Pg-Down: History search forward and auto completion.\r\n"
  "Home   : Jumps the cursor to the beginning of the line.\r\n"
  "End    : Jumps the cursor to the end of the line.\r\n"
  "\r\n"
};

#ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
const char Shellminator::helpTextFormatted[] PROGMEM = {
  "\r\n"
  "\033[1;31m----\033[1;32m Shortcut Keys \033[1;31m----\033[0;37m\r\n"
  "\r\n"
  "\033[1;31mCtrl-A\033[1;32m : Jumps the cursor to the beginning of the line.\r\n"
  "\033[1;31mCtrl-E\033[1;32m : Jumps the cursor to the end of the line.\r\n"
  "\033[1;31mCtrl-D\033[1;32m : Log Out.\r\n"
  "\033[1;31mCtrl-R\033[1;32m : Reverse-i-search.\r\n"
  "\033[1;31mPg-Up\033[1;32m  : History search backwards and auto completion.\r\n"
  "\033[1;31mPg-Down\033[1;32m: History search forward and auto completion.\r\n"
  "\033[1;31mHome\033[1;32m   : Jumps the cursor to the beginning of the line.\r\n"
  "\033[1;31mEnd\033[1;32m    : Jumps the cursor to the end of the line.\r\n"
  "\r\n"
};
#endif

#else

const char Shellminator::helpText[] = {
  "\r\n"
  "---- Shortcut Keys ----\r\n"
  "\r\n"
  "Ctrl-A : Jumps the cursor to the beginning of the line.\r\n"
  "Ctrl-E : Jumps the cursor to the end of the line.\r\n"
  "Ctrl-D : Log Out.\r\n"
  "Ctrl-R : Reverse-i-search.\r\n"
  "Pg-Up  : History search backwards and auto completion.\r\n"
  "Pg-Down: History search forward and auto completion.\r\n"
  "Home   : Jumps the cursor to the beginning of the line.\r\n"
  "End    : Jumps the cursor to the end of the line.\r\n"
  "\r\n"
};

#ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
const char Shellminator::helpTextFormatted[] = {
  "\r\n"
  "\033[1;31m----\033[1;32m Shortcut Keys \033[1;31m----\033[0;37m\r\n"
  "\r\n"
  "\033[1;31mCtrl-A\033[1;32m : Jumps the cursor to the beginning of the line.\r\n"
  "\033[1;31mCtrl-E\033[1;32m : Jumps the cursor to the end of the line.\r\n"
  "\033[1;31mCtrl-D\033[1;32m : Log Out.\r\n"
  "\033[1;31mCtrl-R\033[1;32m : Reverse-i-search.\r\n"
  "\033[1;31mPg-Up\033[1;32m  : History search backwards and auto completion.\r\n"
  "\033[1;31mPg-Down\033[1;32m: History search forward and auto completion.\r\n"
  "\033[1;31mHome\033[1;32m   : Jumps the cursor to the beginning of the line.\r\n"
  "\033[1;31mEnd\033[1;32m    : Jumps the cursor to the end of the line.\r\n"
  "\r\n"
};
#endif

#endif

#ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE

Shellminator::Shellminator(	WebSocketsServer *wsServer_p, uint8_t serverID_p ){

  wsServer = wsServer_p;
  serverID = serverID_p;
  webSocketChannel.select( wsServer, serverID );
  webSocketChannel.setTimeout( 10 );
  channel = &webSocketChannel;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = NULL;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator(	WebSocketsServer *wsServer_p ){

  wsServer = wsServer_p;
  serverID = 0;
  webSocketChannel.select( wsServer, serverID );
  webSocketChannel.setTimeout( 10 );
  channel = &webSocketChannel;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = NULL;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator(	WebSocketsServer *wsServer_p, uint8_t serverID_p, void( *execution_fn_p )( char* ) ){

  wsServer = wsServer_p;
  serverID = serverID_p;
  webSocketChannel.select( wsServer, serverID );
  channel = &webSocketChannel;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator(	WebSocketsServer *wsServer_p, uint8_t serverID_p, void( *execution_fn_p )( char*, Shellminator* ) ){

  wsServer = wsServer_p;
  serverID = serverID_p;
  webSocketChannel.select( wsServer, serverID );
  channel = &webSocketChannel;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // passing execution_fn_p to execution_fn_with_parrent
  execution_fn = NULL;
  execution_fn_with_parrent = execution_fn_p;

}

void Shellminator::webSocketPush( uint8_t data ){
  webSocketChannel.push( data );
}

void Shellminator::webSocketPush( uint8_t* data, size_t size ){
  webSocketChannel.push( data, size );
}

void Shellminator::websocketDisconnect(){

  if( wsServer ){
    wsServer -> disconnect( serverID );
  }

}

#endif

Shellminator::Shellminator( Stream *stream_p ){

  channel = stream_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = NULL;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator( Stream *stream_p, void( *execution_fn_p )( char* ) ){

  channel = stream_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = execution_fn_p;
  execution_fn_with_parrent = NULL;

}

Shellminator::Shellminator( Stream *stream_p, void( *execution_fn_p )( char*, Shellminator* ) ){

  channel = stream_p;

  // It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  // This has to be 1 minimum, because the 0th element is used for the incoming data.
  // The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  // Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  // Because we did not specified the execution function, we have to make it a NULL
  // pointer to make it detectable.
  execution_fn = NULL;
  execution_fn_with_parrent = execution_fn_p;

}

void Shellminator::setBannerText( char* banner_p ){

  // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  // Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

}

void Shellminator::setBannerText( const char* banner_p ){

  // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  // Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

}

void Shellminator::setBannerPathText( char* bannerPath_p ){

  // Copy the content from bannerPath_p to bannerPath. Because strncpy we can be sure that it wont overflow.
  strncpy( bannerPath, bannerPath_p, SHELLMINATOR_BANNER_PATH_LEN );

  // Just in case close the string
  bannerPath[ SHELLMINATOR_BANNER_PATH_LEN - 1 ] = '\0';

}

void Shellminator::setBannerPathText( const char* bannerPath_p ){

  // Copy the content from bannerPath_p to bannerPath. Because strncpy we can be sure that it wont overflow.
  strncpy( bannerPath, bannerPath_p, SHELLMINATOR_BANNER_PATH_LEN );

  // Just in case close the string
  bannerPath[ SHELLMINATOR_BANNER_PATH_LEN - 1 ] = '\0';

}

void Shellminator::attachLogo( char* logo_p ){

  logo = logo_p;

}

#ifdef __AVR__
void Shellminator::attachLogo( __FlashStringHelper * progmemLogo_p ){

  progmemLogo = progmemLogo_p;

}
#endif

void Shellminator::attachLogo( const char* logo_p ){

  logo = (char*)logo_p;

}

void Shellminator::addExecFunc( void( *execution_fn_p )( char* ) ){

  // passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;
  execution_fn_with_parrent = NULL;

}

void Shellminator::addExecFunc( void( *execution_fn_p )( char*, Shellminator* ) ){

  // passing execution_fn_p to execution_fn_with_parrent
  execution_fn = NULL;
  execution_fn_with_parrent = execution_fn_p;

}

void Shellminator::clear() {

  // explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[H" );  // VT100 Home command
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[J" );  // VT100 screen erase command

}

void Shellminator::printBanner() {

  lastBannerSize = 0;

  // Sets the terminal style to bold and the color to green.
  // You can change it if you like. In my opinion the most
  // useful is the invisible one :)
  setTerminalCharacterColor( BOLD, GREEN );

  // Print the banner text and save it's size.
  lastBannerSize += channel -> print( banner );

  // Sets the terminal style to regular and the color to white.
  setTerminalCharacterColor( BOLD, WHITE );

  lastBannerSize += channel -> print( ':' );

  setTerminalCharacterColor( BOLD, BLUE );

  lastBannerSize += channel -> print( bannerPath );

  setTerminalCharacterColor( REGULAR, WHITE );

  lastBannerSize += channel -> print( ' ' );

}

void Shellminator::printHistory(){

  uint32_t i;
  uint32_t firstBuffDim = 1;
  uint32_t index;

  // Check the first valid command in the buffer
  for( i = 1; i < SHELLMINATOR_BUFF_DIM; i++ ){

    // If it's found, store it's index to firstBuffDim
    if( cmd_buff[ i ][ 0 ] == '\0' ){
      firstBuffDim = i - 1;
      break;
    }

  }

  // If the history is empty, we can return.
  if( firstBuffDim == 0 ){
    return;
  }

  // If the history is full, we have to protect
  // firstBuffDim variable.
  if( i >= SHELLMINATOR_BUFF_DIM ){
    firstBuffDim = SHELLMINATOR_BUFF_DIM - 1;
  }

  // Print the history.
  for( i = firstBuffDim; i > 0; i-- ){

    index = firstBuffDim - i + 1;

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    if( enableFormatting ){

      sprintf( acceleratorBuffer, "  \033[1;35m%3d  \033[0;37m%s\r\n", index, cmd_buff[ i ] );

    }

    else{

      sprintf( acceleratorBuffer, "  %3d  %s\r\n", index, cmd_buff[ i ] );

    }

    channel -> print( acceleratorBuffer );

    #else


    channel -> print( ' ' );
    channel -> print( ' ' );

    // It is used to ident digits.
    if( index < 10 ){

      channel -> print( ' ' );
      channel -> print( ' ' );

    }

    // It is used to ident digits.
    else if( index < 100 ){

      channel -> print( ' ' );

    }

    // Print the index and the command.
    setTerminalCharacterColor( BOLD, MAGENTA );
    channel -> print( index );
    setTerminalCharacterColor( REGULAR, WHITE );
    channel -> print( ' ' );
    channel -> print( ' ' );
    channel -> println( cmd_buff[ i ] );

    #endif

  }

}

void Shellminator::printHelp(){

  #ifdef __AVR__

  uint32_t i;

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  if( enableFormatting ){

    for( i = 0; i < strlen_P( helpTextFormatted ); i++ ){

      char c = pgm_read_byte_near( helpTextFormatted + i );
      channel -> print( c );

    }

  }

  else{

  #endif

    for( i = 0; i < strlen_P( helpText ); i++ ){

      char c = pgm_read_byte_near( helpText + i );
      channel -> print( c );

    }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  }
  #endif

  #else

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  if( enableFormatting ){

    channel -> print( helpTextFormatted );
  
  }

  else{
  #endif

    channel -> print( helpText );

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE
  }
  #endif

  #endif

  #ifdef COMMANDER_API_VERSION

  if( commander != NULL ){

    commander -> printHelp( channel, enableFormatting );

  }

  #endif

}

void Shellminator::begin( char* banner_p ) {

  // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  // Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

  // Draw the startup logo.
  drawLogo();

  // Print the banner message.
  printBanner();

}

void Shellminator::begin( const char* banner_p ) {

  // Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  // Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

  // Set the terminal color and style to the defined settings for the logo
  setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

  // Draw the startup logo.
  drawLogo();

  // Print the banner message.
  printBanner();

}

void Shellminator::sendBackspace() {

  // Send a simple backspace combo to the serial port
  channel -> print( (const char*)"\b \b" );

}

void Shellminator::redrawLine(){

  // -- Note --
  //
  // Even if formatting is disabled, this function requires VT100 commands.
  // Please not use it if you dont't want any formatting.

  // General counter variable
  #ifdef COMMANDER_API_VERSION

  uint32_t i;

  #endif

  int32_t j = -1;

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  if( inSearch ){

    redrawHistorySearch();
    return;

  }

  #endif


  if( cmd_buff_cntr > SHELLMINATOR_BUFF_LEN ){

    cmd_buff_cntr = SHELLMINATOR_BUFF_LEN;

  }

  // Terminate the command at the cmd_buff_cntr
  // to not print out the previous command's data.
  cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  acceleratorBufferPtr = acceleratorBuffer;

  if( enableFormatting ){

    acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r\033[1;32m%s\033[1;37m:\033[1;34m%s\033[0;37m \033[0K", banner, bannerPath );

  }

  else{

    acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r%s:%s \033[0K", banner, bannerPath );

  }


  #else

  // Return to the beginning of the line and print the banner
  // then the command buffer will be printed (with colors)
  channel -> print( '\r' );

  /*
  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( lastBannerSize );
  channel -> print( 'C' );
  */

  printBanner();

  channel -> write( 27 );
  channel -> print( "[0K" );

  #endif

  #ifdef COMMANDER_API_VERSION

  // If the command is found in Commander's API-tree
  // it will be highlighted.
  if( commandFound ){

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    setTerminalCharacterColor( acceleratorBufferPtr, BOLD, GREEN );
    acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );

    #else


    setTerminalCharacterColor( BOLD, GREEN );


    #endif

    for( i = 0; i < cmd_buff_cntr; i++ ){

      // If a space character is found, we have to change
      // back the color to white for the arguments.
      if( cmd_buff[ 0 ][ i ] == ' ' ){

        j = i;
        cmd_buff[ 0 ][ i ] = '\0';
        break;

      }

    }

  }

  else{

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    setTerminalCharacterColor( acceleratorBufferPtr, REGULAR, WHITE );
    acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );

    #else

    setTerminalCharacterColor( REGULAR, WHITE );

    #endif

  }

  #endif

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "%s", (char*) &cmd_buff[ 0 ] );

  #else

  channel -> print( (char*) &cmd_buff[ 0 ] );

  #endif

  if( ( j >= 0 ) ){

    cmd_buff[ 0 ][ j ] = ' ';

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    setTerminalCharacterColor( acceleratorBufferPtr, REGULAR, WHITE );
    acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );
    acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "%s", (char*) &cmd_buff[ 0 ][ j ] );

    #else

    setTerminalCharacterColor( REGULAR, WHITE );
    channel -> print( (char*) &cmd_buff[ 0 ][ j ] );

    #endif

  }

  // After all the buffer is out, we can "kill" the rest of the line
  // (clear the line from cursor to the end)
  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\033[0K" );

  #else

  channel -> write( 27 );
  channel -> print( "[0K" );

  #endif



  if( cmd_buff_cntr > cursor ){


    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\033[%dD", uint8_t( cmd_buff_cntr - cursor ) );

    #else

    channel -> write( 27 );    // ESC character( decimal 27 )
    channel -> print( '[' );  // VT100 Cursor command.
    channel -> print( uint8_t( cmd_buff_cntr - cursor ) );  // Step cursor
    channel -> print( 'D' );  // Left.

    #endif

  }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  setTerminalCharacterColor( acceleratorBufferPtr, REGULAR, WHITE );
  acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );
  channel -> print( acceleratorBuffer );

  #else

  setTerminalCharacterColor( REGULAR, WHITE );

  #endif

}

void Shellminator::process( char new_char ) {

  ((*this).*currentState)(new_char);

}

void Shellminator::overrideUpArrow( void( *func )( void ) ){

  upArrowOverrideFunc = func;

}

void Shellminator::overrideDownArrow( void( *func )( void ) ){

  downArrowOverrideFunc = func;

}

void Shellminator::overrideLeftArrow( void( *func )( void ) ){

  leftArrowOverrideFunc = func;

}

void Shellminator::overrideRightArrow( void( *func )( void ) ){

  rightArrowOverrideFunc = func;

}

void Shellminator::overrideAbortKey( void( *func )( void ) ){

  abortKeyFunc = func;

}

void Shellminator::overridePageUpKey( void( *func )( void ) ){

  pageUpKeyFunc = func;

}

void Shellminator::overridePageDownKey( void( *func )( void ) ){

  pageDownKeyFunc = func;

}

void Shellminator::overrideHomeKey( void( *func )( void ) ){

  homeKeyFunc = func;

}

void Shellminator::overrideEndKey( void( *func )( void ) ){

  endKeyFunc = func;

}

void Shellminator::overrideLogoutKey( void( *func )( void ) ){

  logoutKeyFunc = func;

}

void Shellminator::overrideSearchKey( void( *func )( void ) ){

  logoutKeyFunc = func;

}

void Shellminator::freeUpArrow(){

  upArrowOverrideFunc = NULL;

}

void Shellminator::freeDownArrow(){

  downArrowOverrideFunc = NULL;

}

void Shellminator::freeLeftArrow(){

  leftArrowOverrideFunc = NULL;

}

void Shellminator::freeRightArrow(){

  rightArrowOverrideFunc = NULL;

}

void Shellminator::freeAbortKey(){

  abortKeyFunc = NULL;

}

void Shellminator::freePageUpKey(){

  pageUpKeyFunc = NULL;

}

void Shellminator::freePageDownKey(){

  pageDownKeyFunc = NULL;

}

void Shellminator::freeHomeKey(){

  homeKeyFunc = NULL;

}

void Shellminator::freeEndKey(){

  endKeyFunc = NULL;

}

void Shellminator::freeLogoutKey(){

  logoutKeyFunc = NULL;

}

void Shellminator::freeSearchKey(){

  searchKeyFunc = NULL;

}

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

void Shellminator::clientDisconnect(){

  if( clientConnected && client.connected() ){

    client.println( "Logout!" );
    client.stop();

  }

}

#endif

void Shellminator::update() {

  // This variable will hold the character that was read from the channel buffer.
  char c;

  #ifdef SHELLMINATOR_USE_WIFI_CLIENT

  if( server ){

    if( server -> hasClient() ){

      // If we are alredy connected, we have to reject the new connection.
      if( CLIENT_STATE ){

        // Connection reject event!
        server -> available().stop();

      }

      else{

        // New connection event!
        client = server -> available();
        client.setNoDelay(false);
        client.setTimeout( 1000 );
        clientConnected = true;

        client.write( TELNET_IAC_DONT_LINEMODE, 3 );
        client.write( TELNET_IAC_WILL_ECHO, 3 );
        client.write( TELNET_IAC_DONT_ECHO, 3 );
        client.write( TELNET_IAC_WILL_SUPRESS_GO_AHEAD, 3 );
        client.write( TELNET_IAC_DO_SUPRESS_GO_AHEAD, 3 );

        // Initialise the wifiChannel as communication channel
        // to draw the logo and the banner.
        channel = &client;

        // Set the terminal color and style to the defined settings for the logo
        setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

        drawLogo();

        printBanner();

      }

    }

    // Check for disconnection
    if( clientConnected && !CLIENT_STATE ){

      // Client distonnect event!

      // The TX and RX buffers has to be cleared.
      // I did not noticed it on the ESP32 but it was
      // visible with ESP8266 at new connection.
      client.flush();
      delay( 100 );
      while( client.available() ){
        client.read();
      }

      client.stop();
      clientConnected = false;

    }

    // If connected, we have to process the Telnet commands.
    if( clientConnected && CLIENT_STATE ){

      // Check for availabla data.
      if( client.available() ){

        // Telnet command state machine.
        switch( telnetNegotiationState ){

          // In case 0 we have to check the next element in the buffer.
          // If it 0xFF, that means, we have an ongoing Telnet command,
          // so we have to parse it in the next state( state 1 ). Any other
          // situations we select the internal WiFiClient as channel to
          // let Shellminator process the data.
          case 0:

            if( client.peek() == 0xFF ){

              // Read the data to remove it from the buffer.
              client.read();

              // Switch to the next state.
              telnetNegotiationState = 1;

              // Set the communication channel to the default one,
              // to not do anything in the next section.
              channel = &defaultChannel;

            }

            else{

              // Initialise the client as communication channel.
              channel = &client;

            }

            break;

          case 1:

            // This byte is the Telnet command.
            // Right now we don't do anything
            // with it, but we have to read it,
            // to remove it from the buffer.
            client.read();

            // Switch to the next state.
            telnetNegotiationState = 2;

            // Set the communication channel to the default one,
            // to not do anything in the next section.
            channel = &defaultChannel;

            break;

          case 2:

            // This byte is the Telnet option.
            // Right now we don't do anything
            // with it, but we have to read it,
            // to remove it from the buffer.
            client.read();

            // Switch to the default state( state 0 ).
            telnetNegotiationState = 0;

            // Set the communication channel to the default one,
            // to not do anything in the next section.
            channel = &defaultChannel;

            break;

          default:
            // Something went wrong, we should not be here.
            // Switch to the default state( state 0 ).
            telnetNegotiationState = 0;
            break;

        }

      }

    }

    // Else set the default channel. The default channel's
    // available function will return 0, so the next part of
    // the update function won't do anything( as should ).
    else{

      channel = &defaultChannel;

    }

  }

  #endif

  // We have to check the channel buffer. If it is not empty we can read as many characters as possible.
  while ( channel -> available() ) {

    // Read one character from channel Buffer.
    c = (char)channel -> read();

    // Process the new character.
    process( c );

    #ifdef COMMANDER_API_VERSION
    commandCheckTimerStart = millis();
    commandChecked = false;
    #endif

  }

  #ifdef COMMANDER_API_VERSION

  // Command highlight section.
  Commander::API_t *commandAddress;

  // If Commander is available and we did not checked the
  // typed command, we are trying to find and highlight it.
  if( commander && !commandChecked ){
    bool previousCommandFound = commandFound; // hold the previos flag

    // We have to wait 100ms after the last keypress.
    if( ( millis() - commandCheckTimerStart ) > 100 ){

      // Generic counter variable.
      uint32_t i = 0;

      // Commander expects a null terminated string, so we
      // have to terminate the string at the end, or at
      // space character. But after the search we have to
      // store bactk this character to it's original state.
      char charCopy = 0; // initialize the variable

      // Find the end of the input command, or the first space
      // character in it, store it's value to charCopy, and
      // replace it with null character.
      for( i = 0; i <= cmd_buff_cntr; i++ ){

        if( ( cmd_buff[ 0 ][ i ] == ' '  ) || ( i == cmd_buff_cntr ) ){

          charCopy = cmd_buff[ 0 ][ i ];
          cmd_buff[ 0 ][ i ] = '\0';
          break;

        }

      }

      // Try to find the command in Commander's API-tree.
      commandAddress = commander -> operator[]( cmd_buff[0] );

      // If Commander responds with a non-null pointer, it means
      // that we have a mach.
      if( commandAddress ){
        commandFound = true;
      } else {
        commandFound = false;
      }

      // Restore the original state.
      cmd_buff[ 0 ][ i ] = charCopy;

      // Set the flag.
      commandChecked = true;

      // if the commandFound flag has changed, redraw the line
      // to get the colors right
      // If formatting disabled, we must not redraw the line.
      if( ( previousCommandFound != commandFound ) && enableFormatting) {
        redrawLine();
      }

    }

  }

  #endif

}

void Shellminator::setTerminalCharacterColor( uint8_t style, uint8_t color ) {

  if( !enableFormatting ){

    return;

  }

  // The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( style );
  channel -> print( ';' );
  channel -> print( color );
  channel -> print( 'm' );

}

void Shellminator::setTerminalCharacterColor( char* buff, uint8_t style, uint8_t color ){

  if( !enableFormatting ){

    return;

  }

  if( buff == NULL ){

    return;

  }

  sprintf( buff, "\033[%d;%dm", style, color );

}

void Shellminator::setTerminalCharacterColor( Stream *stream_p, uint8_t style, uint8_t color ){

  // The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  stream_p -> write( 27 );
  stream_p -> print( '[' );
  stream_p -> print( style );
  stream_p -> print( ';' );
  stream_p -> print( color );
  stream_p -> print( 'm' );

}

void Shellminator::hideCursor(){

  if( !enableFormatting ){

    return;

  }

  channel -> print( (const char*)"\033[?25l" );

}

void Shellminator::hideCursor( char* buff ){

  if( !enableFormatting ){

    return;

  }

  sprintf( buff, "\033[?25l" );

}

void Shellminator::hideCursor( Stream *stream_p ){

  stream_p -> print( (const char*)"\033[?25l" );

}

void Shellminator::showCursor(){

  if( !enableFormatting ){

    return;

  }

  channel -> print( (const char*)"\033[?25h" );

}

void Shellminator::showCursor( char* buff ){

  if( !enableFormatting ){

    return;

  }

  sprintf( buff, "\033[?25h" );

}

void Shellminator::showCursor( Stream *stream_p ){

  stream_p -> print( (const char*)"\033[?25h" );

}

void Shellminator::drawLogo() {

  if( logo ){

    // Set the terminal color and style to the defined settings for the logo
    setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

    // Draws the startup logo to the terminal interface.
    channel -> print( logo );

    // Set the terminal style to normal.
    setTerminalCharacterColor( REGULAR, WHITE );

  }

  #ifdef __AVR__

  else if( progmemLogo ){

    // Set the terminal color and style to the defined settings for the logo
    setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

    // Draws the startup logo to the terminal interface.
    channel -> print( progmemLogo );

    // Set the terminal style to normal.
    setTerminalCharacterColor( REGULAR, WHITE );

  }

  #endif

}

void Shellminator::beep(){

  if( !mute ){

    // Bell character.
    channel -> print( '\a' );

  }

}

#ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

void Shellminator::historySearchBackward(){

  // Store a copy of the cmd_buff_dim variable.
  // It is necessary, because it can be only changed
  // when we find a command. Other case it has to be intact.
  uint32_t cmd_buff_dim_save;

  // Create a copy of cmd_buff_dim.
  cmd_buff_dim_save = cmd_buff_dim;

  // We search upward the history until the end of the history memory, or match.
  while( cmd_buff_dim_save < ( SHELLMINATOR_BUFF_DIM ) ){

    // Check for match.
    if( strncmp( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim_save ], cursor ) == 0 ){

      // If we found a command we have to save the actual position to cmd_buff_dim.
      cmd_buff_dim = cmd_buff_dim_save;

      // Copy the found data to the 0-th element.
      strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim ], SHELLMINATOR_BUFF_LEN + 1 );

      // We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
      // It is important to track the end of the loaded string.
      // In this case the cursor does not move!
      cmd_buff_cntr = strlen( cmd_buff[ 0 ] );

      // We print the loaded command to the terminal interface.
      redrawLine();

      // Increment the buffer before return.
      // If we don't do this we can not search further.
      cmd_buff_dim++;

      // Return because we have found the command.
      return;

    }

    // Incrememnt the position to test another command in history.
    cmd_buff_dim_save++;

  }

  // We did not found a command in the history.
  // Generate a beep to notify that.
  beep();

}

void Shellminator::historySearchForward(){

  uint32_t cmd_buff_dim_save;

  cmd_buff_dim_save = cmd_buff_dim;

  while( cmd_buff_dim_save > 2 ){

    cmd_buff_dim_save--;

    if( strncmp( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim_save - 1 ], cursor ) == 0 ){

      cmd_buff_dim = cmd_buff_dim_save;

      strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim - 1 ], SHELLMINATOR_BUFF_LEN + 1 );

      cmd_buff_cntr = strlen( cmd_buff[ 0 ] );

      // We print the loaded command to the terminal interface.
      redrawLine();

      return;

    }

  }

  beep();

}

void Shellminator::redrawHistorySearch(){

  uint32_t i;
  uint32_t j;
  bool highlighted = false;
  int32_t searchResult;

  searchMatch = -1;

  if( cmd_buff_cntr > SHELLMINATOR_BUFF_LEN ){

    cmd_buff_cntr = SHELLMINATOR_BUFF_LEN;

  }

  // Terminate the command at the cmd_buff_cntr
  // to not print out the previous command's data.
  cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  acceleratorBufferPtr = acceleratorBuffer;
  // acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r\033[%dC\033[0K", lastBannerSize );
  acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r\033[0;37m(reverse-i-search)'\033[1;33m%s\033[0;37m': \033[0K", cmd_buff[ 0 ] );

  #else

  channel -> print( '\r' );
  setTerminalCharacterColor( REGULAR, WHITE );
  channel -> print( "(reverse-i-search)'" );  // 19 character
  setTerminalCharacterColor( BOLD, YELLOW );
  channel -> print( cmd_buff[ 0 ] );
  setTerminalCharacterColor( REGULAR, WHITE );
  channel -> print( "': \033[0K" );

  #endif

  if( cmd_buff_cntr == 0 ){

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    channel -> print(acceleratorBuffer);

    #endif

    return;

  }

  for( i = 1; i < SHELLMINATOR_BUFF_DIM; i++ ){

    searchResult = substring( cmd_buff[ 0 ], cmd_buff[ i ] );
    if( searchResult >= 0 ){

      /*
      Serial.print( "found: " );
      Serial.print( i );
      Serial.print( ' ' );
      Serial.print( cmd_buff[ i ] );
      Serial.print( ' ' );
      Serial.println( searchResult );
      */

      searchMatch = i;

      for( j = 0; j < strlen( cmd_buff[ i ] ); j++ ){

        if( !highlighted && ( j == searchResult ) ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          setTerminalCharacterColor( acceleratorBufferPtr, BOLD, YELLOW );
          acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );

          #else

          setTerminalCharacterColor( BOLD, YELLOW );

          #endif

          highlighted = true;

        }

        if( highlighted && ( j == ( searchResult + strlen( cmd_buff[ 0 ] ) ) ) ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          setTerminalCharacterColor( acceleratorBufferPtr, REGULAR, WHITE );
          acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );

          #else

          setTerminalCharacterColor( REGULAR, WHITE );

          #endif

          highlighted = false;

        }

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        *acceleratorBufferPtr = cmd_buff[ i ][ j ];
        acceleratorBufferPtr++;

        #else

        channel -> print( cmd_buff[ i ][ j ] );

        #endif

      }

      if( highlighted ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        setTerminalCharacterColor( acceleratorBufferPtr, REGULAR, WHITE );
        acceleratorBufferPtr = acceleratorBuffer + strlen( acceleratorBuffer );

        #else

        setTerminalCharacterColor( REGULAR, WHITE );

        #endif

      }

      #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

      acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r\033[%dC", uint8_t( 19 + cursor ) );
      channel -> print( acceleratorBuffer );

      #else

      channel -> print( '\r' );

      channel -> write( 27 );
      channel -> print( '[' );
      channel -> print( uint8_t( 19 + cursor ) );
      channel -> print( 'C' );

      #endif

      return;

    }

  }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  acceleratorBufferPtr += sprintf( acceleratorBufferPtr, "\r\033[%dC", uint8_t( 19 + cursor ) );
  channel -> print( acceleratorBuffer );

  #else

  channel -> print( '\r' );

  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( uint8_t( 19 + cursor ) );
  channel -> print( 'C' );

  #endif

}

int Shellminator::substring( char* str1, char* str2 ){

  // https://www.geeksforgeeks.org/check-string-substring-another/

  int i;
  int j;

  int m = strlen( str1 );
  int n = strlen( str2 );

  for( i = 0; i <= ( n - m ); i++ ){

    for( j = 0; j < m; j++ ){

      if( str2[ i + j ] != str1[ j ] ){
        break;
      }

    }

    if( j == m ){

      return i;

    }

  }

  return -1;

}

#endif

#ifdef COMMANDER_API_VERSION

void Shellminator::attachCommander( Commander* commander_p ){

  commander = commander_p;

}

#endif

#ifdef SHELLMINATOR_ENABLE_PASSWORD_MODULE

void Shellminator::enablePasswordProtection( uint8_t* passwordHashAddress_p ){

  passwordHashAddress = passwordHashAddress_p;

}

void Shellminator::enablePasswordProtection( const uint8_t* passwordHashAddress_p ){

  passwordHashAddress = (uint8_t*)passwordHashAddress_p;

}

void Shellminator::enablePasswordProtection( char* passwordHashAddress_p ){

  passwordHashAddress = (uint8_t*)passwordHashAddress_p;

}

void Shellminator::enablePasswordProtection( const char* passwordHashAddress_p ){

  passwordHashAddress = (uint8_t*)passwordHashAddress_p;

}

void Shellminator::disablePasswordProtection(){

  passwordHashAddress = NULL;

}

bool Shellminator::checkPassword( uint8_t* pwStr ){

  terminal_sha256_init( &passwordHashCtx );
  terminal_sha256_update( &passwordHashCtx, pwStr, strlen( (const char*)pwStr ) );
  terminal_sha256_final( &passwordHashCtx, passwordHashBuffer );

  return memcmp( passwordHashAddress, passwordHashBuffer, SHA256_BLOCK_SIZE );

}

bool Shellminator::checkPassword( const uint8_t* pwStr ){

  checkPassword( (uint8_t*)pwStr );

}

bool Shellminator::checkPassword( char* pwStr ){

  checkPassword( (uint8_t*)pwStr );

}

bool Shellminator::checkPassword( const char* pwStr ){

  checkPassword( (uint8_t*)pwStr );

}

#endif


//----- QR-code generator part -----//
#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

void Shellminator::generateQRText( char* text, enum qrcodegen_Ecc ecc ){

  // The result of the QR-code generation will be stored in this variable.
  bool result;

  // The actual size of the QR-code will be stored in this variable.
  uint32_t qr_size;

  // This variable helps to track the x direction while drawing the QR code.
  uint32_t x;

  // This variable helps to track the y direction while drawing the QR code.
  uint32_t y;

  // This variable will store the upper pixel while drawing.
  bool upper_pixel;

  // This variable will store the lower pixel while drawing.
  bool lower_pixel;

  // Generate the QR-code with default settings, and store the result to
  // the result variable.
  result = qrcodegen_encodeText(  text,
                                  qr_tempBuff,
                                  qr_data,
                                  ecc,
                                  qrcodegen_VERSION_MIN,
                                  qrcodegen_VERSION_MAX,
                                  qrcodegen_Mask_AUTO,
                                  true
                                );

  // We have to check the result variable. If it is true,
  // the QR-code generation went well.
  if( !result ){

    return;

  }

  // Determinate the size of the QR-code.
  qr_size = qrcodegen_getSize( qr_data );

  // The unicode character table does not have a rectangular square,
  // but it has a half rectangular square on, top, bottom, and a full bar.
  // Check these links to make it more clear:
  //  -Full bar:      https://www.fileformat.info/info/unicode/char/2588/index.htm
  //  -Upper square:  https://www.fileformat.info/info/unicode/char/2580/index.htm
  //  -lower square:  https://www.fileformat.info/info/unicode/char/2584/index.htm
  // To draw a QR-code with a terminal emulator the easiest way to combine these
  // unicode characters. Because it is two 'pixels' high, we have to step the y
  // variable by two lines.
  for( y = 0; y < ( qr_size / 2 ); y++ ){

    // Print a new line at the begining of the horizontal drawing.
    channel -> print( "\r\n" );

    // Draw all horizontal 'pixels'.
    for( x = 0; x < qr_size; x++ ){

      // Determinate the upper pixel value.
      upper_pixel = qrcodegen_getModule( qr_data, x, ( y * 2 ) );

      // Determinate the lower pixel value.
      lower_pixel = qrcodegen_getModule( qr_data, x, ( ( y * 2 ) + 1 ) );

      // Draw the right pattern accordingly.

      // Both pixels are black.
      if( upper_pixel && lower_pixel ){

        channel -> print( "\u2588" );
        continue;

      }

      // Upper pixel is black.
      if( upper_pixel && ( !lower_pixel ) ){

        channel -> print( "\u2580" );
        continue;

      }

      // Lower pixel is black.
      if( ( !upper_pixel ) && lower_pixel ){

        channel -> print( "\u2584" );
        continue;

      }

      // If we get here we have to draw an empty bar.
      // The space character will do the job.
      channel -> print( " " );

    }

  }

  // If the QR code size is not even, we have to draw
  // the last line as well.
  if( ( qr_size % 2 ) != 0 ){

    // Print a new line at the begining of the horizontal drawing.
    channel -> print( "\r\n" );

    // Draw all horizontal 'pixels'.
    for( x = 0; x < qr_size; x++ ){


      // Determinate the pixel value. We store it to upper_pixel variable.
      upper_pixel = qrcodegen_getModule( qr_data, x, ( qr_size - 1 ) );

      // Check if we have to draw.
      if( upper_pixel ){

        channel -> print( "\u2580" );
        continue;

      }

      // If we get here we have to draw an empty bar.
      // The space character will do the job.
      channel -> print( " " );

    }

  }

  // Finally create a new line.
  channel -> print( "\r\n" );

}

void Shellminator::generateQRText( const char* text, enum qrcodegen_Ecc ecc ){

  generateQRText( (char*)text, ecc );

}

void Shellminator::generateQRText( const char* text ){

  generateQRText( (char*)text, qrcodegen_Ecc_MEDIUM );

}

void Shellminator::generateQRText( char* text ){

  generateQRText( text, qrcodegen_Ecc_MEDIUM );

}

#endif

void Shellminator::ShellminatorDefaultState( char new_char ){

  switch( new_char ){

    case '\0':
      break;

    case '\n':
      break;

    case '\b':
      ShellminatorBackspaceState();
      break;

    case 127:
      ShellminatorBackspaceState();
      break;

    case '\r':
      ShellminatorEnterKeyState();
      break;

    case 0x01:
      ShellminatorBeginningOfLineState();
      break;

    case 0x05:
      ShellminatorEndOfLineState();
      break;

    case 0x04:
      ShellminatorLogoutState();
      break;

    case 0x12:
      ShellminatorReverseSearchState();
      break;

    case 0x0C:
      ShellminatorClearScreenState();
      break;

    case '\t':
      ShellminatorAutoCompleteState();
      break;

    case 0x03:
      ShellminatorAbortState();
      break;

    case 27:
      currentState = &Shellminator::ShellminatorEscapeCharacterState;
      break;

    default:
      ShellminatorProcessRegularCharacter( new_char );
      currentState = &Shellminator::ShellminatorDefaultState;
      break;

  }

}

void Shellminator::ShellminatorBackspaceState(){

  // General counter variable
  uint32_t i;

  // If we press a backspace we have to reset cmd_buff_dim to default value
  cmd_buff_dim = 1;

  // We have to check the number of the characters in the buffer.
  // If the buffer is empty we must not do anything!
  if ( cursor > 0 ) {

    // decrease the cmd buffer counter and the cursor position
    cmd_buff_cntr--;
    cursor--;

    // if we are at the end of the command buffer
    if ( cursor == cmd_buff_cntr ) {

      #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE
      if( inSearch ){

        cmd_buff[ 0 ][ cursor + 1 ] = '\0'; // and from the cmd buffer
        redrawLine();

      }

      else{
      #endif

      channel -> print("\b \b"); // just delete the last character from the terminal
      cmd_buff[ 0 ][ cursor + 1 ] = '\0'; // and from the cmd buffer

      #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE
      }
      #endif


    }

    else {

      // if the cursor is somewhere in the middle of the cmd buffer
      // rework the buffer and redraw the whole line
      for( i = cursor; i < cmd_buff_cntr; i++ ) {

        cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];

      }

      redrawLine();

    }

  }

}

void Shellminator::ShellminatorEnterKeyState(){

  // If the enter key is pressed cmd_buff_dim has to be reset to the default value
  cmd_buff_dim = 1;

  // General counter variable
  uint32_t i;

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  if( inSearch  ){

    if( searchMatch > 0 ){

      inSearch = false;
      strncpy( cmd_buff[ 0 ], cmd_buff[ searchMatch ], SHELLMINATOR_BUFF_LEN + 1 );
      cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
      redrawLine();

    }

    else{
      cmd_buff_cntr = 0;
    }

  }

  #endif

  // Because a command is sent we have to close it. Basically we replace the arrived
  // '\r' character with a '\0' string terminator character. Now we have our command
  // in a C/C++ like standard string format.
  cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

  // We send a line break to the terminal to put the next data in new line
  channel -> print( '\r' );
  channel -> print( '\n' );

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  inSearch = false;

  #endif

  // If the arrived data is not just a single enter we have to process the command.
  if ( cmd_buff_cntr > 0 ) {

    // Flush out everyting before command execution.
    // It is required for the prompts, because sometimes garbage
    // can be found in the channel stream. It has to be removed before
    // prompt reading.
    while( channel -> available() ){
      channel -> read();
    }

    if( ( strcmp( cmd_buff[ 0 ], "help" ) == 0 ) || ( strcmp( cmd_buff[ 0 ], "?" ) == 0 ) ){

      printHelp();

    }

    else if( strcmp( cmd_buff[ 0 ], "history" ) == 0 ){

      printHistory();

    }

    // We haveto check that execution_fn_with_parrent is not NULL.
    else if( execution_fn_with_parrent != NULL ){

      // If it is a valid, then call it's function.
      execution_fn_with_parrent( cmd_buff[ 0 ], this );

    }

    // We haveto check that execution_fn is not NULL.
    else if( execution_fn != NULL ){

      // If it is a valid, then call it's function.
      execution_fn( cmd_buff[ 0 ] );

    }

    #ifdef COMMANDER_API_VERSION

    // If a Commander object is added, it can be used
    // to execute the command without an execution_fn.
    else if( commander != NULL ){

      commander -> execute( cmd_buff[ 0 ], channel, this );

    }

    #endif

    // If not, then just print it with Serial.
    else{
      channel -> print( (const char*)"cmd: " );
      channel -> print( cmd_buff[ 0 ] );
    }

    // Send a new line after command execution,
    // so we will not overwrite the last line of the
    // command output with the banner
    // --- It maybe not necessary ---
    //channel -> print( '\r' );
    //channel -> print( '\n' );

    // After we processed the command we have to shift the history upwards.
    // To protect the copy against buffer overflow we use strncpy
    for ( i = ( SHELLMINATOR_BUFF_DIM - 1 ); i > 0; i-- ) {

      strncpy( cmd_buff[ i ], cmd_buff[ i - 1 ], SHELLMINATOR_BUFF_LEN + 1 );

    }

  }

  // After the command processing finished we print a new banner to the terminal.
  // This means that the device is finished and waits the new command.
  printBanner();

  // To empty the incoming string we have to zero it's counter.
  cmd_buff_cntr = 0;
  cursor = 0;

}

void Shellminator::ShellminatorBeginningOfLineState(){

  cursor = 0;
  redrawLine();

}

void Shellminator::ShellminatorEndOfLineState(){

  cursor = cmd_buff_cntr;
  redrawLine();

}

void Shellminator::ShellminatorLogoutState(){

  if( logoutKeyFunc ){

    logoutKeyFunc();
    return;

  }

  #ifdef SHELLMINATOR_USE_WIFI_CLIENT

  clientDisconnect();

  #endif

  #ifdef SHELLMINATOR_ENABLE_WEBSOCKET_MODULE

  websocketDisconnect();

  #endif

}

void Shellminator::ShellminatorReverseSearchState(){

  if( searchKeyFunc ){

    searchKeyFunc();
    return;

  }

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  inSearch = !inSearch;
  redrawLine();

  #endif

}

void Shellminator::ShellminatorClearScreenState(){

  // Only works, if the terminal interface can handle formatting.
  if( enableFormatting ){

    clear();
    redrawLine();

  }

}

void Shellminator::ShellminatorAutoCompleteState(){

  // Auto complete section.
  #ifdef COMMANDER_API_VERSION

  // General counter variable
  uint32_t i;

  // Firstly, we have to set the cursor to the end of the input command.
  // If the algorythm fills the missing characters, they have to placed
  // at the end.
  cursor = cmd_buff_cntr;

  // Pointer to a Commander API-tree element.
  Commander::API_t *commandAddress;

  // The next auto filled character will be placed in this variable.
  char nextChar;

  // This flag holds an auto complete conflict event.
  // Conflict event happens:
  // - after the first character of mismatch( restart, reboot will trigger conflict at the third character )
  // - if cmd_buff_cntr would overflow Commanders command tree.
  // - if we found the end of the last command.
  bool conflict = false;

  // PROGMEM based tree is not supported for auto complete yet.
  if( commander -> memoryType != Commander::MEMORY_REGULAR ){
    return;
  }

  // If there is no conflict event, we are trying
  // to fill as many characters as possible.
  while( !conflict ){

    // Reset the counter to the first Commander API-tree element.
    i = 0;

    // Get the address of the element indexed by i.
    // If the indexed elment does not exists, Commander
    // will return NULL.
    commandAddress = commander -> operator[]( (int)i );

    // Set to default state.
    nextChar = '\0';

    // Go through all elements in Commanders API-tree.
    while( commandAddress ){

      // We have to check that the typed command is exists within an existing command.
      if( strncmp( (const char*)cmd_buff[ 0 ], commandAddress -> name, cmd_buff_cntr ) == 0 ){

        // If it does, we have to check for conflict.
        if( ( nextChar == '\0' ) && ( cmd_buff_cntr < COMMANDER_MAX_COMMAND_SIZE ) && ( commandAddress -> name[ cmd_buff_cntr ] != '\0' ) ){

          // If there is no conflict we can set the next character from the command that we found.
          nextChar = commandAddress -> name[ cmd_buff_cntr ];

        }

        else{

          // We have to check that the next character in the command
          // tree is not the same as the value in nextChar.
          if( commandAddress -> name[ cmd_buff_cntr ] != nextChar ){

            // Trigger conflict.
            conflict = true;

          }

        }

      }

      // Increment i to get the next command's index.
      i++;

      // Get the address of the element indexed by i.
      commandAddress = commander -> operator[]( (int)i );

    }

    // If nextChar does not changed since start, that means
    // we did not found anything similar.
    if( nextChar == '\0' ){

      // We have to trigger conflict to abort the process.
      conflict = true;

    }

    // If we does not had a conflict event, we have to process
    // the foind character as a regular character.
    if( !conflict ){

      process( nextChar );

    }

  }

  #endif

}

void Shellminator::ShellminatorAbortState(){

  if( abortKeyFunc ){

    abortKeyFunc();

  }

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  inSearch = false;

  #endif

  // If the abort key is pressed cmd_buff_dim has to be reset to the default value
  cmd_buff_dim = 1;

  // We send a line break to the terminal to put the next data in new line
  channel -> print( '\r' );
  channel -> print( '\n' );

  printBanner();

  cursor = 0;
  cmd_buff_cntr = 0;

}

void Shellminator::ShellminatorEscapeCharacterState( char new_char ){

  if( new_char == '[' ){

    currentState = &Shellminator::ShellminatorEscapeBracketState;

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorEscapeBracketState( char new_char ){

  switch( new_char ){

    case 'A':
      ShellminatorUpArrowKeyState();
      break;

    case 'B':
      ShellminatorDownArrowKeyState();
      break;

    case 'D':
      ShellminatorLeftArrowKeyState();
      break;

    case 'C':
      ShellminatorRightArrowKeyState();
      break;

    case 'H':
      ShellminatorHomeKeyState();
      break;

    case '1':
      currentState = &Shellminator::ShellminatorHomeKeyState;
      break;

    case 'F':
      ShellminatorEndKeyState();
      break;

    case '4':
      currentState = &Shellminator::ShellminatorEndKeyState;
      break;

    case '3':
      currentState = &Shellminator::ShellminatorDelKeyState;
      break;

    case '5':
      currentState = &Shellminator::ShellminatorPageUpKeyState;
      break;

    case '6':
      currentState = &Shellminator::ShellminatorPageDownKeyState;
      break;

    default:
      currentState = &Shellminator::ShellminatorDefaultState;
      break;

  }

}

void Shellminator::ShellminatorUpArrowKeyState(){

  // Because we have finished the escape sequence interpretation we reset the state-machine.
  currentState = &Shellminator::ShellminatorDefaultState;

  // Check if the arrow function is overriden.
  if( upArrowOverrideFunc ){

    upArrowOverrideFunc();
    return;

  }

  // We have to check that we can go upper in history
  if ( cmd_buff_dim < ( SHELLMINATOR_BUFF_DIM ) ) {

    // If we can, we have to check that the previous command was not empty.
    if ( cmd_buff[ cmd_buff_dim ][0] == '\0' ) {

      // If it was empty we can't do much with an empty command so we return.
      return;

    }

    // Now we have to copy the characters form the histoy to the 0th element in the buffer.
    // Remember the 0th element is always reserved for the new data. If we browse the history the
    // data in the history will overwrite the data in the 0th element so the historical data will be
    // the new data. We use strncpy to prevent overflow.
    strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim ], SHELLMINATOR_BUFF_LEN + 1 );

    // We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
    // It is important to track the end of the loaded string.
    cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
    cursor = cmd_buff_cntr;

    // We print the loaded command to the terminal interface.
    //channel -> print( cmd_buff[ 0 ] );

    redrawLine();

    // We have to increment the cmd_buff_dim variable, to track the history position.
    // Greater number means older command!
    cmd_buff_dim++;

  }

}

void Shellminator::ShellminatorDownArrowKeyState(){

  // Because we have finished the escape sequence interpretation we reset the state-machine.
  currentState = &Shellminator::ShellminatorDefaultState;

  // Check if the arrow function is overriden.
  if( downArrowOverrideFunc ){

    downArrowOverrideFunc();
    return;

  }


  // We have to check that we can go lover in history, and we are not in the first previous command.
  if ( cmd_buff_dim > 2 ) {

    // We have to decrement the cmd_buff_dim variable, to track the history position.
    // Lower number means newer command!
    cmd_buff_dim--;

    // Now we have to copy the characters form the histoy to the 0th element in the buffer.
    // Remember the 0th element is always reserved for the new data. If we browse the history the
    // data in the history will overwrite the data in the 0th element so the historical data will be
    // the new data. We use strncpy to prevent overflow.
    strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim - 1  ], SHELLMINATOR_BUFF_LEN + 1 );

    // We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
    // It is important to track the end of the loaded string.
    cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
    cursor = cmd_buff_cntr;

    // We print the loaded command to the terminal interface.
    //channel -> print( cmd_buff[ 0 ] );
    redrawLine();

  }

  // Check that if we are in the first previous command.
  else if ( cmd_buff_dim == 2 ) {

    // To empty the incoming string we have to zero it's counter.
    cmd_buff_cntr = 0;
    cursor = 0;

    // We have to reset the cmd_buff_dim variable to the default value.
    cmd_buff_dim = 1;

    redrawLine();

  }

  // We have finished so we can break from the switch.

}

void Shellminator::ShellminatorLeftArrowKeyState(){

  // We just simply reset the state-machine.
  currentState = &Shellminator::ShellminatorDefaultState;

  // Check if the arrow function is overriden.
  if( leftArrowOverrideFunc ){

    leftArrowOverrideFunc();
    return;

  }

  // Check if we can move to left.
  if( cursor > 0 ){

    channel -> write( 27 );   // ESC character( decimal 27 )
    channel -> print( '[' );  // VT100 Cursor command.
    channel -> print( '1' );  // 1 character movement.
    channel -> print( 'D' );  // Left.

    // Decrement the cursor variable.
    cursor--;

  }

}

void Shellminator::ShellminatorRightArrowKeyState(){

  // We just simply reset the state-machine.
  currentState = &Shellminator::ShellminatorDefaultState;

  // Check if the arrow function is overriden.
  if( rightArrowOverrideFunc ){

    rightArrowOverrideFunc();
    return;

  }

  // Check if we can move to right.
  if( cursor < cmd_buff_cntr ){

    channel -> write( 27 );   // ESC character( decimal 27 )
    channel -> print( '[' );  // VT100 Cursor command.
    channel -> print( '1' );  // 1 character movement.
    channel -> print( 'C' );  // Left.

    // Increment the cursor variavble.
    cursor++;

  }

}

void Shellminator::ShellminatorHomeKeyState(){

  currentState = &Shellminator::ShellminatorDefaultState;

  if( homeKeyFunc ){

    homeKeyFunc();
    return;

  }

  // send the cursor to the begining of the buffer
  cursor = 0;
  redrawLine();

}

void Shellminator::ShellminatorHomeKeyState( char new_char ){

  if( new_char == '~' ){

    ShellminatorHomeKeyState();

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorEndKeyState(){

  currentState = &Shellminator::ShellminatorDefaultState;

  if( endKeyFunc ){

    endKeyFunc();
    return;

  }

  // send the cursor to the end of the buffer
  cursor = cmd_buff_cntr;
  redrawLine();

}

void Shellminator::ShellminatorEndKeyState( char new_char ){

  if( new_char == '~' ){

    ShellminatorEndKeyState();

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorDelKeyState(){

  // Del key detected.
  // If we press a delet key we have to reset cmd_buff_dim to default value
  cmd_buff_dim = 1;

  // General counter variable
  uint32_t i;

  currentState = &Shellminator::ShellminatorDefaultState;

  // We have to check the number of the characters in the buffer.
  // If the buffer is full we must not do anything!
  if ( cursor != cmd_buff_cntr ) {

    for( i = cursor; i < ( cmd_buff_cntr - 1 ); i++ ){

      cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];

    }

    // If there is at least 1 character in the buffer we jus simply
    // decrement the cmd_buff_cntr. This will result that the new character
    // will be stored in the previous characters place in the buffer.
    cmd_buff_cntr--;

    redrawLine();

  }

}

void Shellminator::ShellminatorDelKeyState( char new_char ){

  if( new_char == '~' ){

    ShellminatorDelKeyState();

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorPageUpKeyState(){

  currentState = &Shellminator::ShellminatorDefaultState;

  if( pageUpKeyFunc ){

    pageUpKeyFunc();
    return;

  }

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  historySearchBackward();

  #endif

}

void Shellminator::ShellminatorPageUpKeyState( char new_char ){

  if( new_char == '~' ){

    ShellminatorPageUpKeyState();

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorPageDownKeyState(){

  currentState = &Shellminator::ShellminatorDefaultState;

  if( pageDownKeyFunc ){

    pageDownKeyFunc();
    return;

  }

  #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

  historySearchForward();

  #endif

}

void Shellminator::ShellminatorPageDownKeyState( char new_char ){

  if( new_char == '~' ){

    ShellminatorPageDownKeyState();

  }

  else{

    currentState = &Shellminator::ShellminatorDefaultState;

  }

}

void Shellminator::ShellminatorProcessRegularCharacter( char new_char ){

  // General counter variable
  uint32_t i;

  // If the cursor is at the end of the command,
  // we simply store the new character.
  if( cursor == cmd_buff_cntr ){

    cmd_buff[ 0 ][ cmd_buff_cntr ] = new_char;

  }

  // If the cursor is somewhere in the middle, we have to shift the
  // end of the command by one character end insert the new character
  // to the cursor position.
  else{

    for( i = cmd_buff_cntr; i > cursor; i-- ){

      cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i - 1 ];

    }

    // Add the new character after the cursor in the buffer
    cmd_buff[ 0 ][ cursor ] = new_char;

  }

  // In this case we have to reset the cmd_buff_dim variable to the default value.
  cmd_buff_dim = 1;

  // If the cursor was at the end we have to print the
  // new character if the cmd_buff had free space at
  // the end.
  if ( cursor == cmd_buff_cntr ) {

    if ( cmd_buff_cntr < SHELLMINATOR_BUFF_LEN ) {

      #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

      if( inSearch ){

        // Increment counters.
        cmd_buff_cntr++;
        cursor++;

        redrawLine();

        // Increment counters.
        cmd_buff_cntr--;
        cursor--;

      }

      else{
      #endif

      channel -> print(new_char);

      #ifdef SHELLMINATOR_ENABLE_SEARCH_MODULE

      }

      #endif

    }

  }

  // Increment counters.
  cmd_buff_cntr++;
  cursor++;

  if ( cursor != cmd_buff_cntr ) {

    // Redraw the command line.
    redrawLine();

  }

  // Check if the counters are overloaded.
  // the buffer storage is SHELLMINATOR_BUFF_LEN + 2,
  // so it is safe to make the counters equeal to SHELLMINATOR_BUFF_LEN
  if( cmd_buff_cntr > SHELLMINATOR_BUFF_LEN ) {

    cmd_buff_cntr = SHELLMINATOR_BUFF_LEN;

  }

  if( cursor > SHELLMINATOR_BUFF_LEN ) {

    cursor = SHELLMINATOR_BUFF_LEN;

  }

}

bool Shellminator::waitForKey( Stream* source, char key, uint32_t timeout ){

  // Save system time
  unsigned long timerStart = millis();

  while( source -> available() < 1 ){
    // Wait for key input.

    // Check if timeout is specified.
    if( timeout == 0 ){
      continue;
    }

    // Timeout detection.
    if( ( millis() - timerStart ) > timeout ){

      return false;

    }

  }

  // Read the pressed key
  char c = source -> read();

  // Compare with the expected key
  if( c == key ){

    return true;

  }

  return false;

}

bool Shellminator::waitForKey( Stream* source, char* keys, uint32_t timeout ){

  // Generic counter variable.
  uint32_t i;

  // Save system time.
  unsigned long timerStart = millis();

  while( source -> available() < 1 ){
    // Wait for key input.

    // Check if timeout is specified.
    if( timeout == 0 ){
      continue;
    }

    // Timeout detection.
    if( ( millis() - timerStart ) > timeout ){

      return false;

    }

  }

  // Read the pressed key
  char c = source -> read();

  // Check all characters in keys array.
  for( i = 0; i < strlen( keys ); i++ ){

    // If there is a match, return with true.
    if( keys[ i ] == c ){

      return true;

    }

  }

  // No match, return false;
  return false;

}

int Shellminator::input( Stream* source, int bufferSize, char* buffer, char* lineText, uint32_t timeout, bool secret ){

  // Return value.
  int retVal = 0;

  // Tracks the index of the next free slot in the buffer.
  uint32_t bufferCounter = 0;

  // Save system time.
  unsigned long timerStart = millis();

  // It will hold the new character.
  char c;

  // Print the prompt text.
  source -> print( lineText );

  // If no timeout event, or no timeout specified, check for new data.
  while( ( ( millis() - timerStart ) < timeout ) || timeout == 0 ){

    // Check for new data.
    if( source -> available() ){

      // Read new data.
      c = source -> read();

      // Check for return key.
      if( ( c == '\n' ) || ( c == '\r' ) || ( c == '\0' ) ){

        // Terminate the buffer.
        buffer[ bufferCounter ] = '\0';

        // print new line.
        source -> println();

        // Empty the input stream.
        while( source -> available() ){
          source -> read();
        }

        // Return how many characters have been read.
        return retVal;

      }

      // Check backspace.
      else if( ( c == '\b' ) || ( c == 127 ) ){

        // Check if the buffer is not empty.
        if( bufferCounter > 0 ){

          // Set back the cursor.
          bufferCounter--;
          retVal--;
          source -> print( "\b \b" );

        }

      }

      else{

        // Check if there is empty space in the buffer.
        if( bufferCounter < ( bufferSize - 1 ) ){

          // Save the new character to the end of the buffer.
          buffer[ bufferCounter ] = c;

          // Increment the counters.
          retVal++;
          bufferCounter++;

          // Check if the echo has to be hidden.
          if( secret ){
            source -> print( '*' );
          }
          else{
            source -> print( c );
          }

        }
      }

    }

  }

  // Terminate the buffer.
  buffer[ bufferCounter ] = '\0';

  source -> println();

  // Empty the input stream.
  while( source -> available() ){
    source -> read();
  }

  // Return with error code. It caused by timeout event.
  return -1;

}

int Shellminator::selectList( Stream* source, char* lineText, int numberOfElements, char* list[], uint32_t timeout, bool* selection ){

  // Generic counter variable
  int i;

  // Print the prompt text.
  source -> println( lineText );
  Shellminator::hideCursor( source );

  // It is used to track the index of the selected element.
  int selectedIndex = 0;

  // It is used for the UP / DOWN arrow key parser state machine.
  uint8_t keyState = 0;

  // It is true, whem multiple selection mode is active.
  bool selectMultiple = false;

  // The next available key data will be stored here.
  char c;

  // Save system time.
  unsigned long timerStart = millis();

  // Check if multiple selection mode is required.
  if( selection != NULL ){

    // Set the flag multiple selection mode.
    selectMultiple = true;

    // Reset all elements to zero in the selection array.
    for( i = 0; i < numberOfElements; i++ ){

      selection[ i ] = false;

    }

  }

  // Infinite loop. The timeout protection is handled inside.
  while( 1 ){

    // Print the list.
    for( i = 0; i < numberOfElements; i++ ){

      // Single and multiple element mode has to be printed differently.
      if( selectMultiple ){

        if( selectedIndex == i ){

          source -> print( '>' );

        }

        else{

          source -> print( ' ' );

        }

        if( selection[ i ] ){

          source -> print( "[X] " );

        }

        else{

          source -> print( "[ ] " );

        }

      }

      else{

        if( selectedIndex == i ){

          source -> print( "[X] " );

        }

        else{

          source -> print( "[ ] " );

        }

      }

      // Print the data from the list for the current element.
      source -> print( list[ i ] );

      // If it is not the last element, print a new line.
      if( i < ( numberOfElements - 1 ) ){

        source -> println();

      }

    }

    // Wait for the next keypress. If no timeout event, or the timeout
    // handling is disabled, just wait for the next key.
    while( ( ( millis() - timerStart ) < timeout ) || timeout == 0 ){

      // If key is available, stop waiting.
      if( source -> available() > 0 ){

        break;

      }

    }

    // Check for timeout! If the previous while is finished, but key is
    // not available, that means timeout occured!
    if( source -> available() <= 0 ){

      // This case turn on the cursor and return with error code.
      Shellminator::showCursor( source );
      source -> println();
      return -1;

    }

    // Process key data.
    while( source -> available() > 0 ){

      // Read next key.
      c = source -> read();

      // Check for enter key.
      if( ( c == '\r' ) || ( c == '\n' ) || ( c == '\0' ) ){

        // If multiple select mode is active, we have to return the number of selected elements.
        if( selectMultiple ){

          // Calculate the number of selected elements.
          selectedIndex = 0;

          for( i = 0; i < numberOfElements; i++ ){

            if( selection[ i ] ){

              selectedIndex++;

            }

          }

        }

        // Turn on the cursor and return the right value.
        Shellminator::showCursor( source );
        source -> println();
        return selectedIndex;

      }

      // Check for abort key or backspace.
      else if( ( c == 0x03 ) || ( c == 127 ) || ( c == '\b' ) ){

        // Turn on cursor and return with error code.
        Shellminator::showCursor( source );
        source -> println();
        return -1;

      }

      // In multiple selection mode, we can select or deselect an element
      // with the space key.
      else if( ( c == ' ' ) && selectMultiple ){

        selection[ selectedIndex ] = !selection[ selectedIndex ];

      }

      // Arrow key logic.
      switch( keyState ){

        // Default state
        case 0:

          // If ESC character received, go to the next state
          if( c == 27 ){

            keyState = 1;

          }

          break;
        
        // ESC char received
        case 1:

          // If [ character received, go to the next state
          if( c == '[' ){

            keyState = 2;

          }

          // Otherwise go to default state
          else{

            keyState = 0;

          }

          break;

        // [ char received
        case 2:

          // Up arrow
          if( c == 'A' ){

            // Move the cursor and detect underflow.
            selectedIndex--;
            if( selectedIndex < 0 ){

              selectedIndex = 0;

            }

          }

          // Down arrow
          else if( c == 'B' ){

            // Move the cursor and detect overflow.
            selectedIndex++;
            if( selectedIndex >= numberOfElements ){

              selectedIndex = numberOfElements - 1;

            }

          }

          // Go to default state
          keyState = 0;
          break;

        default:
          keyState = 0;
          break;

      }

    }

    // If a keypress happened, we have to refresh the list.
    // We have to delete the previously printed content.
    for( i = 0; i < numberOfElements; i++ ){

      source -> print( "\033[2K\033[A" );

    }

    source -> println();

    

  }

  // Normally, we should not be here. In case of any problem,
  // turn on the cursor, and return with error code.
  Shellminator::showCursor( source );
  source -> println();
  return -1;

}

Shellminator* Shellminator::castVoidToShellminator( void* ptr ){

  return (Shellminator*)ptr;

}

#ifdef SHELLMINATOR_ENABLE_PROGRESS_BAR_SUPPORT

void Shellminator::drawProgressBar( Stream* stream_p, float percentage, char* text, char done, char todo ){

  float ratio;
  uint32_t i;

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  char printBuffer[ SHELLMINATOR_PROGRESS_BAR_SIZE + SHELLMINATOR_PROGRESS_BAR_TEXT_SIZE + 20 ];
  char* printBufferPtr;

  #else

  char numberBuff[ 15 ];

  #endif

  if( percentage > 100.0f ){
    percentage = 100.0f;
  }

  ratio = percentage / 100.0;


  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  printBufferPtr = printBuffer;

  if( enableFormatting ){
    printBufferPtr += snprintf( printBufferPtr, 15, "\r\033[0K%3d.%d%% ", (int)percentage, (int)( (float)percentage * 10.0 ) % 10 );
  }

  else{
    printBufferPtr += snprintf( printBufferPtr, 15, "\r%3d.%d%% ", (int)percentage, (int)( (float)percentage * 10.0 ) % 10 );
  }

  #else

  if( enableFormatting ){
    snprintf( numberBuff, sizeof( numberBuff ), "\r\033[0K%3d.%d%% ", (int)percentage, (int)( (float)percentage * 10.0 ) % 10 );
  }

  else{
    snprintf( numberBuff, sizeof( numberBuff ), "\r%3d.%d%% ", (int)percentage, (int)( (float)percentage * 10.0 ) % 10 );
  }
  stream_p -> print( numberBuff );

  #endif

  for( i = 0; i < SHELLMINATOR_PROGRESS_BAR_SIZE; i++ ){

    if( ( (float)i / SHELLMINATOR_PROGRESS_BAR_SIZE ) < ratio ){

      #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

      *printBufferPtr = done;
      printBufferPtr++;

      #else

      stream_p -> print( done );

      #endif

    }

    else{

      #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

      *printBufferPtr = todo;
      printBufferPtr++;

      #else

      stream_p -> print( todo );

      #endif

    }

  }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  strncpy( printBufferPtr, " | ", 3 );
  printBufferPtr += 3;

  strncpy( printBufferPtr, text, SHELLMINATOR_PROGRESS_BAR_TEXT_SIZE );
  printBufferPtr += strlen( text );

  stream_p -> print( printBuffer );

  #else

  stream_p -> print( " | " );
  stream_p -> print( text );

  #endif


}

void Shellminator::drawProgressBar( Stream* stream_p, float percentage, char* text ){

  drawProgressBar( channel, percentage, text, '#', '-' );

}

void Shellminator::drawProgressBar( float percentage, char* text, char done_p, char todo_p ){

  drawProgressBar( channel, percentage, text, done_p, todo_p );

}

void Shellminator::drawProgressBar( float percentage, char* text ){

  drawProgressBar( channel, percentage, text, '#', '-' );

}

#endif

void Shellminator::autoDetectTerminal(){

  char buff[ 20 ] = "";
  char magic[] = { 5, 0 };
  Shellminator::input( channel, sizeof( buff ), buff, magic, 500 );

  if( strcmp( buff, "PuTTY" ) == 0 ){

  }

  else{

    enableFormatting = false;
    channel -> println( "unknown terminal..." );
    
  }

}

