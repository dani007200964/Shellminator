
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


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );



void upArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "up" );
}

void downArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "down" );
}

void leftArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "left" );
}

void rightArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "right" );
}

void searchKeyCallback( Shellminator* caller ){
    caller -> freeSearchKey();
    caller -> freeUpArrow();
    caller -> freeDownArrow();
    caller -> freeLeftArrow();
    caller -> freeRightArrow();
    caller -> setBannerText( "default" );
}


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Use the arrow keys to change the banner text." );
    Serial.print( "To reset the functionality, press" );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    Serial.print( " Ctrl-R ");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "key." );
    Serial.println();

    shell.overrideUpArrow( upArrowCallback );
    shell.overrideDownArrow( downArrowCallback );
    shell.overrideLeftArrow( leftArrowCallback );
    shell.overrideRightArrow( rightArrowCallback );
    shell.overrideSearchKey( searchKeyCallback );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
