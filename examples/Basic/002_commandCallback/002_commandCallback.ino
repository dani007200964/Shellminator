
/*
 * Created on June 8 2024
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a function, that can be called every time when a command gets entered.
void execution_function( char* command, Shellminator* caller ){
    // Just simply echo back the received command.
    caller -> channel -> print( "We have a command to execute: " );
    caller -> channel -> println( command );
}




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the execution function to the shell object.
    shell.attachExecFunc( execution_function );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
