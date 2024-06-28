
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
#include "Shellminator-Commander-Interface.hpp"
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-Progress.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-List-Detailed.hpp"
#include "GUI/Shellminator-PlotModule.hpp"
#include "GUI/Shellminator-Notification.hpp"
#include "GUI/Shellminator-Level-Meter.hpp"
#include "Commander-API.hpp"
#include "math.h"


// We have to create an object from Commander class.
Commander commander;

bool calcGolden_func( char *args, CommandCaller* caller );
bool drawSine_func( char *args, CommandCaller* caller );
bool selfDestruct_func( char *args, CommandCaller* caller );
bool beep_func( char *args, CommandCaller* caller );
bool mute_func( char *args, CommandCaller* caller );
bool unmute_func( char *args, CommandCaller* caller );
bool logBattery_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "calcGolden", "Calculates the golden ratio.", calcGolden_func ),
    systemCommand( "drawSine", "Draw a sine wave.", drawSine_func ),
    systemCommand( "selfDestruct", "Initiates the self-destruct protocol.", selfDestruct_func ),
    systemCommand( "beep", "Generates a beep sound.", beep_func ),
    systemCommand( "mute", "Disables beep functionality.", mute_func ),
    systemCommand( "unmute", "Enables beep functionality.", unmute_func ),
    systemCommand( "logBattery", "Prints the battery status.", logBattery_func )
};

const char* destructListOptions[] = {
    "No",
    "Noooo",
    "I'm confused",
    "Hell yeah!"
};


const char* destructListText =  "Are you really want to initiate the self destruct protocol?";

ShellminatorList destructList( destructListOptions, 4, destructListText );

ShellminatorNotification notification;

ShellminatorLevelMeter meter( "Battery Level" );

// Create a plotter object.
ShellminatorButton destructButton( "Initiate Self Destruct" );

Shellminator::shellEvent_t destructButtonEvent;

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress;

float plotData[ 30 ] = { 0 };
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

ShellminatorPlot plot( plotData, plotDataSize, "Plot" );

// Create a Shellminator object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );

bool diagEnabled = false;

// We need a buffer to store the characters from the input.
char nameBuffer[ 30 ];


uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://www.shellminator.org/html/index.html\r\n\r\n"

;


// This function will be called, when the button is pressed.
void destructButtonClick( ShellminatorScreen* screen );

void destructListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );

void nameCallback( char* buffer, int bufferSize, Shellminator* parent ){

    parent -> channel -> println();
    if( diagEnabled ){
        Serial.println( "Initialising Commander..." );
    }
    commander.attachTree( API_tree );
    commander.init();

    if( diagEnabled ){
        Serial.print( "[ " );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
        Serial.print( "OK" );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        Serial.println( " ]" );
    }

    shell.attachCommander( &commander );

    if( diagEnabled ){
        Serial.println( "Initialising System Components..." );
    }
    destructButtonEvent.type = Shellminator::SHELL_EVENT_KEY;
    destructButtonEvent.data = (uint8_t)'y';

    destructButton.attachEvent( destructButtonEvent );
    destructButton.attachTriggerFunction( destructButtonClick );
    destructButton.setColor( Shellminator::RED );

    destructList.attachCallback( destructListCallback );

    meter.setWarningPercentage( 70.0 );
    meter.setErrorPercentage( 90.0 );

    if( diagEnabled ){
        Serial.print( "[ " );
        shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
        Serial.print( "OK" );
        shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        Serial.println( " ]" );
    }

    shell.format( &Serial, Shellminator::BOLD, Shellminator::GREEN );
    Serial.println( "System init finished!" );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    Serial.print( "To list available commands use the " );
    shell.format( &Serial, Shellminator::BACKGROUND, Shellminator::YELLOW );
    Serial.print( " help " );
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( " command." );


    // The input does not generate a new line before the callback,
    // if it is required, it needs to be done manually.
    parent -> channel -> println();

    // Echo back the name.
    parent -> channel -> print( "Welcome " );
    parent -> channel -> print( buffer );
    parent -> channel -> println( '!' );

    parent -> channel -> print( "Note - The default password is " );
    shell.format( parent -> channel, Shellminator::BOLD, Shellminator::YELLOW );
    parent -> channel -> print( "Passwod");
    shell.format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    parent -> channel -> println( "?" );

    parent -> channel -> print( "Please log out after you finished with" );
    shell.format( parent -> channel, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    parent -> channel -> print( " Ctrl-D ");
    shell.format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    parent -> channel -> println( "key!" );

    // Attach the logo.
    shell.attachLogo( logo );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    shell.begin( nameBuffer );

}

void loggingCallback( char* buffer, int bufferSize, Shellminator* parent ){
    bool elabled = false;
    parent -> channel -> println();
    if( strcmp( buffer, "y" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "yes" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "Yes" ) == 0 ){
        elabled = true;
    }
    else{
        parent -> channel -> println( "Your answer is treated as No." );
    }
    if( elabled ){
        parent -> channel -> println( "Extended logging ENABLED" );
        diagEnabled = true;
    }
    else{
        parent -> channel -> println( "Extended logging DISABLED" );
        diagEnabled = true;
    }
    parent -> input( nameBuffer, sizeof( nameBuffer ), "Please enter your name: ", nameCallback );
}

void formattingCallback( char* buffer, int bufferSize, Shellminator* parent ){
    bool elabled = false;
    parent -> channel -> println();
    if( strcmp( buffer, "y" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "yes" ) == 0 ){
        elabled = true;
    }
    else if( strcmp( buffer, "Yes" ) == 0 ){
        elabled = true;
    }
    else{
        parent -> channel -> println( "Your answer is treated as No." );
    }

    if( elabled ){
        parent -> channel -> println( "Formatting ENABLED" );
        parent -> enableFormatting = true;
    }
    else{
        parent -> channel -> println( "Formatting DISABLED" );
        parent -> enableFormatting = true;
    }
    parent -> input( nameBuffer, sizeof( nameBuffer ), "Would you like to enable boot messages? [y/n] ", loggingCallback );
}



// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();
    shell.enableFormatting = false;

    Serial.println( "Program Start!" );


    // Initialize shell object just enough to create a promt.
    // Setting an empty string as argument to skip banner printing.
    shell.begin( "" );

    shell.input( nameBuffer, sizeof( nameBuffer ), "Would you like to enable formatting? [y/n] ", formattingCallback );
    //shell.input( nameBuffer, sizeof( nameBuffer ), "Please enter your name: ", nameCallback );

    /*
    shell.enableFormatting = false;
    Serial.println( "Would you like to enable formatting?[ y/n ] " );
    if( Shellminator::waitForKey( &Serial, 'y' ) ){
        shell.enableFormatting = true;
    }

    Serial.println( "Would you like to start the system with advanced logging enabled?[ y/n ] " );
    if( Shellminator::waitForKey( &Serial, 'y' ) ){
        diagEnabled = true;
        commander.attachDebugChannel( &Serial );
    }
    */


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

/// This is an example function for the cat command
bool calcGolden_func(char *args, CommandCaller* caller ){

    double limit = 1.0;
    int i;
    int end = 400;
    uint32_t timerStart = 0;
    uint32_t period = 10;

    progress.setFormat( "t" );
    progress.setText( "Calculating..." );
    shell.beginScreen( &progress );

    i = 0;
    while( i < end ){
        if( ( millis() - timerStart ) > period ){
            i++;
            timerStart = millis();
            limit =sqrt( 1 + limit);
            progress.setPercentage( (float)i / (float)end * 100.0 );
        }
        shell.update();
    }

    caller -> print( "  golden-ratio: " );
    caller -> println( limit );

    return true;
}

/// This is an example function for the dog command
bool drawSine_func(char *args, CommandCaller* caller ){

    int i;

    for( i = 0; i < plotDataSize; i++ ){
        plotData[ i ] = sin( (float)i / (float)plotDataSize * M_PI * 2.0 );
    }

    shell.beginScreen( &plot );
    return true;

}

/// This is an example function for the sum command
bool selfDestruct_func(char *args, CommandCaller* caller ){
    shell.clear();
    shell.beginScreen( &destructButton );
    return true;
}

void destructButtonClick( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    parent -> swapScreen( &destructList );
}

void destructListCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    switch( selected ){
        case 0:
            notification.setText( "Self destruct sequence stopped!" );
            break;
        case 1:
            notification.setText( "Self destruct sequence stoooopped!" );
            break;
        case 2:
            notification.setText( "Confusion detected!\nSelf destruct sequence stopped." );
            break;
        case 3:
            notification.setText( "You doesn't have permission to\ninitiate the self destruct sequence!" );
            break;
        default:
            notification.setText( "Something went wrong with the list!" );
            break;
    }

    parent -> swapScreen( &notification );

}

bool beep_func(char *args, CommandCaller* caller ){
    if( shell.mute ){
        caller -> println( "Terminal is muted. Please unmute it!" );
        return true;
    }
    caller -> println( "Beeep!" );
    shell.beep();
    return true;
}

bool mute_func(char *args, CommandCaller* caller ){
    shell.mute = true;
    return true;
}

bool unmute_func(char *args, CommandCaller* caller ){
    shell.mute = false;
    return true;
}

bool logBattery_func(char *args, CommandCaller* caller ){
    meter.setPercentage( ( (int)millis()/1000 ) % 100 );
    shell.beginScreen( &meter );
    return true;
}