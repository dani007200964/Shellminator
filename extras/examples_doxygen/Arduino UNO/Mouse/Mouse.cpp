
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


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

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




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );

    shell.mouseBegin();

    Serial.println();
    Serial.println( "Click on the screen with multiple buttons on the mouse." );
    Serial.println( "The mouse wheel works as well." );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

    if( shell.mouseAvailable() ){

        Shellminator::mouseEvent_t mouseData = shell.mouseRead();
        Serial.print("X: ");
        Serial.print( mouseData.x );
        Serial.print("\tY: ");
        Serial.print( mouseData.y );
        Serial.print( "\t" );

        switch( mouseData.event ){

            case Shellminator::MOUSE_LEFT_PRESSED:
                Serial.println( "LM P" );
                break;

            case Shellminator::MOUSE_LEFT_RELEASED:
                Serial.println( "LM R" );
                break;

            case Shellminator::MOUSE_RIGHT_PRESSED:
                Serial.println( "RM P" );
                break;

            case Shellminator::MOUSE_RIGHT_RELEASED:
                Serial.println( "RM R" );
                break;

            case Shellminator::MOUSE_MIDDLE_PRESSED:
                Serial.println( "MM P" );
                break;

            case Shellminator::MOUSE_MIDDLE_RELEASED:
                Serial.println( "MM R" );
                break;

            case Shellminator::MOUSE_WHEEL_UP:
                Serial.println( "MW U" );
                break;

            case Shellminator::MOUSE_WHEEL_DOWN:
                Serial.println( "MW D" );
                break;

            default:
                Serial.println( "INVALID" );
                break;

        }

    }


}
