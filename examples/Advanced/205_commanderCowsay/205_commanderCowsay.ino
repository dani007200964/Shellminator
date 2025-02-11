
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


// Not an average looking cow for sure
const char* cow_top =    "        \\   ^__^\r\n         \\  ";
const char* cow_bottom = "\\_______\r\n            (__)\\       )\\/\\\r\n                ||----w |\r\n                ||     ||\r\n";

// Eye configurations
const char* default_eyes = "(oo)";
const char* tired_eyes = "(--)";
const char* wink_eyes = "(-O)";
const char* greedy_eyes = "($$)";

// We have to create an object from Commander class.
Commander commander;

bool cowsay_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "cowsay", "Like the Linux command. Just give it a string.\r\n\tflags: --greedy\r\n\t       --tired", cowsay_func )
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

bool cowsay_func(char *args, CommandCaller* caller ){
    char textBuffer[ 30 ];
    int i;

    // The text argument is optional but must be the first argument if provided.
    Argument text( args, 0 );

    // These are the optional flags for modifying the cow’s eyes.
    Argument tired_flag( args, 't', "tired" );
    Argument wink_flag( args, 'w', "wink" );
    Argument greedy_flag( args, 'g', "greedy" );

    // Check if the flags exist in the command.
    tired_flag.find();
    wink_flag.find();
    greedy_flag.find();


    caller -> print( "< " );

    // Draw the user text when it is given.
    if( text.parseString( textBuffer ) ){
        caller -> print( textBuffer );
        caller -> println( " >" );
        caller -> print( ' ' );
        for( i = strlen( textBuffer ) + 2; i > 0 ; i-- ){
            caller -> print( '-' );
        }
    }

    // Draw the default text.
    else{
        caller -> print( "booo >\r\n ------" );
    }
    caller -> println();
    

    caller -> print( cow_top );

    // The eye type is determined based on priority.
    if( tired_flag.isFound() ){
        caller -> print( tired_eyes );
    }
    else if( wink_flag.isFound() ){
        caller -> print( wink_eyes );
    }
    else if( greedy_flag.isFound() ){
        caller -> print( greedy_eyes );
    }

    // If no flags are present, use the default eyes.
    else{
        caller -> print( default_eyes );
    }
    caller -> print( cow_bottom );
    return true;
}