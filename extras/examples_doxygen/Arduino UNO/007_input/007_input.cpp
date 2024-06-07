
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup for Shellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/


#include "Shellminator.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// We need a buffer to store the characters from the input.
char inputBuffer[ 30 ];


void inputCallback( char* buffer, int bufferSize, Shellminator* parent ){
    // The input does not generate a new line before the callback,
    // if it is required, it needs to be done manually.
    parent -> channel -> println();

    // Echo back the name.
    parent -> channel -> print( "Your name is: " );
    parent -> channel -> print( buffer );
}


// System init section.
void setup(){

    Serial.begin(115200);

    // Initialize shell object.
    shell.begin( "arnold" );

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Generate an input prompt.
    shell.input( inputBuffer, sizeof( inputBuffer ), "Please enter your name: ", inputCallback );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
