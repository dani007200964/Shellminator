
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
#include "Shellminator-BLE-Stream.hpp"


// We will need a buffer to avoid flickering.
uint8_t printBuffer[ 100 ];
int printBufferSize = sizeof( printBuffer );

ShellminatorBleStream ble;

// Create a Shellminator object, and initialize it to use WebSocketsServer
Shellminator shell( &ble );

// Hash for 'Password' as password. Obviously, replace it
// when working on something sensitive.
uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };




// System init section.
void setup(){

    Serial.begin(115200);

    // Enable buffering.
    shell.enableBuffering( printBuffer, printBufferSize );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    // initialize shell object.
    shell.begin( "arnold" );

    // Attach connect and disconnect callbacks.
    ble.attachConnectCallback( userConnectedCallback );
    ble.attachDisconnectCallback( userDisconnectedCallback );

    ble.attachDebugChannel( &Serial );
    ble.begin();


}

// Infinite loop.
void loop(){

    ble.update();
    shell.update();

    // Give some time to the other tasks on RTOS systems.
    delay( 2 );


}

void userConnectedCallback( ShellminatorBleStream* socket ){
    // Print 'welcome' screen after connection.
    Serial.println( "Sending Welcome Page..." );
    shell.printLoginScreen();
}

void userDisconnectedCallback( ShellminatorBleStream* socket ){
    // In case of disconnect event, close the terminal.
    shell.logOut();
}