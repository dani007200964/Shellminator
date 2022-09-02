/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example demonstrates how to create and use an execution function.
 * The executin function just prints out the command, but not execute it.
 * To execute the commands, you need a command parser. You can create your
 * own or use Commander as shown in Shellminator_Commander example.
*/

#include "Shellminator-IO.hpp"
#include "Shellminator.hpp"

// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command );

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial, executionFunction );

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"                                                              \r\n"

;

void setup() {

  // Initialize Serial with 115200 baudrate.
  Serial.begin( 115200 );

  // If you using Atmega32U4, the code will wait, until
  // you open the serial port.
  while( !Serial );

  // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Print start message
  Serial.println( "Program begin..." );

  // initialize shell object.
  shell.begin( "arnold" );

}

void loop() {

  shell.update();

}

void executionFunction( char* command ){

  Serial.println( "Execution function called!" );
  Serial.print( "Received command: " );
  Serial.println( command );

}
