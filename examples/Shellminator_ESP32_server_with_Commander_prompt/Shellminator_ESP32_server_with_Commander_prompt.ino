/*
 * Created on Oct. 2 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.03.29
 *
*/

/*
 * This eample shows, how to use the prompts.
 * It is designed to work with ESP32.
 */

#include <WiFi.h>
#include "esp_wifi.h"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Necessary includes
#include "Commander-API.hpp"
#include "Commander-IO.hpp"

#define SERVER_PORT 23

// WiFi credentials.
const char* ssid     = "your-wifi-ssid";
const char* password = "your-wifi-pass";

// Create an instance of the server.
// It will be available on port 23.
WiFiServer server( SERVER_PORT );

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell( &server );

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
void name_func( char *args, Stream *response );
void reboot_func( char *args, Stream *response );
void sudo_func( char *args, Stream *response );
void sum_func( char *args, Stream *response );

// Commander API-tree
Commander::API_t API_tree[] = {
    apiElement( "name", "Enter a name and echo it back.", name_func ),
    apiElement( "reboot", "Reboot command with prompt. To disable prompt use like this: reboot now", reboot_func ),
    apiElement( "sudo", "The worst sudo ever. It demonstrates how to hide typed characters.", sudo_func ),
    apiElement( "sum", "This function sums two number from a prompt.", sum_func )
};

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

  // WiFi configuration section
  Serial.print( "Connect to  WiFi: " );
  Serial.print( ssid );

  WiFi.mode( WIFI_STA );
  WiFi.setSleep(WIFI_PS_NONE);
  WiFi.begin( ssid, password );

  while( WiFi.status() != WL_CONNECTED ){

    delay( 1000 );
    Serial.print( "." );

  }

  shell.beginServer();

  Serial.println( " [ OK ]" );

  Serial.println( "Connected!" );
  Serial.print( "Device IP: " );
  Serial.print( WiFi.localIP() );
  Serial.print( " at port: " );
  Serial.println( SERVER_PORT );


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

  // initialize shell object.
  shell.begin( "arnold" );

}

void loop() {

  shell.update();

}

/// This is an example function for the name command
void name_func(char *args, Stream *response )
{

  char nameBuff[ 30 ];
  int inputResult;

  inputResult = Shellminator::input( response, sizeof( nameBuff ), nameBuff, "Please enter your name: ", 0 );

  if( inputResult ){

    response -> print("Your name is: ");
    response -> println( nameBuff );

  }

  else{
    response -> println( "Prompt error!" );
  }

}

/// This is an example function for the reboot command
void reboot_func(char *args, Stream *response )
{

  if( strcmp( args, "now" ) == 0 ){
    response -> println( "Please wait, the system will reboot..." );
    return;
  }

  response -> print("Reboot the system now?\r\n[ y / n ]: ");

  if( Shellminator::waitForKey( response, "yY", 30000 ) ){
    response -> println( "Please wait, the system will reboot..." );
  }

  else{
    response -> println( "Reboot aborted!" );
  }

}

/// This is an example function for the sudo command
void sudo_func(char *args, Stream *response )
{

  char pwBuff[ 30 ];
  int inputResult;

  inputResult = Shellminator::input( response, sizeof( pwBuff ), pwBuff, "Please enter your password: ", 60000, true );

  if( inputResult > 0 ){

    response -> print("Your password is: ");
    response -> println( pwBuff );

  }

  else{
    response -> println( "Password error!" );
  }

}

/// This is an example function for the sum command
void sum_func(char *args, Stream *response )
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

  // Buffer for number readings.
  char aBuff[ 6 ];
  char bBuff[ 6 ];

  // Try to read the first number.
  argResult = Shellminator::input( response, sizeof( aBuff ), aBuff, "First number: ", 0 );

  // Check if the prompt was filled with something.
  if( argResult < 1 ){

    response -> println( "No input!" );
    return;
  }

  // Parse it as a number.
  argResult = sscanf( aBuff, "%d", &a );

  // Check if it is a number.
  if( argResult != 1 ){

    response -> print( "Argument error! It has to be a number!\r\n" );

    // Sadly we have to stop the command execution and return.
    return;

  }

  // Try to read the first number.
  argResult = Shellminator::input( response, sizeof( bBuff ), bBuff, "Second number: ", 0 );

  // Check if the prompt was filled with something.
  if( argResult < 1 ){

    response -> println( "No input!" );
    return;
  }

  // Parse it as a number.
  argResult = sscanf( bBuff, "%d", &b );

  // Check if it is a number.
  if( argResult != 1 ){

    response -> print( "Argument error! It has to be a number!\r\n" );

    // Sadly we have to stop the command execution and return.
    return;

  }

  // Calculate the sum.
  sum = a + b;

  // Print out the result.
  response -> print( a );
  response -> print( " + " );
  response -> print( b );
  response -> print( " = " );
  response -> println( sum );

}
