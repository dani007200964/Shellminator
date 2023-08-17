
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


// std library.
#include <stdio.h>
#include <stdlib.h>

// Core System Functions.
#include "System.h"

// Contains a modified Stream class for communication.
#include "stdioStream.hpp"

// Contains Emscripten related functions.
#include <emscripten.h>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-Buttons.hpp"
#include "Shellminator-PlotModule.hpp"
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

void startButtonEvent();
void stopButtonEvent();
void reloadButtonEvent();

// Create a class named layout. This is inherited from the ShellminatorScreen class,
// this way it can be added to the terminal as a screen.
class layout : public ShellminatorScreen{

public:

    // Override the base draw function with our custom one.
    void draw() override;

    void update( int width_p, int  height_p ) override;

    // Override the base init function with our custom one.
    void init( Shellminator* parent_p )override;

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

    startButton.setOrigin( 1, 1 );
    startButton.update( width_p / 2, 1 );

    stopButton.setOrigin( startButton.right(), 1 );
    stopButton.update( width_p - startButton.right(), 1 );

    reloadButton.setOrigin( 1, stopButton.down() );
    reloadButton.update( width_p, 3 );

    plot.setOrigin( 1, reloadButton.down() );
    plot.update( width_p / 2, height_p - reloadButton.down() );

}

void layout::init( Shellminator* parent_p ){

    // It will be used to configure the button events.
    Shellminator::shellEvent_t buttonEvent;

    // Save the reference of the caller terminal.
    parent = parent_p;

    // Construct the three button objects.
    startButton = ShellminatorButton( "Start" );
    stopButton = ShellminatorButton( "Stop" );
    reloadButton = ShellminatorButton( "Reload" );

    // Construct the plot object.
    plot = ShellminatorPlot( sinePoints, NUMBER_OF_DATA_POINTS, "Sine", Shellminator::RED );

    // Initialise the button objects.
    startButton.init( parent );
    stopButton.init( parent );
    reloadButton.init( parent );

    // Initialise the plot objects.
    plot.init( parent );

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
    buttonEvent.data = (uint8_t)'r';
    reloadButton.attachEvent( buttonEvent );
    reloadButton.attachTriggerFunction( reloadButtonEvent );

    startButton.setColor( Shellminator::GREEN );
    stopButton.setColor( Shellminator::RED );
    reloadButton.setColor( Shellminator::YELLOW );
    reloadButton.setRoundCorners();

}

// Create an object from the layout class.
layout splitLayout;


// Infinite Loop.
void loop();

// Init Section.
void setup();



// Main program.
int main(){

    // Call init section.
    setup();

    // Setup the infinite loop and start it.
    emscripten_set_main_loop( loop, 0, 1 );

    // If we are lucky, the code never reaches this.
    return 0;

}

void startButtonEvent(){
    scaler = 1.0;
}

void stopButtonEvent(){
    scaler = 0.0;
}

void reloadButtonEvent(){
    splitLayout.plot.setColor( Shellminator::GREEN );
}


void setup(){

    // Init code.

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



}

void loop(){

    // Infinite loop.

    // Fill the data arrays with new data
    for( int i = 0; i < NUMBER_OF_DATA_POINTS; i++ ){

        sinePoints[i] = sin( 3.14159265358979323846 * 4.0 * i / NUMBER_OF_DATA_POINTS + millis() / 5000.0 ) * scaler;

    }

    // Process the new data.
    shell.update();



}