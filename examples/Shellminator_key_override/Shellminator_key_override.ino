/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
 * 
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example demonstrates how to override key behaviour. It can be usefull
 * when you build a robot for example, because you can controll it with arrow
 * keys. Also you can attach an abort handler to stop that robot.
*/

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command );

// Prototype to a function that will be called every time
// when you press the up arrow key.
void upArrowOverride();

// Prototype to a function that will be called every time
// when you press the down arrow key.
void downArrowOverride();

// Prototype to a function that will be called every time
// when you press the left arrow key.
void leftArrowOverride();

// Prototype to a function that will be called every time
// when you press the right arrow key.
void rightArrowOverride();

// Prototype to a function that will be called every time
// when you press the abort key( ctrl+c ).
void abortHandler();

// Create a Shellminator object, and initialise it to use Serial
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

  // Initialise Serial with 115200 baudrate.
  Serial.begin( 115200 );

  // Wait for connection.
  while( !Serial );

  // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Override key functions
  shell.overrideUpArrow( upArrowOverride );
  shell.overrideDownArrow( downArrowOverride );
  shell.overrideLeftArrow( leftArrowOverride );
  shell.overrideRightArrow( rightArrowOverride );
  
  shell.overrideAbortKey( abortHandler );

  // Print start message
  Serial.println( "Program begin..." );

  // Initialise shell object.
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

void upArrowOverride(){

  Serial.println();
  Serial.println( "Up arrow function!" );
  
}

void downArrowOverride(){

  Serial.println();
  Serial.println( "Down arrow function!" );
  
}

void leftArrowOverride(){

  Serial.println();
  Serial.println( "Left arrow function!" );
  
}

void rightArrowOverride(){

  Serial.println();
  Serial.println( "Right arrow function!" );
  
}

void abortHandler(){

  Serial.println();
  Serial.println( "Emergency stop!" );
  Serial.println( "Reset key functions to normal!" );

  shell.freeUpArrow();
  shell.freeDownArrow();
  shell.freeLeftArrow();
  shell.freeRightArrow();
  
}
