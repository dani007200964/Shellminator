
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
#include "GUI/Shellminator-List.hpp"
#include "GUI/Shellminator-Notification.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// This will be used to display the answer for the chosen answer.
ShellminatorNotification notification;

// These options are available for choose.
const char* listOptions[] = {
    "Red Pill",
    "Blue Pill"
};

// Instructions for the list.
const char* listText =  "Choose your destiny Neo";

// Create a list.
ShellminatorList neoList( listOptions, 2, listText );

// Create a callback for the list.
void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    // Attach the callback for the list.
    neoList.attachCallback( listCallback );

    shell.begin( "arnold" );
    shell.beginScreen( &neoList );


}

// Infinite loop.
void loop(){

    shell.update();


}

void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    // Generate answer based on the selected answer.
    if( selected == 0 ){
        notification.setText( "You stay in Wonderland and I show\nyou how deep the rabbit hole goes." );
    }
    else{
        notification.setText( "The story ends, you wake up in your bed and\nbelieve whatever you want to believe." );
    }

    parent -> swapScreen( &notification );

}