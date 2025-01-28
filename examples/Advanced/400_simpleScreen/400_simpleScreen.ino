
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
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-ScreenGrid.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-Notification.hpp"
#include "GUI/Shellminator-Level-Meter.hpp"
#include "GUI/Shellminator-Progress.hpp"


ShellminatorButton button_0( "BTN 0" );
ShellminatorButton button_1( "BTN 1" );
ShellminatorButton button_2( "BTN 2" );
ShellminatorButton button_3( "BTN 3" );
ShellminatorButton button_enter( "Enter" );
ShellminatorLevelMeter meter_1( "Audio L" );
ShellminatorLevelMeter meter_2( "Audio R" );

ShellminatorNotification notification;
ShellminatorProgress progress_1;
ShellminatorProgress progress_2;
ShellminatorProgress progress_3;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a grid 3 rows and 6 columns
ShellminatorScreenGrid grid( 3, 6 );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    stdioChannel.println( "Program Start!" );

    notification.setText( "Hello!" );

    meter_1.setPercentage( 35.0 );
    meter_2.setPercentage( 73.0 );

    progress_1.setPercentage( 30.0 );
    progress_2.setPercentage( 80.0 );
    progress_3.setPercentage( 21.0 );

    // Place button_0 to the first row and the first column.
    // If no span specified it defaults to one grid cell size.
    grid.addWidget( &button_0, 0, 0 );

    // Place button_1 to the first row and the second column.
    grid.addWidget( &button_1, 0, 1 );

    // Place button_2 to the second row and the first column.
    grid.addWidget( &button_2, 1, 0 );

    // Place button_2 to the second row and the second column.
    grid.addWidget( &button_3, 1, 1 );

    // Place button_enter to the third row and the first column.
    // Make it one cells heigh and two cell wide.
    grid.addWidget( &button_enter, 2, 0, 1, 2 );

    // Place notification to the first row and the third column.
    // Make it two cells high and one cell wide.
    grid.addWidget( &notification, 0, 2, 2, 1 );


    grid.addWidget( &progress_1, 0, 5 );
    grid.addWidget( &progress_2, 1, 5 );
    grid.addWidget( &progress_3, 2, 5 );

    grid.addWidget( &meter_1, 0, 3, 3, 1 );
    grid.addWidget( &meter_2, 0, 4, 3, 1 );


    // Initialize shell object.
    shell.begin( "arnold" );

    // Register the Screen object. The terminal will pass
    // the control to it, until the user presses the ESC button.
    shell.beginScreen( &grid );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
