/*
* Created on Aug 10 2020
*
* Copyright (c) 2020 - Daniel Hajnal
* hajnal.daniel96@gmail.com
*
* This file is part of the Shellminator project.
*/

#include "Shellminator.hpp"

const char *Shellminator::version = SHELLMINATOR_VERSION;

const char *Shellminator::logo __attribute__((weak))=
"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"                                                              \r\n"
;


Shellminator::Shellminator( SHELLMINATOR_SERIAL_CLASS *serialPort_p ) {

  /// Passes the serial port object to the local variable.
  serialPort = serialPort_p;

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

Shellminator::Shellminator( SHELLMINATOR_SERIAL_CLASS *serialPort_p, void( *execution_fn_p )( char* ) ) {

  /// Passes the serial port object to the local variable.
  serialPort = serialPort_p;

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

void Shellminator::addExecFunc( void( *execution_fn_p )( char* ) ){

  /// passing execution_fn_p to execution_fn
  execution_fn = execution_fn_p;

}

void Shellminator::clear() {

  /// explanation can be found here: http://braun-home.net/michael/info/misc/VT100_commands.htm
  serialPort -> write( 27 );    // ESC character( decimal 27 )
  serialPort -> print( "[H" );  // VT100 Home command
  serialPort -> write( 27 );    // ESC character( decimal 27 )
  serialPort -> print( "[J" );  // VT100 screen erase command

}

void Shellminator::printBanner() {

  /// Sets the terminal style to bold and the color to green.
  /// You can change it if you like. In my opinion the most
  /// useful is the invisible one :)
  setTerminalCharacterColor( BOLD, GREEN );

  /// Print the banner text.
  serialPort -> print( banner );

  /// Sets the terminal style to regular and the color to white.
  setTerminalCharacterColor( REGULAR, WHITE );

  /// Prints the end of the banner text. Why this?
  /// I don't know it looks a bit Linuxier this way.
  serialPort -> print( ":~$ " );

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
  serialPort -> print( "\b \b" );

}

void Shellminator::process( char new_char ) {

  /// General counter variable
  uint32_t i;

  /// Add the new character to the end of the 0th element in the buffer
  cmd_buff[ 0 ][ cmd_buff_cntr ] = new_char;

  /// Check if the new character is backspace character.
  /// '\b' or 127 are both meaning that the backspace kes is pressed
  if ( ( new_char == '\b' ) || ( new_char == 127 ) ) {

    /// If we press a backspace we have to reset cmd_buff_dim to default value
    cmd_buff_dim = 1;

    /// We have to check the number of the characters in the buffer.
    /// If the buffer is empty we must not do anything!
    if ( cmd_buff_cntr > 0 ) {

      /// If there is at least 1 character in the buffer we jus simply
      /// decrement the cmd_buff_cntr. This will result that the new character
      /// will be stored in the previous characters place in the buffer.
      cmd_buff_cntr--;

      /// We have to send a backspace combo to the terminal application
      /// to actually remove the character from the terminal interfacel.
      sendBackspace();

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
    serialPort->println();

    /// If the arrived tata is not just a single enter we have to process the command.
    if ( cmd_buff_cntr > 0 ) {

      /// We haveto check that execution_fn is not NULL.
      if( execution_fn != NULL ){

        /// If it is a valid, then call it's function.
        execution_fn( cmd_buff[ 0 ] );

      }

      /// If not, then just print it with Serial.
      else{
        serialPort -> print( "cmd: " );
        serialPort -> println( cmd_buff[ 0 ] );
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

        /// Because we have finished the ecape sequence interpretation we reset the state-machine.
        escape_state = 0;

        /// We have to check that we can go upper in history
        if ( cmd_buff_dim < ( SHELLMINATOR_BUFF_DIM ) ) {

          /// If we can we have to check that the previous command was not empty.
          if ( cmd_buff[ cmd_buff_dim ][0] == '\0' ) {

            /// If it was empty we can't do much with an empty command so we return.
            break;

          }

          /// If it was not empty we have to clear the text that was in the terminal.
          /// Only the command text has to be deleted!
          for ( i = 0; i < cmd_buff_cntr; i++ ) {

            /// The easyest way is to sand as many backspaces as many character was in the 0th element in the buffer.
            sendBackspace();

          }

          /// Now we have to copy the characters form the histoy to the 0th element in the buffer.
          /// Remember the 0th element is always reserved for the new data. If we brows the history the
          /// data in the history will overwrite the data in the 0th element so the historical data will be
          /// the new data. We use strncpy to prevent overflow.
          strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim ], SHELLMINATOR_BUFF_LEN );

          /// We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
          /// It is important to track the end of the loaded string.
          cmd_buff_cntr = strlen( cmd_buff[ 0 ] );

          /// We print the loaded command to the terminal interface.
          serialPort -> print( cmd_buff[ 0 ] );

          /// We have to increment the cmd_buff_dim variable, to track the history position.
          /// Greater number means older command!
          cmd_buff_dim++;

        }

        /// We have finished so we can break from the switch.
        break;

      /// Down arrow pressed
      case 'B':

        /// Because we have finished the ecape sequence interpretation we reset the state-machine.
        escape_state = 0;

        /// We have to check that we can go lover in history, and we are not in the first previous command.
        if ( cmd_buff_dim > 2 ) {

          /// We have to decrement the cmd_buff_dim variable, to track the history position.
          /// Lower number means newer command!
          cmd_buff_dim--;

          /// We have to clear the text that was in the terminal.
          /// Only the command text has to be deleted!
          for ( i = 0; i < cmd_buff_cntr; i++ ) {

            /// The easyest way is to sand as many backspaces as many character was in the 0th element in the buffer.
            sendBackspace();

          }

          /// Now we have to copy the characters form the histoy to the 0th element in the buffer.
          /// Remember the 0th element is always reserved for the new data. If we brows the history the
          /// data in the history will overwrite the data in the 0th element so the historical data will be
          /// the new data. We use strncpy to prevent overflow.
          strncpy( cmd_buff[ 0 ], cmd_buff[ cmd_buff_dim - 1  ], SHELLMINATOR_BUFF_LEN );

          /// We have to calculate the historical data length to pass it to the cmd_buff_cntr variable.
          /// It is important to track the end of the loaded string.
          cmd_buff_cntr = strlen( cmd_buff[ 0 ] );

          /// We print the loaded command to the terminal interface.
          serialPort -> print( cmd_buff[ 0 ] );

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

          /// We have to reset the cmd_buff_dim variable to the default value.
          cmd_buff_dim = 1;

        }

        /// We have finished so we can break from the switch.
        break;

      /// Right arrow pressed
      /// Currently not used
      case 'C':

        /// We just simply reset the state-machine.
        escape_state = 0;

        /// We have finished so we can break from the switch.
        break;

        /// Right arrow pressed
        /// Currently not used
      case 'D':

        /// We just simply reset the state-machine.
        escape_state = 0;

        /// We have finished so we can break from the switch.
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

  /// Any other cases means that the new character is just a simple character that was pressed on the keyboard.
  else {

    /// In this case we have to reset the cmd_buff_dim variable to the default value.
    cmd_buff_dim = 1;

    /// We hace to increment the cmd_buff_cntr variable, to point to the next empty space in the buffer.
    cmd_buff_cntr++;

    /// We have to check that after the incrementation of cmd_buff_cntr, we are still in the boundaries of the buffer.
    /// We also have to reserve space for the terminator character, this is why we have to subtract 1 from
    /// the buffer size in the compersation.
    if ( cmd_buff_cntr >= ( SHELLMINATOR_BUFF_LEN - 1 ) ) {

      /// If the buffer is full we dont let cmd_buff_cntr variable to go further than the size of the buffer - 2.
      /// In this case the new characters will be rejected by the module.
      cmd_buff_cntr = SHELLMINATOR_BUFF_LEN - 2;

    }

    /// Other case means the buffer is luckily not full.
    else {

      /// In this case we print the new character to make it visible on the terminal interface.
      serialPort -> print( new_char );

    }

    /// We have finished so we can return.
    return;

  }

}

void Shellminator::update() {

  /// This variable will hold the character that was read from the Serial buffer.
  char c;

  /// We have to check the Serial buffer. If it is not empty we can read as many characters as possible.
  while ( serialPort -> available() ) {

    /// Read one character from Serial Buffer.
    c = (char)serialPort -> read();

    /// Process the new character.
    process( c );

  }

}

void Shellminator::setTerminalCharacterColor( uint8_t style, uint8_t color ) {

  /// The reference what I used can be found here: https://www.nayab.xyz/linux/escapecodes.html
  serialPort -> write( 27 );
  serialPort -> print( "[" );
  serialPort -> print( style );
  serialPort -> print( ";" );
  serialPort -> print( color );
  serialPort -> print( "m" );

}


void Shellminator::drawLogo() {

  /// Draws the startup logo to the terminal interface.
  serialPort -> print( logo );

}