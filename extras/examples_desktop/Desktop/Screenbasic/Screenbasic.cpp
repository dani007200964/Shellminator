
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

#include <math.h>

#include "System.h"

#include "stdioStream.hpp"

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-Screen.hpp"
#include "Shellminator-PlotModule.hpp"

// Use stdio as Channel.
stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use stdioChannel
Shellminator shell( &stdioChannel );

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://github.com/dani007200964/Shellminator\r\n\r\n"

;

/*
class teszt : public ShellminatorScreen{

public:
    void draw( Shellminator* parent, int width, int  height ) override;
    void init()override;
private:
    int cntr;
};

void teszt::draw( Shellminator* parent, int width, int  height ){
    parent -> clear();
    parent -> channel -> print( "Counter: " );
    parent -> channel -> print( cntr );
    cntr++;
    if( cntr >= 100 ){
        parent -> endScreen();
    }
}

void teszt::init(){
    cntr=0;
}


teszt X;
*/

#define NUMBER_OF_DATA_POINTS 20

// The plot will be generated from the content of this data array.
float sawtoothPoints[ NUMBER_OF_DATA_POINTS ];
float sinePoints[ NUMBER_OF_DATA_POINTS ];

class layout : public ShellminatorScreen{

public:
    void draw( Shellminator* parent, int width, int  height ) override;
    void init()override;
private:
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

    originLeftX = 1;
    originLeftY = 1;

    widthLeft = width / 2;
    heightLeft = height;

    originRightX = width / 2 + 1;
    originRightY = 1;

    widthRight = width - originRightX;
    heightRight = height;
    
    plotLeft.setOrigin( originLeftX, originLeftY );
    plotLeft.draw( parent, widthLeft, heightLeft );

    plotRight.setOrigin( originRightX, originRightY );
    plotRight.draw( parent, widthRight, heightRight );
}

void layout::init(){
    plotLeft = ShellminatorPlot( sinePoints, NUMBER_OF_DATA_POINTS, "Sine", Shellminator::RED );
    plotRight = ShellminatorPlot( sawtoothPoints, NUMBER_OF_DATA_POINTS, "Sawtooth", Shellminator::GREEN );
}

layout splitLayout;


// Main program.
int main(){

    // System init section.
    if( setlocale(LC_ALL, NULL) == NULL ){

        wprintf( L"Error setting locale!\r\n" );

    }


    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );
    shell.beginScreen( &splitLayout );

    // Infinite loop.
    while( 1 ){

        for( int i = 0; i < NUMBER_OF_DATA_POINTS; i++ ){

            sinePoints[i] = sin( 3.14159265358979323846 * 4.0 * (float)i / (float)NUMBER_OF_DATA_POINTS  ) * 3.0 + millis() / 5000.0;
            sawtoothPoints[i] = ( i%5 ) + millis() / 5000.0;

        }

        // Process the new data.
        shell.update();

    }

    return 0;

}
