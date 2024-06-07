
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


#include <stdio.h>

#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "System.h"

#include "stdioStream.hpp"

#include "Shellminator.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };




// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    stdioChannel.print( "What could be the password? Maybe " );
    Shellminator::setFormat_m( &stdioChannel, Shellminator::BOLD, Shellminator::YELLOW );
    stdioChannel.print( "Passwod");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );
    stdioChannel.println( "?" );

    stdioChannel.print( "Oh, and please log out after you finished with" );
    Shellminator::setFormat_m( &stdioChannel, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    stdioChannel.print( " Ctrl-D ");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );
    stdioChannel.println( "key!" );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );


    // Initialize shell object.
    shell.begin( "arnold" );



    // Infinite loop.
    while( 1 ){


        // Process the new data.
        shell.update();



    }

    return 0;

}
