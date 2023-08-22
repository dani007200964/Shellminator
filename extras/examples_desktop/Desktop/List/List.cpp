
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
#include "Shellminator-GUI.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

const char* listOptions[] ={
    "Arnold",
    "Arnie",
    "T-100"
};

ShellminatorList list( listOptions, 3 );


void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent );


// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );
    list.attachCallback( listCallback );
    shell.beginScreen( &list );



    // Infinite loop.
    while( 1 ){


        // Process the new data.
        shell.update();



    }

    return 0;

}

void listCallback( const char* options[], int optionsSize, int selected, ShellminatorScreen* parent ){
    stdioChannel.print( "List callback with mask: " );
    stdioChannel.println( selected );

    shell.setBannerText( options[ selected ] );

}