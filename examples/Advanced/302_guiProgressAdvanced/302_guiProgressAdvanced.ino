
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"
#include "GUI/Shellminator-Progress.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress;

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

    progress.setFormat( "t" );
    progress.setText( "Working on something..." );
    progress.setColor( Shellminator::GREEN );

    shell.begin( "arnold" );

    progress.start();
    shell.beginScreen( &progress );


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

        progress.setPercentage( percentage );

    }

    shell.update();



}
