
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

// Create a button object and specify the text for it.
ShellminatorButton button( "This is a button" );

// Create an event handler for the button.
Shellminator::shellEvent_t buttonEvent;

// It will be used later when pressing the button.
bool toggle = false;


// This function will be called, when the button is pressed.
void buttonClick( ShellminatorScreen* screen );


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    // Configure the event for the x key on the keyboard.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    // Attach the event to the button.
    button.attachEvent( buttonEvent );

    // Attach the callback function for the button.
    // It will be called, when the button gets pressed.
    button.attachTriggerFunction( buttonClick );

    // Set the default color for the button to red.
    button.setColor( Shellminator::RED );

    // Start as usual but...
    shell.begin( "arnold" );

    // Start a screen right after to hide the console.
    shell.beginScreen( &button, 100 );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

void buttonClick( ShellminatorScreen* screen ){
    // We need the parent terminal later for redraw request.
    Shellminator* parent;

    // Extract the parent terminal's reference from the screen object.
    parent = screen -> getParent();

    // If no parent found, we have to return.
    if( parent == NULL ){
        return;
    }

    // Toggling the toggle variable
    toggle = !toggle;

    // Change color according to toggle.
    if( toggle ){
        button.setColor( Shellminator::GREEN );
    }
    else{
        button.setColor( Shellminator::RED );
    }

    // This is very important! We have to request a redraw
    // each time, when we modified something on the GUI!
    parent -> requestRedraw();
}