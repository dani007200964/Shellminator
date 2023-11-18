
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

#include "Shellminator-Colorizer.hpp"


// Use stdio as Channel.
stdioStream stdioChannel;



// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }

    int i;
    ShellminatorColorizer defaultColorizer;

    // Start with space
    char msg1[] = " start with space";
    defaultColorizer.reset();
    stdioChannel.print( "\033[0m" );

    stdioChannel.print( "Input:  " );
    stdioChannel.println( msg1 );

    stdioChannel.print( "Result: " );
    for( i = 0; i < strlen( msg1 ); i++ ){
        defaultColorizer.printChar( &stdioChannel, msg1[ i ] );
    }
    stdioChannel.println();
    stdioChannel.println();

    // argument short
    char msg2[] = "cmd -a -b 10 -cat -!";
    defaultColorizer.reset();
    stdioChannel.print( "\033[0m" );

    stdioChannel.print( "Input:  " );
    stdioChannel.println( msg2 );

    stdioChannel.print( "Result: " );
    for( i = 0; i < strlen( msg2 ); i++ ){
        defaultColorizer.printChar( &stdioChannel, msg2[ i ] );
    }
    stdioChannel.println();
    stdioChannel.println();

    stdioChannel.print( "\033[0m" );

    // argument long
    char msg3[] = "cmd --a --bo 1 --!a 1";
    defaultColorizer.reset();
    stdioChannel.print( "\033[0m" );

    stdioChannel.print( "Input:  " );
    stdioChannel.println( msg3 );

    stdioChannel.print( "Result: " );
    for( i = 0; i < strlen( msg3 ); i++ ){
        defaultColorizer.printChar( &stdioChannel, msg3[ i ] );
    }
    stdioChannel.println();
    stdioChannel.println();

    // string argument
    char msg4[] = "cmd \"cat 1\" dog";
    defaultColorizer.reset();
    stdioChannel.print( "\033[0m" );

    stdioChannel.print( "Input:  " );
    stdioChannel.println( msg4 );

    stdioChannel.print( "Result: " );
    for( i = 0; i < strlen( msg4 ); i++ ){
        defaultColorizer.printChar( &stdioChannel, msg4[ i ] );
    }
    stdioChannel.println();
    stdioChannel.println();

    // env var argument
    char msg5[] = "cmd $! $ $cat 10";
    defaultColorizer.reset();
    stdioChannel.print( "\033[0m" );

    stdioChannel.print( "Input:  " );
    stdioChannel.println( msg5 );

    stdioChannel.print( "Result: " );
    for( i = 0; i < strlen( msg5 ); i++ ){
        defaultColorizer.printChar( &stdioChannel, msg5[ i ] );
    }
    stdioChannel.println();
    stdioChannel.println();

    stdioChannel.print( "\033[0m" );
    stdioChannel.println();

    return 0;

}
