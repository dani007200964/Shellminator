
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




// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    // Print some formatted text. Firstly, we need to specify the format,
    // then print the text as usual.
    Shellminator::setFormat_m( &stdioChannel, Shellminator::BOLD, Shellminator::YELLOW );
    stdioChannel.println( "Is it bold? Is it yellow?");

    // Finally, we need to set back the format to default.
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

    // Some other example.
    Shellminator::setFormat_m( &stdioChannel, Shellminator::BLINKING, Shellminator::CYAN );
    stdioChannel.println( "Can it blink?");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

    Shellminator::setFormat_m( &stdioChannel, Shellminator::UNDERLINE, Shellminator::RED );
    stdioChannel.println( "Underline?");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

    Shellminator::setFormat_m( &stdioChannel, Shellminator::ITALIC, Shellminator::GREEN );
    stdioChannel.println( "Italic?");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

    Shellminator::setFormat_m( &stdioChannel, Shellminator::ITALIC, Shellminator::BOLD, Shellminator::UNDERLINE, Shellminator::YELLOW );
    stdioChannel.println( "Multiple?");
    Shellminator::setFormat_m( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );



    // Infinite loop.
    while( 1 ){





    }

    return 0;

}
