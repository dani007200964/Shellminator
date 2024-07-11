
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


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

float plotData[ 100 ];
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

float amplitude = 30.0;
float periods = 2.0;

uint32_t timerStart = 0;
uint32_t timerPeriod = 1000;

ShellminatorPlot plot( plotData, plotDataSize, "Simple Sine", Shellminator::YELLOW );


void calcSine();


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );
    calcSine();

    shell.begin( "arnold" );
    shell.beginScreen( &plot );


}

// Infinite loop.
void loop(){

    while( 1 ){

        if( ( millis() - timerStart ) > timerPeriod ){
            timerStart = millis();
            calcSine();
            shell.requestRedraw();
        }

        shell.update();

    }


}

void calcSine(){
    int i;

    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = sin( millis() / 1000.0 + periods * 2.0 * M_PI * (float)i / (float)plotDataSize ) * amplitude;
    }

}