
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


// std library.
#include <stdio.h>
#include <stdlib.h>

// Core System Functions.
#include "System.h"

// Contains a modified Stream class for communication.
#include "stdioStream.hpp"

// Contains Emscripten related functions.
#include <emscripten.h>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command, Shellminator* shell );

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


// Infinite Loop.
void loop();

// Init Section.
void setup();



// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}

void executionFunction( char* command, Shellminator* shell ){

  shell -> channel -> println( "Execution function called!" );
  shell -> channel -> print( "Received command: " );
  shell -> channel -> println( command );

}


void setup(){

    // Init code.

    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}