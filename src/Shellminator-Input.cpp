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

void Shellminator::input( char *buffer, int bufferSize, const char *instruction, void(*callback)(char*, int, Shellminator*), bool secret ){
    
    // Create a new line for the input prompt.
    channel -> println();

    // Save the buffer and its size.
    inputDestinationBuffer = buffer;
    inputDestinationBufferSize = bufferSize;

    // If the output buffer is larger, than the internal buffer,
    // limit its size. The terminal interface can't handle more
    // characters anyway.
    if( inputDestinationBufferSize > SHELLMINATOR_BUFF_LEN ){
        inputDestinationBufferSize = SHELLMINATOR_BUFF_LEN;
    }

    // Save the callback function address.
    inputCallback = callback;

    // Calculate the text size for the instruction text.
    inputInstuctionSize = strlen( instruction );

    // Print the instruction.
    channel -> print( instruction );

    // Save the flag for the input prompt.
    inputActive = true;

    // Save the secret mode setting.
    inputSecretMode = secret;

    // In case of secret mode, print a lock
    // and midify the instruction text size
    // accordingly.
    if( inputSecretMode ){
        channel -> print( "\U0001F512  " );
        inputInstuctionSize += 3;
    }

    // Redraw the line.
    redrawLine();
}

/*
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
*/
