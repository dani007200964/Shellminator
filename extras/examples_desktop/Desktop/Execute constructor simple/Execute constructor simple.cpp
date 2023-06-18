
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example demonstrates how to create and use an execution function.
 * The execution function just prints out the command, but not execute it.
 * To execute the commands, you need a command parser. You can create your
 * own or use Commander as shown in Shellminator_Commander example.
*/


#include <stdio.h>

#include <fcntl.h>
#include <io.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "System.h"

#include "stdioStream.hpp"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command );

// Create a Shellminator object, and initialize it to use stdioChannel.
// Also attach the execution-function.
Shellminator shell( &stdioChannel, executionFunction );

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://github.com/dani007200964/Shellminator\r\n\r\n"

;




// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );



    // Infinite loop.
    while( 1 ){


        // Process the new data.
        shell.update();



    }

}

void executionFunction( char* command ){

  Serial.println( "Execution function called!" );
  Serial.channel -> print( "Received command: " );
  Serial.channel -> println( command );

}