
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
#include "GUI/Shellminator-PlotModule.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

float plotData[] = { 10, 20, 30, 20, 10, 0, -10, -20, -30, -20 };
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

ShellminatorPlot plot( plotData, plotDataSize, "My Plot" );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );

    shell.begin( "arnold" );
    shell.beginScreen( &plot );


}

// Infinite loop.
void loop(){

    while( 1 ){

        shell.update();

    }


}
