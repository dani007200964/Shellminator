
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

const char* listOptions[] ={
    "Arnold",
    "Arnie",
    "T-100"
};

ShellminatorList list( listOptions, 3 );


void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent );


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );
    list.attachCallback( listCallback );
    shell.beginScreen( &list );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent ){
    Serial.print( "List callback with mask: " );
    Serial.println( selected );

    shell.setBannerText( options[ selected ] );

}