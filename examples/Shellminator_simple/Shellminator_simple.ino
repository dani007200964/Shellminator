/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
 * 
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup forShellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Create a Shellminator object, and initialise it to use Serial
Shellminator shell( &Serial );


const char logo[] = 

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"                                                              \r\n"
;


void setup() {

  // Initialise Serial with 115200 baudrate.
  Serial.begin( 115200 );

  // Wait for connection.
  while( !Serial );

  // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Print start message
  Serial.println( "Program begin..." );

  // Initialise shell object.
  shell.begin( "arnold" );

}

void loop() {
  
  shell.update();

}
