
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


#include "math.h"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Buttons.hpp"

#define NUMBER_OF_DATA_POINTS 30


void buttonClick( void ){

}

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// The plot will be generated from the content of this data array.
float data1[ NUMBER_OF_DATA_POINTS ];

// Create a plotter object.
ShellminatorButton button( "Plot", buttonClick, 10, 3 );

Shellminator::shellEvent_t buttonEvent;




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    button.attachEvent( buttonEvent );
    button.setEventModeAuto();

    shell.begin( "arnold" );
    shell.beginScreen( &button );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
