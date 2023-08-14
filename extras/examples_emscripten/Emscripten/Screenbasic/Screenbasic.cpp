
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
#include "Shellminator-PlotModule.hpp"
#include <math.h>


// Use stdio as Channel.
stdioStream stdioChannel;

// The data array will contain this much points.
#define NUMBER_OF_DATA_POINTS 15

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// The plots will be generated from the content of this two data array.
float sawtoothPoints[ NUMBER_OF_DATA_POINTS ];
float sinePoints[ NUMBER_OF_DATA_POINTS ];

#define TERMINAL_BUFFER_SIZE 50
uint8_t terminalBuffer[ TERMINAL_BUFFER_SIZE ];

// Create a class named layout. This is inherited from the ShellminatorScreen class,
// this way it can be added to the terminal as a screen.
class layout : public ShellminatorScreen{

public:

    // Override the base draw function with our custom one.
    void draw( int width, int  height ) override;

    // Override the base init function with our custom one.
    void init( Shellminator* parent_p )override;

private:

    // Create the two ShellminatorPlot object. We will use them side by side in out layout.
    ShellminatorPlot plotLeft;
    ShellminatorPlot plotRight;

    // It has to hold the reference of the caller terminal.
    // It has to be saved in init.
    Shellminator* parent = NULL;

};

void layout::draw( int width, int  height ){

    // If the reference to the caller terminal is invalid,
    // we can not continue.
    if( parent == NULL ){
        return;
    }

    // Size of the left plot.
    int widthLeft;
    int heightLeft;

    // Origin of the left plot.
    int originLeftX;
    int originLeftY;

    // Size of the right plot.
    int widthRight;
    int heightRight;

    // Origin of the right plot.
    int originRightX;
    int originRightY;

    // The left plot will be on the left side...
    originLeftX = 1;
    originLeftY = 1;

    widthLeft = width / 2;
    heightLeft = height;

    // The left plot will be on the right side...
    originRightX = width / 2 + 1;
    originRightY = 1;

    widthRight = width - originRightX;
    heightRight = height;

    // Set the calculated origins to the two plots.    
    plotLeft.setOrigin( originLeftX, originLeftY );
    plotRight.setOrigin( originRightX, originRightY );

    // IMPORTANT! We have to draw from left to right and from up to down.
    // For this reason we start the drawing with the left plot.
    plotLeft.draw( widthLeft, heightLeft );

    // Draw the right plot secondly.
    plotRight.draw( widthRight, heightRight );

}

void layout::init( Shellminator* parent_p ){

    // Save the reference of the caller terminal.
    parent = parent_p;

    // Construct the two plot objects.
    plotLeft = ShellminatorPlot( sinePoints, NUMBER_OF_DATA_POINTS, "Sine", Shellminator::RED );
    plotRight = ShellminatorPlot( sawtoothPoints, NUMBER_OF_DATA_POINTS, "Sawtooth", Shellminator::GREEN );

    // Initialise the plot objects.
    plotLeft.init( parent );
    plotRight.init( parent );

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

        sinePoints[i] = sin( 3.14159265358979323846 * 4.0 * (float)i / (float)NUMBER_OF_DATA_POINTS ) * 3.0 + millis() / 5000.0;
        sawtoothPoints[i] = ( i % 5 ) + millis() / 5000.0;

    }

    // Process the new data.
    shell.update();



}