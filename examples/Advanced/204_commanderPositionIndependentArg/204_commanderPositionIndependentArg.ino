
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
#include "Shellminator-Commander-Interface.hpp"
#include "Commander-API.hpp"
#include "Commander-Arguments.hpp"


// We have to create an object from Commander class.
Commander commander;

bool random_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "random", "Generates a random number.\r\n\targs: --low:Minimum number\r\n\t      --high: Maximum number", random_func )
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
    commander.init();

    shell.attachCommander( &commander );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

bool random_func(char *args, CommandCaller* caller ){
    Argument min_number( args, 'l', "low" );
    Argument max_number( args, 'h', "high" );

    min_number.parseInt();
    max_number.parseInt();

    // Check if both parameters are found
    if( (bool)min_number && (bool)max_number ){
        caller -> println( random( (int)min_number, (int)max_number ) );
        return true;
    }

    // Check if only high bound is given
    else if( (bool)max_number && !(bool)min_number ){
        caller -> println( random( (int)max_number ) );
        return true;
    }

    // Check if only low bound is given. Because this doesn't
    // makes much sense generate an error.
    else if( !(bool)max_number && (bool)min_number ){
        caller -> println( "Only low bound is given. Low bound is also neccesary!" );
        return false;
    }

    // Create some formatting on the error message to navigate
    // the user to the right direction.
    caller -> print( "Argument error! Please check " );
    shell.format( caller, Shellminator::UNDERLINE, Shellminator::RED );
    caller -> print( "random?");
    shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
    caller -> println( " for help!" );
    return false;
}