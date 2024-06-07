
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
char inputBuffer[ 30 ];

int width = 0;
int height = 0;


// Infinite Loop.
void loop();

// Init Section.
void setup();

void heightCallback( char* buffer, int bufferSize, Shellminator* parent ){

    int scan_result;

    scan_result = sscanf( buffer, "%d", &height );

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    if( scan_result != 1 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    if( height <= 0 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // Calculate the area
    int area = width * height;

    // Print the result
    parent -> channel -> println();
    parent -> channel -> print( "The area of the plane is: " );
    parent -> channel -> print( area );
    parent -> channel -> print( "mm2" );
}

void widthCallback( char* buffer, int bufferSize, Shellminator* parent ){

    int scan_result;

    scan_result = sscanf( buffer, "%d", &width );

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( scan_result != 1 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( width <= 0 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    // Create a new prompt for the height.
    parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
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
    shell.begin( "arnold" );

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Generate an input prompt.
    shell.input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}