
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


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// We need a buffer to store the characters from the input.
char nameBuffer[ SHELLMINATOR_BANNER_LEN ];
int nameBufferSize = SHELLMINATOR_BANNER_LEN;

char pathBuffer[ SHELLMINATOR_BANNER_PATH_LEN ];
int pathBufferSize = SHELLMINATOR_BANNER_PATH_LEN;


// Infinite Loop.
void loop();

// Init Section.
void setup();

void pathCallback( char* buffer, int bufferSize, Shellminator* parent ){

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The path input is empty." );

        parent -> input( pathBuffer, pathBufferSize, "Please enter the path: ", pathCallback );
        return;
    }

    // Just in case terminate the string in the buffer.
    buffer[ pathBufferSize - 1 ] = '\0';

    // Create a new prompt for the height.
    parent -> setBannerText( nameBuffer );
    parent -> setBannerPathText( pathBuffer );
}

void nameCallback( char* buffer, int bufferSize, Shellminator* parent ){

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The name input is empty." );

        parent -> input( nameBuffer, nameBufferSize, "Please enter your name: ", nameCallback );
        return;
    }

    // Just in case terminate the string in the buffer.
    buffer[ nameBufferSize - 1 ] = '\0';

    // Create a new prompt for the height.
    parent -> input( pathBuffer, pathBufferSize, "Please enter the path: ", pathCallback );
}


// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}



void setup(){

    // Init code.

    // Initialize shell object.
    shell.begin( "" );

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Generate an input prompt.
    shell.input( nameBuffer, nameBufferSize, "Please enter your name: ", nameCallback );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}