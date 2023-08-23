
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
#include "Shellminator-GUI.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorProgress progress;




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();
    shell.enableFormatting = false;

    // Initialize shell object.
    shell.begin( "arnold" );
    progress.setText( "Calculating something..." );
    progress.setFormat( "ms| r" );
    progress.setColor( Shellminator::RED );
    shell.beginScreen( &progress );
    progress.start();


}

// Infinite loop.
void loop(){

    progress.setPercentage( ( ( millis() / 10 ) % 10000 ) / 10000.0 * 100.0 );

    // Process the new data.
    shell.update();


}
