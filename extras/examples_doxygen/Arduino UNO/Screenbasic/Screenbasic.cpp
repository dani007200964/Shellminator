
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
#include "Shellminator-IO.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-PlotModule.hpp"
#include <math.h>


// The data array will contain this much points.
#define NUMBER_OF_DATA_POINTS 15

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// The plots will be generated from the content of this two data array.
float sawtoothPoints[ NUMBER_OF_DATA_POINTS ];
float sinePoints[ NUMBER_OF_DATA_POINTS ];

// Create a class named layout. This is inherited from the ShellminatorScreen class,
// this way it can be added to the terminal as a screen.
class layout : public ShellminatorScreen{

public:

    // Override the base draw function with our custom one.
    void draw( Shellminator* parent, int width, int  height ) override;

    // Override the base init function with our custom one.
    void init()override;

private:

    // Create the two ShellminatorPlot object. We will use them side by side in out layout.
    ShellminatorPlot plotLeft;
    ShellminatorPlot plotRight;

};

void layout::draw( Shellminator* parent, int width, int  height ){

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
    plotLeft.draw( parent, widthLeft, heightLeft );

    // Draw the right plot secondly.
    plotRight.draw( parent, widthRight, heightRight );

}

void layout::init(){

    // Construct the two plot objects.
    plotLeft = ShellminatorPlot( sinePoints, NUMBER_OF_DATA_POINTS, "Sine", Shellminator::RED );
    plotRight = ShellminatorPlot( sawtoothPoints, NUMBER_OF_DATA_POINTS, "Sawtooth", Shellminator::GREEN );

}

// Create an object from the layout class.
layout splitLayout;




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Try to enable buffering.
    if( shell.enableBuffering() ){
        Serial.println( "Memory allocation failed for buffering!" );
    }

    // Initialize shell object.
    shell.begin( "arnold" );

    // Register the Screen object. The terminal will pass
    // the control to it, until the user presses the ESC button.
    shell.beginScreen( &splitLayout );


}

// Infinite loop.
void loop(){

    // Fill the data arrays with new data
    for( int i = 0; i < NUMBER_OF_DATA_POINTS; i++ ){

        sinePoints[i] = sin( 3.14159265358979323846 * 4.0 * (float)i / (float)NUMBER_OF_DATA_POINTS ) * 3.0 + millis() / 5000.0;
        sawtoothPoints[i] = ( i % 5 ) + millis() / 5000.0;

    }

    // Process the new data.
    shell.update();


}
