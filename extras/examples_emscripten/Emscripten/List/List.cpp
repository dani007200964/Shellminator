
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

#include "Shellminator.hpp"
#include "Shellminator-GUI.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

const char* listOptions[] ={
    "Arnold",
    "Arnie",
    "T-100"
};

ShellminatorList list( listOptions, 3 );


// Infinite Loop.
void loop();

// Init Section.
void setup();

void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent );


// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}

void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent ){
    stdioChannel.print( "List callback with mask: " );
    stdioChannel.println( selected );

    shell.setBannerText( options[ selected ] );

}

void setup(){

    // Init code.

    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );
    list.attachCallback( listCallback );
    shell.beginScreen( &list );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}