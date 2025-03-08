
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Commander-API.hpp"
#include "Commander-Arguments.hpp"
#include "Shellminator.hpp"
#include "Shellminator-Commander-Interface.hpp"


// We have to create an object from Commander class.
Commander commander;

// Create a colorizer object.
CommanderColorizer colorizer;

bool echo_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "echo", "Simple echo command.", echo_func )
};

// These will be the system variables.
char* VERSION = (char*)"V1.0.2a"; // Simple firmware version string.
int MILLIS = 0; // It will store the milliseconds since boot.
float BATTERY = 100.0; // It will store the battery percentage.

// Create system variable tree.
Commander::systemVariable_t System_variables[] = {
    // MILLIS variable is an int.
    systemVariableInt( MILLIS ),

    // VERSION variable is a string
    systemVariableString( VERSION ),

    // BATTERY variable is a float
    systemVariableFloat( BATTERY )
};

// Create a ShellminatorCommanderInterface object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    commander.attachDebugChannel( &Serial );
    commander.attachTree( API_tree );

    // Attach system variables to the commander object.
    commander.attachVariables( System_variables );

    commander.init();

    shell.attachCommander( &commander );

    // Attach the colorizer to the shell.
    shell.attachColorizer( &colorizer );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Calculate milliseconds since start.
    MILLIS = millis();

    // Fictive battery calculation. Just to make it change over time.
    BATTERY = ( MILLIS % 1000 ) / 10.0;

    // Process the new data.
    shell.update();


}

bool echo_func(char *args, CommandCaller* caller ){
    char textBuffer[ 30 ];

    // The text argument is required to parse system variables.
    Argument text( args, 0 );

    // Try to parse string or system variables
    if( text.parseString( textBuffer ) ){
        caller -> println( textBuffer );
        return true;
    }

    // If parsing failes, just print out the args.
    caller -> println( args );
    return true;
}