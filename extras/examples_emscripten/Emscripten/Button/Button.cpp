
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

#include "math.h"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Buttons.hpp"

#define NUMBER_OF_DATA_POINTS 30


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// The plot will be generated from the content of this data array.
float data1[ NUMBER_OF_DATA_POINTS ];

// Create a plotter object.
ShellminatorButton button( "Plot" );

Shellminator::shellEvent_t buttonEvent;


// Infinite Loop.
void loop();

// Init Section.
void setup();

// This function will be called, when the button is pressed.
void buttonClick( void );


// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}

void buttonClick( void ){
    button.setColor( Shellminator::GREEN );
}

void setup(){

    // Init code.

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );

    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    button.attachEvent( buttonEvent );
    button.setEventModeAuto();
    button.setRoundCorners();
    button.setColor( Shellminator::RED );

    shell.begin( "arnold" );
    shell.beginScreen( &button, 100 );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}