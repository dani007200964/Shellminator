
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
*/


#include <stdio.h>

#include <fcntl.h>
#include <io.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "System.h"

#include "stdioStream.hpp"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Progress.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress( &shell );

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://github.com/dani007200964/Shellminator\r\n\r\n"

;


// Prototype to a function that will be called every time
// when you press the enter key.
void executionFunction( char* command, Shellminator* shell );


// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    stdioChannel.println( "-- Simple Progress Bar Demo --" );



    // Infinite loop.
    while( 1 ){


        int i;
        for( i = 0; i < 100; i++ ){

            progress.drawProgressBar( i, "Test progress" );
            delay( 50 );

        }

        for( i = 100; i > 0; i-- ){

            progress.drawProgressBar( i, "Test progress" );
            delay( 50 );

        }



    }

}
