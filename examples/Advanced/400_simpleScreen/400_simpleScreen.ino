
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


ShellminatorLevelMeter meter( "Audio" );
ShellminatorButton button_1( "BTN 1" );
ShellminatorButton button_2( "BTN 2" );
ShellminatorButton button_3( "BTN 3" );

ShellminatorProgress progress_1;
ShellminatorProgress progress_2;
ShellminatorProgress progress_3;

ShellminatorNotification notification;

// Create a grid 3 rows and 3 columns
ShellminatorScreenGrid grid( 3, 3 );


uint32_t timerStart = 0;
uint32_t timerPeriod = 2000;

float progress_1_percentage = 30;
float progress_2_percentage = 40;
float progress_3_percentage = 60;
float meter_percentage = 10;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

Shellminator::shellEvent_t button_1_event;
Shellminator::shellEvent_t button_2_event;
Shellminator::shellEvent_t button_3_event;

void button_1_click( ShellminatorScreen* screen );
void button_2_click( ShellminatorScreen* screen );
void button_3_click( ShellminatorScreen* screen );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );


    // Place button_1 to the first row and the second column.
    grid.addWidget( &button_1, 0, 1 );

    // Place button_2 to the second row and the second column.
    grid.addWidget( &button_2, 1, 1 );

    // Place button_3 to the third row and the second column.
    grid.addWidget( &button_3, 2, 1 );

    // Place meter to the first row and the first column.
    // Make it three row heigh and one row wide.
    grid.addWidget( &meter, 0, 0, 3, 1 );

    // Place the progress bars in line with the buttons to the right side.
    // Also make them two collumns wide.
    grid.addWidget( &progress_1, 0, 2 );
    grid.addWidget( &progress_2, 1, 2 );
    grid.addWidget( &progress_3, 2, 2 );

    progress_1.setText( "CPU" );
    progress_2.setText( "RAM" );
    progress_3.setText( "UPDATE" );
    progress_3.setFormat( "t" );

    progress_1.setColor( Shellminator::GREEN );
    progress_2.setColor( Shellminator::YELLOW );
    progress_3.setColor( Shellminator::RED );

    button_1.setColor( Shellminator::GREEN );
    button_2.setColor( Shellminator::YELLOW );
    button_3.setColor( Shellminator::RED );

    button_1_event.type = Shellminator::SHELL_EVENT_KEY;
    button_1_event.data = (uint8_t)'a';
    button_2_event.type = Shellminator::SHELL_EVENT_KEY;
    button_2_event.data = (uint8_t)'b';
    button_3_event.type = Shellminator::SHELL_EVENT_KEY;
    button_3_event.data = (uint8_t)'c';

    button_1.attachEvent( button_1_event );
    button_1.attachTriggerFunction( button_1_click );
    button_2.attachEvent( button_2_event );
    button_2.attachTriggerFunction( button_2_click );
    button_3.attachEvent( button_3_event );
    button_3.attachTriggerFunction( button_3_click );

    meter.setColor( Shellminator::CYAN );
    meter.setWarningColor( Shellminator::YELLOW );
    meter.setErrorColor( Shellminator::RED );

    meter.setWarningPercentage( 50.0 );
    meter.setErrorPercentage( 80.0 );

    // Initialize shell object.
    shell.begin( "arnold" );

    // Register the Screen object. The terminal will pass
    // the control to it, until the user presses the ESC button.
    shell.beginScreen( &grid );


}

// Infinite loop.
void loop(){

    if( ( millis() - timerStart ) > timerPeriod ){
        timerStart = millis();
        progress_1_percentage += 15.0;
        progress_2_percentage += 20.0;
        progress_3_percentage += 10.0;
        meter_percentage += 5;

        if( progress_1_percentage > 100.0 ){
            progress_1_percentage = 0;
        }
        if( progress_2_percentage > 100.0 ){
            progress_2_percentage = 0;
        }
        if( progress_3_percentage > 100.0 ){
            progress_3_percentage = 0;
        }
        if( meter_percentage > 100.0 ){
            meter_percentage = 0;
        }

        progress_1.setPercentage( progress_1_percentage );
        progress_2.setPercentage( progress_2_percentage );
        progress_3.setPercentage( progress_3_percentage );
        meter.setPercentage( meter_percentage );

        // After calculation, we have to request the shell
        // to redraw the plot.
        shell.requestRedraw();
    }

    // Process the new data.
    shell.update();


}

void button_1_click( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    
    notification.setText( "Button 1 pressed!" );
 
    parent -> swapScreen( &notification );
}

void button_2_click( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    
    notification.setText( "Button 2 pressed!" );
 
    parent -> swapScreen( &notification );
}

void button_3_click( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    
    notification.setText( "Button 3 pressed!" );
 
    parent -> swapScreen( &notification );
}