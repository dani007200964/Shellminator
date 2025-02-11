
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
#include "GUI/Shellminator-Level-Meter.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorLevelMeter meter( "Audio Level" );

uint32_t timerStart = 0;
uint32_t period = 100;
float percentage = 1.0;
float step = 1.0;




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    shell.begin( "arnold" );
    shell.beginScreen( &meter );


}

// Infinite loop.
void loop(){

    if( ( millis() - timerStart ) > period ){
        timerStart = millis();
        percentage += step;

        if( percentage > 100.0 ){
            step = -1.0;
            percentage = 100.0;
        }

        if( percentage < 0.0 ){
            step = 1.0;
            percentage = 0.0;
        }

        meter.setPercentage( percentage );

    }

    shell.update();


}
