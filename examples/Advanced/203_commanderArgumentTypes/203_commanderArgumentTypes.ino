
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

bool upper_func( char *args, CommandCaller* caller );
bool area_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "upper", "Replaces all characters in a string to upper-case.", upper_func ),
    systemCommand( "area", "Calculates the volume of two float numbers", area_func ),
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

bool upper_func(char *args, CommandCaller* caller ){
    // In case of a string argument, a buffer is needed.
    // This buffer will store the the extracted string.
    char stringBuffer[ 30 ];

    int i;

    // Create a positional argument.
    Argument stringArg( args, 0 );

    if( !stringArg.parseString( stringBuffer ) ){
        caller -> print( "Argument error! A string is required!" );
        return false;
    }
    
    for( i = 0; i < strlen( stringBuffer ); i++ ){
        stringBuffer[ i ] = toupper( stringBuffer[ i ] );
    }

    caller -> print( "Upper case string: " );
    caller -> println( stringBuffer );
    return true;
}

bool area_func(char *args, CommandCaller* caller ){
    float area;
    Argument a( args, 0 );
    Argument b( args, 1 );

    if( !( a.parseFloat() && b.parseFloat() ) ){
        caller -> print( "Argument error! Two float numbers are required, separated with a blank space.\r\n" );
        return false;
    }
    
    // Calculate the area.
    area = (float)a * (float)b;
    
    // Print out the result.
    caller -> print( (float)a );
    caller -> print( " * " );
    caller -> print( (float)b );
    caller -> print( " = " );
    caller -> println( area );
    return true;
}