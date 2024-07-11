
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



#include "math.h"

#include "Shellminator.hpp"
#include "GUI/Shellminator-Buttons.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a plotter object.
ShellminatorButton button( "This is a button" );

Shellminator::shellEvent_t buttonEvent;


// This function will be called, when the button is pressed.
void buttonClick( ShellminatorScreen* screen );


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    button.attachEvent( buttonEvent );
    button.attachTriggerFunction( buttonClick );
    button.setColor( Shellminator::RED );

    shell.begin( "arnold" );
    shell.beginScreen( &button, 100 );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void buttonClick( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    button.setColor( Shellminator::GREEN );
    parent -> requestRedraw();
}