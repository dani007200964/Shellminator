
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"
#include "GUI/Shellminator-PlotModule.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Data points
float plotData[] = { 10, 20, 30, 20, 10, 0, -10, -20, -30, -20 };

// Number of points in the data
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

// Create a plot object
ShellminatorPlot plot( plotData, plotDataSize, "My Plot" );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    shell.begin( "arnold" );
    shell.beginScreen( &plot );


}

// Infinite loop.
void loop(){

    shell.update();


}
