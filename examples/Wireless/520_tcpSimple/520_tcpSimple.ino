
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
#include "Shellminator-TcpSocket.hpp"


#define TCP_PORT 23

char ssid[] = "Replace With Your SSID";        // your network SSID (name)
char pass[] = "Replace With Your Password";    // your network password (use for WPA, or use as key for WEP)

// We will need a buffer to avoid flickering.
uint8_t printBuffer[ 100 ];
int printBufferSize = sizeof( printBuffer );

ShellminatorTcpSocket tcp( TCP_PORT );

// Create a Shellminator object, and initialize it to use WebSocketsServer
Shellminator shell( &tcp );

// Hash for 'Password' as password. Obviously, replace it
// when working on something sensitive.
uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };

// Shellminator logo.
const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"

;




// System init section.
void setup(){

    Serial.begin(115200);

    WiFi.begin(ssid, pass);

    // Attempt to connect to WiFi network:
    Serial.print("Attempting to connect to Network");
    while( WiFi.status() != WL_CONNECTED ){
        Serial.print( '.' );
        delay( 1000 );
    }
    Serial.print("Connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Enable buffering.
    shell.enableBuffering( printBuffer, printBufferSize );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    // initialize shell object.
    shell.begin( "arnold" );

    // Attach connect and disconnect callbacks.
    tcp.attachConnectCallback( userConnectedCallback );
    tcp.attachDisconnectCallback( userDisconnectedCallback );

    tcp.attachDebugChannel( &Serial );
    tcp.begin();


}

// Infinite loop.
void loop(){

    tcp.update();
    shell.update();

    // Give some time to the other tasks on RTOS systems.
    delay( 2 );


}

void userConnectedCallback( ShellminatorTcpSocket* socket ){
    // Print 'welcome' screen after connection.
    shell.printLoginScreen();
}

void userDisconnectedCallback( ShellminatorTcpSocket* socket ){
    // In case of disconnect event, close the terminal.
    shell.logOut();
}