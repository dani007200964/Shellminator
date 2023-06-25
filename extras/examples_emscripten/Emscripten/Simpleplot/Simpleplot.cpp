
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

#include "math.h"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-PlotModule.hpp"

#define NUMBER_OF_DATA_POINTS 30


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// The plot will be generated from the content of this data array.
float data1[ NUMBER_OF_DATA_POINTS ];

// Create a plotter object.
ShellminatorPlot plot( &shell, data1, sizeof( data1 ) / sizeof( data1[ 0 ] ) );

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



void setup(){

    // Init code.

    // Clear the terminal
    shell.clear();

    if( !plot.begin() ){

        stdioChannel.println( "Memory allocation failed for plot!" );

    }

    stdioChannel.println( "Program Start!" );

    // Generate a pretty sine-wave.
    for( int i = 0; i < NUMBER_OF_DATA_POINTS; i++ ){

        data1[i] = sin( 3.14159265358979323846 * 4.0 * (float)i / (float)NUMBER_OF_DATA_POINTS  ) * 3.0;

    }

    plot.draw();

    shell.begin( "arnold" );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}