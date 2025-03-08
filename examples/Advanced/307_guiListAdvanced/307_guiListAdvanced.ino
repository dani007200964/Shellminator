
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"
#include "GUI/Shellminator-List-Detailed.hpp"
#include "GUI/Shellminator-Notification.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorNotification notification;

// Generate a list of options
const char* listOptions[] = {
    "Aladdin",
    "The Iron Giant",
    "Treasure Planet"
};

// Generate a list of details
// The order must match with the options
const char* listDetails[] = {
    "1992. November 25.",
    "1999. June 31.",
    "2002. November 27."
};

// Simple instructions text.
const char* listText =  "Choose a movie to watch:";

// Create a detailed list.
ShellminatorListDetailed movieList( listOptions, listDetails, 3, listText );

// Callback for the list.
void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );
    movieList.attachCallback( listCallback );

    shell.begin( "arnold" );
    shell.beginScreen( &movieList );


}

// Infinite loop.
void loop(){


    shell.update();


}

void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    // Generate a notification based on the answer.
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