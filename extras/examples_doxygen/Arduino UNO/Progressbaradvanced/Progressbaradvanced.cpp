
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
*/


#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Progress.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

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


// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command, Shellminator* shell );


// System init section.
void setup(){

    Serial.begin(115200);

    // Try to allocate memory for 40 characters.
    if( !shell.enableBuffering( 40 ) ){

        // If it fails, print the problem.
        // The error is handled internally, and it will still work, but without buffering.
        Serial.println( "Can not allocate memory for buffering!" );

    }

    // Clear the terminal
    shell.clear();

    Serial.println( "-- Advanced Progress Bar Demo --" );

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

// Infinite loop.
void loop(){

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
