
/*
 * Created on June 8 2024
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
#include "Shellminator-Neofetch.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the default neofetch callback.
    shell.attachNeofetchFunc( defaultShellminatorNeofetch );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
