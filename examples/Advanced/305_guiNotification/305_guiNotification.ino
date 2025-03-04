
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/



#include "math.h"

#include "Shellminator.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-Notification.hpp"



// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorNotification notification;

// Create a plotter object.
ShellminatorButton button( "Press" );

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
    
    notification.setText( "Hurray!\nYou pressed the button!" );

    parent -> swapScreen( &notification );
}