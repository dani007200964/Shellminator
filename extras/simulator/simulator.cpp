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

#include <stdio.h>

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

// Commander API-tree
Commander::API_t API_tree[] = {
    apiElement( "format", "Enable or disable terminal formatting.\r\n\tExample: format [ enable ]\r\n\t\t[ enable ] - 0 disabled, other enabled", format_func ),
    apiElement( "cls", "Clear the terminal screen.", cls_func ),
    apiElement( "beep", "Play a beep sound.", beep_func ),
    apiElement( "count", "This function shows a progress bar.", count_func )
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


int main(){

  printf( "Simulator start...\r\n" );

  // Clear the terminal
  shell.clear();

  shell.enableFormatting = false;

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

void count_func(char *args, Stream *response, void* parent )
{

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
