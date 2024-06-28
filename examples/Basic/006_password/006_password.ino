
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

uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.print( "What could be the password? Maybe " );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.print( "Passwod");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "?" );

    Serial.print( "Oh, and please log out after you finished with" );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    Serial.print( " Ctrl-D ");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "key!" );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );


    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
