
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
#include "GUI/Shellminator-List-Detailed.hpp"
#include "GUI/Shellminator-Notification.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorNotification notification;

const char* listOptions[] = {
    "Aladdin",
    "The Iron Giant",
    "Treasure Planet"
};

const char* listDetails[] = {
    "1992. November 25.",
    "1999. June 31.",
    "2002. November 27."
};


const char* listText =  "Choose a movie to watch:";

ShellminatorListDetailed movieList( listOptions, listDetails, 3, listText );

void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );
    movieList.attachCallback( listCallback );

    shell.begin( "arnold" );
    shell.beginScreen( &movieList );


}

// Infinite loop.
void loop(){

    while( 1 ){

        shell.update();

    }


}

void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    if( selected == 0 ){
        notification.setText( "Good choice, you can watch Aladdin here:\nhttps://www.imdb.com/title/tt0103639/" );
    }
    else if( selected == 1 ){
        notification.setText( "Good choice, you can watch The Iron Giant here:\nhttps://www.imdb.com/title/tt0129167/" );
    }
    else{
        notification.setText( "Good choice, you can watch the Treasure Planet here:\nhttps://www.imdb.com/title/tt0133240/" );
    }

    parent -> swapScreen( &notification );

}