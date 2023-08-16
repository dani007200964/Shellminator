
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
#include "Shellminator-IO.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-Buttons.hpp"
#include <math.h>


// Use stdio as Channel.
stdioStream stdioChannel;

// The data array will contain this much points.
#define NUMBER_OF_DATA_POINTS 15

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

#define TERMINAL_BUFFER_SIZE 50
uint8_t terminalBuffer[ TERMINAL_BUFFER_SIZE ];

// Create a class named layout. This is inherited from the ShellminatorScreen class,
// this way it can be added to the terminal as a screen.
class layout : public ShellminatorScreen{

public:

    // Override the base draw function with our custom one.
    void draw() override;

    void update( int width_p, int  height_p ) override;

    bool redrawRequest() override;

    // Override the base init function with our custom one.
    void init( Shellminator* parent_p )override;

private:

    // Create the two ShellminatorPlot object. We will use them side by side in out layout.
    ShellminatorButton startButton;
    ShellminatorButton stopButton;
    ShellminatorButton reloadButton;

    // It has to hold the reference of the caller terminal.
    // It has to be saved in init.
    Shellminator* parent = NULL;

};

void layout::draw(){

    // If the reference to the caller terminal is invalid,
    // we can not continue.
    if( parent == NULL ){
        return;
    }

    startButton.draw();
    stopButton.draw();
    reloadButton.draw();

}

void layout::update( int width_p, int  height_p ){

    // If the reference to the caller terminal is invalid,
    // we can not continue.
    if( parent == NULL ){
        return;
    }

    startButton.setOrigin( 1, 1 );
    startButton.update( width_p / 2, 1 );

    stopButton.setOrigin( startButton.right(), 1 );
    stopButton.update( width_p - startButton.right(), 1 );

    reloadButton.setOrigin( 1, stopButton.down() );
    reloadButton.update( width_p, 3 );

}

bool layout::redrawRequest(){
    bool redraw = false;
    redraw |= startButton.redrawRequest();
    redraw |= stopButton.redrawRequest();
    redraw |= reloadButton.redrawRequest();

    return redraw;
}

void layout::init( Shellminator* parent_p ){

    // It will be used to configure the button events.
    Shellminator::shellEvent_t buttonEvent;

    // Save the reference of the caller terminal.
    parent = parent_p;

    startButton = ShellminatorButton( "Start" );
    stopButton = ShellminatorButton( "Stop" );
    reloadButton = ShellminatorButton( "Reload" );

    startButton.init( parent );
    stopButton.init( parent );
    reloadButton.init( parent );

    // Set the button event for the start button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'s';
    startButton.attachEvent( buttonEvent );

    // Set the button event for the stop button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';
    stopButton.attachEvent( buttonEvent );

    // Set the button event for the reload button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'r';
    reloadButton.attachEvent( buttonEvent );

    startButton.setColor( Shellminator::GREEN );
    stopButton.setColor( Shellminator::RED );
    reloadButton.setColor( Shellminator::YELLOW );
    reloadButton.setRoundCorners();

}

// Create an object from the layout class.
layout splitLayout;




// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );

    // Try to enable buffering.
    if( !shell.enableBuffering( terminalBuffer, TERMINAL_BUFFER_SIZE ) ){
        stdioChannel.println( "Memory allocation failed for buffering!" );
    }

    // Initialize shell object.
    shell.begin( "arnold" );

    // Register the Screen object. The terminal will pass
    // the control to it, until the user presses the ESC button.
    shell.beginScreen( &splitLayout );



    // Infinite loop.
    while( 1 ){


        // Process the new data.
        shell.update();



    }

    return 0;

}
