
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


#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"


// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command );

// Create a Shellminator object, and initialize it to use Serial.
// Also attach the execution-function.
Shellminator shell( &Serial, executionFunction );

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




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void executionFunction( char* command ){

  Serial.println( "Execution function called!" );
  Serial.print( "Received command: " );
  Serial.println( command );

}