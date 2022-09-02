/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example demonstrates how to attach a Commander-API based command
 * parser to a Shellminator object.
 * To use this example you need the Commander-API to be installed to your
 * Arduino IDE.
*/

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

#include "Commander-API.hpp"
#include "Commander-IO.hpp"

// Create a Shellminator object, and initialize it to use Serial
// If we use Commander, we don't need an execution function,
// because Shellminator internally manages that.
Shellminator shell( &Serial );

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"                                                              \r\n"

;

// We have to create an object from Commander class.
Commander commander;

// We have to create the prototype functions for our commands.
// The arguments has to be the same for all command functions.
void cat_func( char *args, commandResponse *response );
void dog_func( char *args, commandResponse *response );
void sum_func( char *args, commandResponse *response );
void led_func( char *args, commandResponse *response );

// Commander API-tree
Commander::API_t API_tree[] = {
    apiElement( "cat", "Description for cat command.", cat_func ),
    apiElement( "dog", "Description for dog command.", dog_func ),
    apiElement( "led", "Toggle the buit-in LED.", led_func ),
    apiElement( "sum", "This function sums two number from the argument list.", sum_func )
};

void setup() {

  // initialize Serial with 115200 baudrate.
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

  // There is an option to attach a debug channel to Commander.
  // It can be handy to find any problems during the initialization
  // phase. In this example we will use Serial for this.
  commander.attachDebugChannel( &Serial );

  // At start, Commander does not know anything about our commands.
  // We have to attach the API_tree array from the previous steps
  // to Commander to work properly.
  commander.attachTree( API_tree );

  // Initialize Commander.
  commander.init();

  shell.attachCommander( &commander );

  // Initialize shell object.
  shell.begin( "arnold" );

}

void loop() {

  shell.update();

}

/// This is an example function for the cat command
void cat_func(char *args, commandResponse *response )
{

  response -> print("Hello from cat function!\r\n");

}

/// This is an example function for the dog command
void dog_func(char *args, commandResponse *response )
{

  response -> print("Hello from dog function!\r\n");

}

/// This is an example function for the led command
void led_func(char *args, commandResponse *response )
{

  // Toggle your LED pin here, if you have on your board
  // digitalWrite( LED_PIN, !digitalRead( LED_PIN ) );
  response -> print("LED toggle!\r\n");

}

/// This is an example function for the sum command
void sum_func(char *args, commandResponse *response )
{

  // These variables will hold the value of the
  // two numbers, that has to be summed.
  int a = 0;
  int b = 0;

  // This variable will hold the result of the
  // argument parser.
  int argResult;

  // This variable will hold the sum result.
  int sum = 0;

  argResult = sscanf( args, "%d %d", &a, &b );

  // We have to check that we parsed successfully  the two
  // numbers from the argument string.
  if( argResult != 2 ){

    // If we could not parse two numbers, we have an argument problem.
    // We print out the problem to the response channel.
    response -> print( "Argument error! Two numbers required, separated with a blank space.\r\n" );

    // Sadly we have to stop the command execution and return.
    return;

  }

  // Calculate the sum.
  sum = a + b;

  // Print out the result.
  response -> printf( "%d + %d = %d\r\n", a, b, sum );

}
