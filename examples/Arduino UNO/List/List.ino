
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
    "opt-1",
    "opt-2",
    "opt-3",
    "opt-4",
    "opt-5",
    "opt-6",
    "opt-7",
    "opt-8",
    "opt-9",
    "opt-10",
    "opt-11",
    "opt-12"
};

const char* listDetails[] ={
    "Text for option 1\nEgyszer volt\nol nem volt\nvolt egyszer egy cica\nFarkan volt a mica\nevett volna sokat\nblah blah blak\n\n\n\asdasd",
    "Text for option 2",
    "Text for option 3",
    "Text for option 4",
    "Text for option 5",
    "Text for option 6",
    "Text for option 7",
    "Text for option 8",
    "Text for option 9",
    "Text for option 10",
    "Text for option 11",
    "Text for option 12"
};

ShellminatorListDetailed list( listOptions, listDetails, 12, "Please select a name:" );


void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent );


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();
    shell.enableFormatting = false;

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

    shell.setBannerText( options[ selected ] );

}