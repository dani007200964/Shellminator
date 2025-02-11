
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
#include <math.h>


// Simulation constants
#define R 5100.0      // Resistor (Ohm)
#define C 0.01    // Capacitance (Farad)
#define U0 5.0        // Supply Voltage (Volt)

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

float plotData[ 100 ];
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

uint32_t timerStart = 0;
uint32_t timerPeriod = 1000;

// Set a custom color for the plot.
ShellminatorPlot plot( plotData, plotDataSize, "Capacitor Voltage[ V ]", Shellminator::RED );


void capacitorSimulator();


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

    // In the timerPeriod defined intervals, recalculate the current
    // state of the simulated capacitor.
    if( ( millis() - timerStart ) > timerPeriod ){
        timerStart = millis();
        capacitorSimulator();

        // After calculation, we have to request the shell
        // to redraw the plot.
        shell.requestRedraw();
    }

    shell.update();


}

void capacitorSimulator(){
    int i;

    // With the for loop, we shift every element in the plot to the left
    for( i = 0; i < plotDataSize - 1; i++ ){
        plotData[ i ] = plotData[ i + 1 ];
    }

    // To the last element, we calculate the new value based on system time.
    plotData[ plotDataSize - 1 ] = U0 * ( 1.0 - exp( -( millis() / 1000.0 ) *  ( 1.0 / ( R * C ) ) ) );

}