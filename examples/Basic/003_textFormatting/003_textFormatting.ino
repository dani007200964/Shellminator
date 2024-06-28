
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




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Print some formatted text. Firstly, we need to specify the format,
    // then print the text as usual.
    shell.format( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.println( "Is it bold? Is it yellow?");

    // Finally, we need to set back the format to default.
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    // Some other example.
    shell.format( &Serial, Shellminator::BLINKING, Shellminator::CYAN );
    Serial.println( "Can it blink?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::UNDERLINE, Shellminator::RED );
    Serial.println( "Underline?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::ITALIC, Shellminator::GREEN );
    Serial.println( "Italic?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::ITALIC, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.println( "Multiple?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    // How to disable formatting?
    shell.enableFormatting = false;
    shell.format( &stdioChannel, Shellminator::ITALIC, Shellminator::GREEN );
    stdioChannel.println( "Disable formatting?");
    shell.format( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );


}

// Infinite loop.
void loop(){



}
