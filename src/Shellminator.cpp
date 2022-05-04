/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.03
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

#if __has_include ("Commander-API.hpp")
#include "Commander-API.hpp"
#endif

const char *Shellminator::version = SHELLMINATOR_VERSION;

#ifdef SHELLMINATOR_USE_ARDUINO_SERIAL
Shellminator::Shellminator( HardwareSerial *serialPort_p ) {

  /// Initialise the arduinoSerialChannel as communication channel.
  arduinoSerialChannel.select( serialPort_p );
  channel = &arduinoSerialChannel;

  /// It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  /// This has to be 1 minimum, because the 0th element is used for the incoming data.
  /// The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  /// Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  /// Because we did not specified the execution function, we have to make it a NULL
  /// pointer to make it detectable.
  execution_fn = NULL;

}

Shellminator::Shellminator( HardwareSerial *serialPort_p, void( *execution_fn_p )( char* ) ) {

  /// Initialise the arduinoSerialChannel as communication channel.
  arduinoSerialChannel.select( serialPort_p );
  channel = &arduinoSerialChannel;

  /// It has to be zero. We dont want to process any garbage.
  cmd_buff_cntr = 0;

  /// This has to be 1 minimum, because the 0th element is used for the incoming data.
  /// The maximum value has to be ( SHELLMINATOR_BUFF_DIM - 1 )
  cmd_buff_dim = 1;

  /// Just in case terminate the begining of the buffer
  cmd_buff[ 0 ][ 0 ] = '\0';

  /// passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;

}
#endif

void Shellminator::attachLogo( char* logo_p ){

  logo = logo_p;

}

void Shellminator::attachLogo( const char* logo_p ){

  logo = (char*)logo_p;

}

void Shellminator::addExecFunc( void( *execution_fn_p )( char* ) ){

  /// passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;

}

void Shellminator::clear() {

  /// explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[H" );  // VT100 Home command
  channel -> write( 27 );    // ESC character( decimal 27 )
  channel -> print( (const char*)"[J" );  // VT100 screen erase command

}

void Shellminator::printBanner() {

  /// Sets the terminal style to bold and the color to green.
  /// You can change it if you like. In my opinion the most
  /// useful is the invisible one :)
  setTerminalCharacterColor( BOLD, GREEN );

  /// Print the banner text.
  channel -> print( banner );

  /// Sets the terminal style to regular and the color to white.
  setTerminalCharacterColor( REGULAR, WHITE );

  /// Prints the end of the banner text. Why this?
  /// I don't know it looks a bit Linuxier this way.
  channel -> print( (const char*)":~$ " );

}

void Shellminator::begin( char* banner_p ) {

  /// Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  /// Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

  /// Set the terminal color and style to the defined settings for the logo
  setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

  /// Draw the startup logo.
  drawLogo();

  /// Print the banner message.
  printBanner();

}

void Shellminator::begin( const char* banner_p ) {

  /// Copy the content from banner_p to banner. Because strncpy we can be sure that it wont overflow.
  strncpy( banner, banner_p, SHELLMINATOR_BANNER_LEN );

  /// Just in case close the string
  banner[ SHELLMINATOR_BANNER_LEN - 1 ] = '\0';

  /// Set the terminal color and style to the defined settings for the logo
  setTerminalCharacterColor( SHELLMINATOR_LOGO_FONT_STYLE, SHELLMINATOR_LOGO_COLOR );

  /// Draw the startup logo.
  drawLogo();

  /// Print the banner message.
  printBanner();

}

void Shellminator::sendBackspace() {

  /// Send a simple backspace combo to the serial port
  channel -> print( (const char*)"\b \b" );

}

void Shellminator::redrawLine(){

  /// General counter variable
  uint32_t i;

  channel -> write( 27 );
  channel -> print( "[2K\r" );

  printBanner();

  #ifdef COMMANDER_API_VERSION

  if( commandFound ){

    setTerminalCharacterColor( BOLD, GREEN );

  }

  else{

    setTerminalCharacterColor( REGULAR, WHITE );

  }

  #endif

  for( i = 0; i < cmd_buff_cntr; i++ ){
    channel -> print( cmd_buff[ 0 ][ i ] );
  }

  for( i = cmd_buff_cntr; i > cursor; i-- ){
      channel -> write( 27 );    // ESC character( decimal 27 )
      channel -> print( '[' );  // VT100 Cursor command.
      channel -> print( '1' );  // 1 character movement.
      channel -> print( 'D' );  // Left.
  }
  
}

void Shellminator::process( char new_char ) {

  /// General counter variable
  uint32_t i;

  /// Check if the new character is backspace character.
  /// '\b' or 127 are both meaning that the backspace kes is pressed
  if ( ( new_char == '\b' ) || ( new_char == 127 ) ) {

    /// If we press a backspace we have to reset cmd_buff_dim to default value
    cmd_buff_dim = 1;

    /// We have to check the number of the characters in the buffer.
    /// If the buffer is empty we must not do anything!
    if ( cursor > 0 ) {

      for( i = (cursor - 1); i < ( cmd_buff_cntr - 1 ); i++ ){

        cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];

      }

      /// If there is at least 1 character in the buffer we jus simply
      /// decrement the cmd_buff_cntr. This will result that the new character
      /// will be stored in the previous characters place in the buffer.
      cmd_buff_cntr--;
      cursor--;

      redrawLine();

      /// We have no more things to do, so return
      return;

    }

  }

  /// If the enter key is pressed in the keyboard, the terminal application
  /// will send a '\r' character.
  else if ( new_char == '\r' ) {

    /// If the enter key is pressed cmd_buff_dim has to be reset to the default value
    cmd_buff_dim = 1;

    /// Because a command is sent we have to close it. Basically we replace the arrived
    /// '\r' character with a '\0' string terminator character. Now we have our command
    /// in a C/C++ like standard string format.
    cmd_buff[ 0 ][ cmd_buff_cntr ] = '\0';

    /// We send a line break to the terminal to put the next data in new line
    channel -> print( '\r' );
    channel -> print( '\n' );

    /// If the arrived tata is not just a single enter we have to process the command.
    if ( cmd_buff_cntr > 0 ) {

      /// We haveto check that execution_fn is not NULL.
      if( execution_fn != NULL ){

        /// If it is a valid, then call it's function.
        execution_fn( cmd_buff[ 0 ] );

      }

      /// If not, then just print it with Serial.
      else{
        channel -> print( (const char*)"cmd: " );
        channel -> print( cmd_buff[ 0 ] );
        channel -> print( '\r' );
        channel -> print( '\n' );
      }

      /// After we processed the command we have to shift the history upwards.
      /// To protect the copy against buffer overflow we use strncpy
      for ( i = ( SHELLMINATOR_BUFF_DIM - 1 ); i > 0; i-- ) {

        strncpy( cmd_buff[ i ], cmd_buff[ i - 1 ], SHELLMINATOR_BUFF_LEN );

      }

    }

    /// After the command processing finished we print a new banner to the terminal.
    /// This means that the device is finished and waits the new command.
    printBanner();

    /// To empty the incoming string we have to zero it's counter.
    cmd_buff_cntr = 0;
    cursor = 0;

    /// We have no more things to do, so return
    return;

  }

  /// This part handles the arrow detection. Arrows are composed as a VT100 command.
  /// These commands usually contains a pattern. This pattern usually starts with
  /// an Escape character, that is deciman 27 in ASCII table.
  /// The escape_state variable stores the state of the VT100 command interpreter
  /// state-machine.
  else if ( new_char == 27 ) {

    /// If escape character recived we set escape_state variable to 1.
    escape_state = 1;

    /// We have no more things to do, so return
    return;
  }

  /// If the escape_state variable is 1 that means we expect that the new character will be
  /// a '[' character.
  else if ( escape_state == 1 ) {

    /// Check that the new character is '['
    if ( new_char == '[' ) {

      /// If it is, we set escape_state variable to 2
      escape_state = 2;

      /// We have no more things to do, so return
      return;

    }

    else {

      /// If the new character is not '[', that means it is not a VT100 command so we have to stop
      /// the interpretation of the escape sequence.
      escape_state = 0;

      /// We have no more things to do, so return
      return;

    }

  }

  /// If the escape_state variable is 2 that means we expect that the new character will be
  /// an 'A', 'B', 'C' or 'D' character. These four characters are represent the four arrow keys.
  /// A -> Up
  /// B -> Down
  /// C -> Right
  /// D -> Left
  else if ( escape_state == 2 ) {

    /// To chose between the four valid values the easyest way is a switch.
    switch ( new_char ) {

      /// Up arrow pressed
      case 'A':

        // Check if the arrow function is overriden.
        if( upArrowOverrideFunc ){

          upArrowOverrideFunc();
          break;

        }

        /// Because we have finished the ecape sequence interpretation we reset the state-machine.
        escape_state = 0;

        /// We have to check that we can go upper in history
        if ( cmd_buff_dim < ( SHELLMINATOR_BUFF_DIM ) ) {

          /// If we can we have to check that the previous command was not empty.
          if ( cmd_buff[ cmd_buff_dim ][0] == '\0' ) {

            /// If it was empty we can't do much with an empty command so we return.
            break;

          }

          /// Now we have to copy the characters form the histoy to the 0th element in the buffer.
          /// Remember the 0th element is always reserved for the new data. If we brows the history the
          /// data in the history will overwrite the data in the 0th element so the historical data will be
          /// the new data. We use strncpy to prevent overflow.
          strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim ], SHELLMINATOR_BUFF_LEN );

          /// We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
          /// It is important to track the end of the loaded string.
          cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
          cursor = cmd_buff_cntr;

          /// We print the loaded command to the terminal interface.
          //channel -> print( cmd_buff[ 0 ] );

          redrawLine();

          /// We have to increment the cmd_buff_dim variable, to track the history position.
          /// Greater number means older command!
          cmd_buff_dim++;

        }

        /// We have finished so we can break from the switch.
        break;

      /// Down arrow pressed
      case 'B':

        // Check if the arrow function is overriden.
        if( downArrowOverrideFunc ){

          downArrowOverrideFunc();
          break;

        }

        /// Because we have finished the ecape sequence interpretation we reset the state-machine.
        escape_state = 0;

        /// We have to check that we can go lover in history, and we are not in the first previous command.
        if ( cmd_buff_dim > 2 ) {

          /// We have to decrement the cmd_buff_dim variable, to track the history position.
          /// Lower number means newer command!
          cmd_buff_dim--;

          /// Now we have to copy the characters form the histoy to the 0th element in the buffer.
          /// Remember the 0th element is always reserved for the new data. If we brows the history the
          /// data in the history will overwrite the data in the 0th element so the historical data will be
          /// the new data. We use strncpy to prevent overflow.
          strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim - 1  ], SHELLMINATOR_BUFF_LEN );

          /// We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
          /// It is important to track the end of the loaded string.
          cmd_buff_cntr = strlen( cmd_buff[ 0 ] );
          cursor = cmd_buff_cntr;

          /// We print the loaded command to the terminal interface.
          ///channel -> print( cmd_buff[ 0 ] );
          redrawLine();

        }

        /// Check that if we are in the first previous command.
        else if ( cmd_buff_dim == 2 ) {

          /// If we are in the first previous command, and we press the down key,
          /// that means we want to go to the 0th element in the terminal.
          /// In this case we have to clear the 0th element.
          for ( i = 0; i < cmd_buff_cntr; i++ ) {

            /// The easyest way is to sand as many backspaces as many character was in the 0th element in the buffer.
            sendBackspace();

          }

          /// To empty the incoming string we have to zero it's counter.
          cmd_buff_cntr = 0;
          cursor = 0;

          /// We have to reset the cmd_buff_dim variable to the default value.
          cmd_buff_dim = 1;

        }

        /// We have finished so we can break from the switch.
        break;

      /// Right arrow pressed
      /// Currently not used
      case 'C':

        // Check if the arrow function is overriden.
        if( rightArrowOverrideFunc ){

          rightArrowOverrideFunc();
          break;

        }
        
        if( cursor < cmd_buff_cntr ){

          channel -> write( 27 );    // ESC character( decimal 27 )
          channel -> print( '[' );  // VT100 Cursor command.
          channel -> print( '1' );  // 1 character movement.
          channel -> print( 'C' );  // Left.
          cursor++;

        }
        
        /// We just simply reset the state-machine.
        escape_state = 0;

        /// We have finished so we can break from the switch.
        break;

        /// Left arrow pressed
        /// Currently not used
      case 'D':

        // Check if the arrow function is overriden.
        if( leftArrowOverrideFunc ){

          leftArrowOverrideFunc();
          break;

        }
        
        if( cursor > 0 ){

          channel -> write( 27 );    // ESC character( decimal 27 )
          channel -> print( '[' );  // VT100 Cursor command.
          channel -> print( '1' );  // 1 character movement.
          channel -> print( 'D' );  // Left.
          cursor--;

        }

        /// We just simply reset the state-machine.
        escape_state = 0;

        /// We have finished so we can break from the switch.
        break;

      // Check for Del key;
      case '3':
        escape_state = 3;
        break;

      /// Any other cases means that it was probably a VT100 command but not an arrow key.
      default:

        /// In this case we just simply reset the state-machine.
        escape_state = 0;

        /// We have finished so we can break from the switch.
        break;

    }

    /// We have finished so we can return.
    return;

  }

  // Detect del key termination.
  else if ( escape_state == 3 ) {

    if( new_char == '~' ){

      // Del key detected.
      /// If we press a delet key we have to reset cmd_buff_dim to default value
      cmd_buff_dim = 1;

      /// We have to check the number of the characters in the buffer.
      /// If the buffer is full we must not do anything!
      if ( cursor != cmd_buff_cntr ) {

        for( i = cursor; i < ( cmd_buff_cntr - 1 ); i++ ){

          cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i + 1 ];

        }

        /// If there is at least 1 character in the buffer we jus simply
        /// decrement the cmd_buff_cntr. This will result that the new character
        /// will be stored in the previous characters place in the buffer.
        cmd_buff_cntr--;

        redrawLine();

      }

    }

    escape_state = 0;
    return;

  }

  // todo Commander command search.
  else if( new_char == '\t' ){

    //channel -> print( "Tabulator!\r\n" );

    #ifdef COMMANDER_API_VERSION

    #endif


    return;

  }

  // Abort key detection.
  else if( new_char == 0x03 ){

    if( abortKeyFunc ){

      abortKeyFunc();

    }

    /// If the abort key is pressed cmd_buff_dim has to be reset to the default value
    cmd_buff_dim = 1;

    /// We send a line break to the terminal to put the next data in new line
    channel -> print( '\r' );
    channel -> print( '\n' );

    printBanner();

    cursor = 0;
    cmd_buff_cntr = 0;
    
    return;

  }

  /// Any other cases means that the new character is just a simple character that was pressed on the keyboard.
  else {

    for( i = cmd_buff_cntr; i > cursor; i-- ){
      cmd_buff[ 0 ][ i ] = cmd_buff[ 0 ][ i - 1 ];
    }

    /// Add the new character to the end of the 0th element in the buffer
    cmd_buff[ 0 ][ cursor ] = new_char;

    /// In this case we have to reset the cmd_buff_dim variable to the default value.
    cmd_buff_dim = 1;

    // Increment counters.
    cmd_buff_cntr++;
    cursor++;

    // Check if the counters overlowed.
    if( cmd_buff_cntr >= ( SHELLMINATOR_BUFF_LEN - 1 ) ){

      cmd_buff_cntr = ( SHELLMINATOR_BUFF_LEN - 1 );

    }

    if( cursor >= ( SHELLMINATOR_BUFF_LEN - 1 ) ){

      cursor = ( SHELLMINATOR_BUFF_LEN - 1 );

    }

    // Redraw the command line.
    redrawLine();

    /// We have finished so we can return.
    return;

  }

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

void Shellminator::update() {

  /// This variable will hold the character that was read from the channel buffer.
  char c;

  // todo wifis eszközöknél kell neki majd még plusz cucc.

  /// We have to check the channel buffer. If it is not empty we can read as many characters as possible.
  while ( channel -> available() ) {

    /// Read one character from channel Buffer.
    c = (char)channel -> read();

    /// Process the new character.
    process( c );

    #ifdef COMMANDER_API_VERSION
    commandCheckTimerStart = millis();
    commandChecked = false;
    #endif

  }

  #ifdef COMMANDER_API_VERSION

  // Todo command detection.
  Commander::API_t *commandAddress;

  if( commander && !commandChecked ){

    if( ( millis() - commandCheckTimerStart ) > 100 ){

      uint32_t i = 0;
      char charCopy;

      for( i = 0; i <= cmd_buff_cntr; i++ ){

        if( ( cmd_buff[ 0 ][ i ] == ' '  ) || ( i == cmd_buff_cntr ) ){

          charCopy = cmd_buff[ 0 ][ i ];
          cmd_buff[ 0 ][ i ] = '\0';
          break;

        }

      }

      commandAddress = commander -> operator[]( cmd_buff[0] );
      if( commandAddress ){
    
        commandFound = true;

      }

      else{

        commandFound = false;

      }

      cmd_buff[ 0 ][ i ] = charCopy;

      //commandCheckTimerStart = millis();
      commandChecked = true;
      redrawLine();

    }

  }

  #endif

}

void Shellminator::setTerminalCharacterColor( uint8_t style, uint8_t color ) {

  /// The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( style );
  channel -> print( ';' );
  channel -> print( color );
  channel -> print( 'm' );

}


void Shellminator::drawLogo() {

  /// Draws the startup logo to the terminal interface.
  channel -> print( logo );

}

#ifdef COMMANDER_API_VERSION

void Shellminator::attachCommander( Commander* commander_p ){

  commander = commander_p;

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
  // To draw a QR-code with a terminal emulator the easyest way to combine these
  // unicode characters. Because it is two 'pixels' high, we have to step the y
  // variable by two lines.
  for( y = 0; y < ( qr_size / 2 ); y++ ){

    // Print a new line at the begining of the horizontal drawing.
    channel -> println();

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
    channel -> println();

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
  channel -> println();

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
