
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

    // It has to hold the reference of the caller terminal.
    // It has to be saved in init.
    Shellminator* parent = NULL;

};

void layout::draw( int width, int  height ){

    Shellminator::shellEvent_t event;

    // If the reference to the caller terminal is invalid,
    // we can not continue.
    if( parent == NULL ){
        return;
    }

    if( parent -> eventAvailable() ){
        parent -> setCursorPosition( 1, 2 );
        parent -> channel -> print( "\r\033[Kevent: " );
        event = parent -> readEvent();

        if( event.type == Shellminator::SHELL_EVENT_MOUSE ){
            parent -> channel -> print( "Mouse-" );

            switch( event.eventCode ){

                case Shellminator::EVENT_CODE_MOUSE_LEFT_PRESSED:
                    parent -> channel -> print( "L Pressed" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_LEFT_RELEASED:
                    parent -> channel -> print( "L Released" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_RIGHT_PRESSED:
                    parent -> channel -> print( "R Pressed" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_RIGHT_RELEASED:
                    parent -> channel -> print( "R Released" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_MIDDLE_PRESSED:
                    parent -> channel -> print( "M Pressed" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_MIDDLE_RELEASED:
                    parent -> channel -> print( "M Released" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_WHEEL_UP:
                    parent -> channel -> print( "W Up" );
                    break;

                case Shellminator::EVENT_CODE_MOUSE_WHEEL_DOWN:
                    parent -> channel -> print( "W Down" );
                    break;

                default:
                    break;

            }

            parent -> channel -> print( " at X:" );
            parent -> channel -> print( event.x );

            parent -> channel -> print( " Y:" );
            parent -> channel -> print( event.y );

            if( event.eventCode == Shellminator::EVENT_CODE_MOUSE_LEFT_PRESSED ){
                if( event.y > 2 ){
                    parent -> setCursorPosition( event.x, event.y );
                    parent -> channel -> print( "\u2588" );
                }
            }

        }

    }

}

void layout::init( Shellminator* parent_p ){

    // Save the reference of the caller terminal.
    parent = parent_p;

    if( parent == NULL ){
        return;
    }

    parent -> setCursorPosition( 1, 1 );
    parent -> channel -> print( "Try mouse functionality." );

}

// Create an object from the layout class.
layout mouseLayout;


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
    shell.beginScreen( &mouseLayout );



}

void loop(){

    // Infinite loop.

    // Process the new data.
    shell.update();



}