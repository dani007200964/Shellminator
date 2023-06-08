/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

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

  if( bufferMemoryAllocated ){

    redrawHistorySearchBuffered();

  }

  else{

    redrawHistorySearchSimple();

  }

}

void Shellminator::redrawHistorySearchSimple(){

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

  channel -> print( '\r' );
  setTerminalCharacterColor( REGULAR, WHITE );
  channel -> print( "(reverse-i-search)'" );  // 19 characters long.
  setTerminalCharacterColor( BOLD, YELLOW );
  channel -> print( cmd_buff[ 0 ] );
  setTerminalCharacterColor( REGULAR, WHITE );
  channel -> print( "': \033[0K" );

  if( cmd_buff_cntr == 0 ){

    return;

  }

  for( i = 1; i < SHELLMINATOR_BUFF_DIM; i++ ){

    searchResult = substring( cmd_buff[ 0 ], cmd_buff[ i ] );

    if( searchResult >= 0 ){

      searchMatch = i;

      for( j = 0; j < strlen( cmd_buff[ i ] ); j++ ){

        if( !highlighted && ( j == searchResult ) ){

          setTerminalCharacterColor( BOLD, YELLOW );

          highlighted = true;

        }

        if( highlighted && ( j == ( searchResult + strlen( cmd_buff[ 0 ] ) ) ) ){

          setTerminalCharacterColor( REGULAR, WHITE );

          highlighted = false;

        }

        channel -> print( cmd_buff[ i ][ j ] );

      }

      if( highlighted ){

        setTerminalCharacterColor( REGULAR, WHITE );

      }

      channel -> print( '\r' );

      channel -> write( 27 );
      channel -> print( '[' );
      channel -> print( uint8_t( 19 + cursor ) );
      channel -> print( 'C' );

      return;

    }

  }

  channel -> print( '\r' );

  channel -> write( 27 );
  channel -> print( '[' );
  channel -> print( uint8_t( 19 + cursor ) );
  channel -> print( 'C' );

}

void Shellminator::redrawHistorySearchBuffered(){

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

  bufferedPrinter.printf( "\r\033[0;37m(reverse-i-search)'\033[1;33m%s\033[0;37m': \033[0K", cmd_buff[ 0 ] );

  if( cmd_buff_cntr == 0 ){

    bufferedPrinter.flush();
    return;

  }

  for( i = 1; i < SHELLMINATOR_BUFF_DIM; i++ ){

    searchResult = substring( cmd_buff[ 0 ], cmd_buff[ i ] );
    if( searchResult >= 0 ){

      searchMatch = i;

      for( j = 0; j < strlen( cmd_buff[ i ] ); j++ ){

        if( !highlighted && ( j == searchResult ) ){

          setTerminalCharacterColor( &bufferedPrinter, BOLD, YELLOW );
          highlighted = true;

        }

        if( highlighted && ( j == ( searchResult + strlen( cmd_buff[ 0 ] ) ) ) ){

          setTerminalCharacterColor( &bufferedPrinter, REGULAR, WHITE );
          highlighted = false;

        }

        bufferedPrinter.printf( "%c", cmd_buff[ i ][ j ] );

      }

      if( highlighted ){

        setTerminalCharacterColor( &bufferedPrinter, REGULAR, WHITE );

      }

      bufferedPrinter.printf( "\r\033[%dC", uint8_t( 19 + cursor ) );
      bufferedPrinter.flush();

      return;

    }

  }

  bufferedPrinter.printf( "\r\033[%dC", uint8_t( 19 + cursor ) );
  bufferedPrinter.flush();

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

