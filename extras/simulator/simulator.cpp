/*
 * Created on April 08 2023
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.04.08
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

/*
This is a basic simulator for Shellminator. You can modify this code as you like, to test
your code. It is faster and easier, than working with an embedded device first.
Please note that, some functions are not working here.
*/

#include <math.h>
#include <stdio.h>

#include <fcntl.h>
#include <io.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"

#include "stdioStream.hpp"

// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &stdioChannel );

// We have to create an object from Commander class.
Commander commander;

// We have to create the prototype functions for our commands.
// The arguments has to be the same for all command functions.
void format_func( char *args, Stream *response, void* parent );
void cls_func( char *args, Stream *response, void* parent );
void beep_func( char *args, Stream *response, void* parent );
void count_func( char *args, Stream *response, void* parent );
void select_func( char *args, Stream *response, void* parent );
void fruit_func( char *args, Stream *response, void* parent );
void getCursor_func( char *args, Stream *response, void* parent );
void setCursor_func( char *args, Stream *response, void* parent );

// Commander API-tree
Commander::API_t API_tree[] = {
    apiElement( "format", "Enable or disable terminal formatting.\r\n\tExample: format [ enable ]\r\n\t\t[ enable ] - 0 disabled, other enabled", format_func ),
    apiElement( "cls", "Clear the terminal screen.", cls_func ),
    apiElement( "beep", "Play a beep sound.", beep_func ),
    apiElement( "count", "This function shows a progress bar.", count_func ),
    apiElement( "select", "This function shows a select list", select_func ),
    apiElement( "fruit", "This function shows how to select multiple elements from a list.", fruit_func ),
    apiElement( "getCursor", "This function returns the cursor position in the terminal.", getCursor_func ),
    apiElement( "setCursor", "This function set the cursor position in the terminal.", setCursor_func )
};

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"
"\r\n\033[0;37m"

;

//ShellminatorBufferedPrinter printer;
ShellminatorBufferedPrinter printer( &stdioChannel );
int printerBufferSize = 30;

int main(){

  //_setmode( _fileno( stdout ), 0x00020000 ); // _O_U16TEXT is 0x00020000
  if( setlocale(LC_ALL, NULL) == NULL ){

    wprintf( L"Error setting locale!\r\n" );

  }

  wprintf( L"Simulator start...\r\n" );

  // Try to identify the terminal emulator.
  shell.autoDetectTerminal();

  shell.enableFormatting = true;

  // Clear the terminal
  //shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );


  // There is an option to attach a debug channel to Commander.
  // It can be handy to find any problems during the initialization
  // phase. In this example we will use stdio for this.
  commander.attachDebugChannel( &stdioChannel );

  // At start, Commander does not know anything about our commands.
  // We have to attach the API_tree array from the previous steps
  // to Commander to work properly.
  commander.attachTree( API_tree );

  // Initialize Commander.
  commander.init();

  shell.attachCommander( &commander );

  shell.enableBuffering();

  /*
  printer.allocate( 30 );
  printer.printf( "Test!\r\n" );
  printer.flush();
  printer.~ShellminatorBufferedPrinter();
  */
  

  // Begin the terminal.
  shell.begin( "arnold" );

  while( 1 ){


    // Process the terminal.
    shell.update();


  }

  // The parameters to the function are put after the comma
  return 0;

}

void format_func(char *args, Stream *response, void* parent )
{

  // Pointer to shell object if possible.
  Shellminator* shell;

  // argument value
  int value;

  // This variable will hold the result of the
  // argument parser.
  int argResult;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );

    argResult = sscanf( args, "%d", &value );

    if( argResult != 1 ){

      response -> print( "Argument error!\r\n" );
      return;

    }

    shell -> enableFormatting = !!value;
    return;

  }

  response -> print("Shell object not available!\r\n");

}

void cls_func(char *args, Stream *response, void* parent )
{

  // Pointer to shell object if possible.
  Shellminator* shell;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );
    shell -> clear();

  }

}

void beep_func(char *args, Stream *response, void* parent )
{

  // Pointer to shell object if possible.
  Shellminator* shell;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );
    shell -> beep();

  }

}

void count_func(char *args, Stream *response, void* parent ){

  // Pointer to shell object if possible.
  Shellminator* shell;

  uint32_t i;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );

    for( i = 0; i < 10; i++ ){

      shell -> drawProgressBar( i * 10.0, (char*)"Counting...", 'O', 'o' );
      delay( 200 );

    }

    shell -> drawProgressBar( 100.0, (char*)"Finished counting!", 'O', 'o' );

    response -> println();

  }

}

void select_func(char *args, Stream *response, void* parent ){

  // List of options.
  char *options[] = {
    (char*) "Yes",
    (char*) "No",
    (char*) "Noooooo"
  };

  // It will store the index of the selected element.
  int selected;

  // Calculate the number of elements in the options array.
  int optionsSize = sizeof( options ) / sizeof( options[ 0 ] );

  // Create a list prompt with single selection mode.
  selected = Shellminator::selectList( response, (char*)"Select answer:", optionsSize, options, 5000 );

  // Check for the result. If it is greater or
  // equal with 0, that means the result is valid.
  if( selected >= 0 ){

    // Print the selected object.
    response -> print( "Selected option: " );
    response -> println( options[ selected ] );

  }

  else{

    // We have an error. Print it.
    response -> println( "Abort, or timeout!" );

  }

}

void fruit_func(char *args, Stream *response, void* parent ){

  // List of options.
  char *options[ 5 ] = {
    (char*) "Mango",
    (char*) "Apple",
    (char*) "Lime",
    (char*) "Banana",
    (char*) "Raspberry"
  };

  // The result will be generated here.
  bool resultList[ 5 ];

  // It will hold the status of the prompt.
  int status;

  // Generic counter.
  int i;

  // Calculate the number of elements in the options array.
  int optionsSize = sizeof( options ) / sizeof( options[ 0 ] );

  // Create a list prompt with multiple selection mode.
  status = Shellminator::selectList( response, (char*)"Select answer:", optionsSize, options, 60000, resultList );

  // Check the status. If it is greater, than 0,
  // that means we have at least one element selected.
  if( status > 0 ){

    // Print the selected elements.
    response -> println( "The following selected in the list:" );

    for( i = 0; i < optionsSize; i++ ){

      if( resultList[ i ] ){

        response -> print( "\t-" );
        response -> println( options[ i ] );

      }

    }

  }

  // If the status is 0, that means we do not have an error, but nothing has been selected.
  else if( status == 0 ){

    response -> println( "Nothing selected!" );

  }

  // If the result is less than 0, that means, we have an error.
  else{

    response -> println( "Abort, or timeout!" );

  }

}

void getCursor_func(char *args, Stream *response, void* parent )
{

  int x;
  int y;

  // Pointer to shell object if possible.
  Shellminator* shell;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );
    if( shell -> getCursorPosition( &x, &y ) ){
      response -> println( x );
      response -> println( y );
    }

    else{

      response -> println( "Error!" );

    }

  }



}

void setCursor_func(char *args, Stream *response, void* parent )
{

  // Pointer to shell object if possible.
  Shellminator* shell;

  if( parent ){

    shell = Shellminator::castVoidToShellminator( parent );
    shell -> setCursorPosition( 999, 999 );

  }



}


