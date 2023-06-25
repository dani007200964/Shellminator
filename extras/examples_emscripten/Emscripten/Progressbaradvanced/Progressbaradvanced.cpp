
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
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
#include "Shellminator-IO.hpp"
#include "Shellminator-Progress.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress( &shell );

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

// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command, Shellminator* shell );


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

    // Try to allocate memory for 40 characters.
    if( !shell.enableBuffering( 40 ) ){

        // If it fails, print the problem.
        // The error is handled internally, and it will still work, but without buffering.
        stdioChannel.println( "Can not allocate memory for buffering!" );

    }

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "-- Advanced Progress Bar Demo --" );

    // Set the colors for the three segments.
    progress.doneColor = Shellminator::GREEN;
    progress.todoColor = Shellminator::RED;
    progress.middleColor = Shellminator::WHITE;

    // Modify the color and the style of the percent text.
    progress.percentColor = Shellminator::YELLOW;
    progress.percentStyle = Shellminator::BOLD;

    // Modify the color and the style of the user defined progress text.
    progress.textColor = Shellminator::BLUE;
    progress.textStyle = Shellminator::ITALIC;



}

void loop(){

    // Infinite loop.

    int i;

    // Count up to 100%
    for( i = 0; i < 100; i++ ){

        // Set the text and the characters for the three segments.
        progress.drawProgressBar( i, "Counting Up...", "\u2588", "\u2583", "\u2599" );
        delay( 50 );

    }

    // Count down to 0%
    for( i = 100; i > 0; i-- ){

        // Set the text and the characters for the three segments.
        progress.drawProgressBar( i, "Counting Down...", "\u2588", "\u2583", "\u2599" );
        delay( 50 );

    }



}