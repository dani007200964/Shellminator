
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

    // Clear the terminal
    shell.clear();

    Serial.println( "-- Simple Progress Bar Demo --" );


}

// Infinite loop.
void loop(){

    int i;
    for( i = 0; i < 100; i++ ){

        progress.drawProgressBar( i, "Test progress" );
        delay( 50 );

    }

    for( i = 100; i > 0; i-- ){

        progress.drawProgressBar( i, "Test progress" );
        delay( 50 );

    }


}
