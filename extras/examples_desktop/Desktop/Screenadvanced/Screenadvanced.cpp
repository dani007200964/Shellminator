
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
//#include "Shellminator-Screen.hpp"
//#include "Shellminator-Buttons.hpp"
//#include "Shellminator-PlotModule.hpp"
#include "Shellminator-GUI.hpp"
#include <math.h>


// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

#define TERMINAL_BUFFER_SIZE 500
uint8_t terminalBuffer[ TERMINAL_BUFFER_SIZE ];
float scaler = 0.0;

// The data array will contain this much points.
#define NUMBER_OF_DATA_POINTS 15

// Array of data points for the plot.
float sinePoints[ NUMBER_OF_DATA_POINTS ];

void startButtonEvent( ShellminatorScreen* parent );
void stopButtonEvent( ShellminatorScreen* parent );
void reloadButtonEvent( ShellminatorScreen* parent );

// Create a class named layout. This is inherited from the ShellminatorScreen class,
// this way it can be added to the terminal as a screen.
class layout : public ShellminatorScreen{

public:

    // Override the base draw function with our custom one.
    void draw() override;

    void update( int width_p, int  height_p ) override;

    // Override the base init function with our custom one.
    void init( Shellminator* parent_p, Stream* channel_p )override;

    // Create the ShellminatorPlot object.
    ShellminatorPlot plot;

private:

    // Create the three ShellminatorButton object.
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

    plot.draw();

}

void layout::update( int width_p, int  height_p ){

    // If the reference to the caller terminal is invalid,
    // we can not continue.
    if( parent == NULL ){
        return;
    }

    height = height_p;
    width = width_p;

    startButton.setOrigin( 1, 1 );
    startButton.update( width_p / 2, 1 );

    stopButton.setOrigin( startButton.right(), 1 );
    stopButton.update( width_p - startButton.right(), 1 );

    reloadButton.setOrigin( 1, stopButton.down() );
    reloadButton.update( width_p, 3 );

    plot.setOrigin( 1, reloadButton.down() );
    plot.update( width_p / 2, height_p - reloadButton.down() );

}

void layout::init( Shellminator* parent_p, Stream* channel_p ){

    // It will be used to configure the button events.
    Shellminator::shellEvent_t buttonEvent;

    // Save the reference of the caller terminal.
    parent = parent_p;
    channel = channel_p;

    // Construct the three button objects.
    startButton = ShellminatorButton( "Start" );
    stopButton = ShellminatorButton( "Stop" );
    reloadButton = ShellminatorButton( "Reload" );

    // Construct the plot object.
    plot = ShellminatorPlot( sinePoints, NUMBER_OF_DATA_POINTS, "Sine", Shellminator::RED );

    // Initialise the button objects.
    startButton.init( parent, channel );
    stopButton.init( parent, channel );
    reloadButton.init( parent, channel );

    // Initialise the plot objects.
    plot.init( parent, channel );

    // Set the button event for the start button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'s';
    startButton.attachEvent( buttonEvent );
    startButton.attachTriggerFunction( startButtonEvent );

    // Set the button event for the stop button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';
    stopButton.attachEvent( buttonEvent );
    stopButton.attachTriggerFunction( stopButtonEvent );

    // Set the button event for the reload button.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'R';
    reloadButton.attachEvent( buttonEvent );
    reloadButton.attachTriggerFunction( reloadButtonEvent );

    startButton.setColor( Shellminator::GREEN );
    stopButton.setColor( Shellminator::RED );
    reloadButton.setColor( Shellminator::YELLOW );

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


        // Fill the data arrays with new data
        for( int i = 0; i < NUMBER_OF_DATA_POINTS; i++ ){

            sinePoints[i] = sin( 3.14159265358979323846 * 4.0 * i / NUMBER_OF_DATA_POINTS + millis() / 5000.0 ) * scaler;

        }

        // Process the new data.
        shell.update();



    }

    return 0;

}

void startButtonEvent( ShellminatorScreen* parent ){
    scaler = 1.0;
}

void stopButtonEvent( ShellminatorScreen* parent ){
    scaler = 0.0;
}

void reloadButtonEvent( ShellminatorScreen* parent ){
    splitLayout.plot.setColor( Shellminator::GREEN );
}