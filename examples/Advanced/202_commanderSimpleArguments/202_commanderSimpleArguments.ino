
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

bool cat_func( char *args, CommandCaller* caller );
bool dog_func( char *args, CommandCaller* caller );
bool sum_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "cat", "Description for cat command.", cat_func ),
    systemCommand( "dog", "Description for dog command.", dog_func ),
    systemCommand( "sum", "This function sums two number from the argument list.", sum_func )
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

/// This is an example function for the cat command
bool cat_func(char *args, CommandCaller* caller ){

    caller -> print("Hello from cat function!\r\n");
    return true;

}

/// This is an example function for the dog command
bool dog_func(char *args, CommandCaller* caller ){

    caller -> print("Hello from dog function!\r\n");
    return true;

}

/// This is an example function for the sum command
bool sum_func(char *args, CommandCaller* caller ){
    int sum;
    Argument a( args, 0 );
    Argument b( args, 1 );

    if( !( a.parseInt() && b.parseInt() ) ){
        caller -> print( "Argument error! Two numbers required, separated with a blank space.\r\n" );
        return false;
    }
    
    // Calculate the sum.
    sum = (int)a + (int)b;
    
    // Print out the result.
    caller -> print( (int)a );
    caller -> print( " + " );
    caller -> print( (int)b );
    caller -> print( " = " );
    caller -> println( sum );
    return true;
}