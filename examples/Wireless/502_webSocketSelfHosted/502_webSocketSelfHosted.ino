
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


#include "WiFi.h"
#include "Shellminator.hpp"
#include "Shellminator-WebServer.hpp"
#include "Shellminator-Websocket.hpp"


#define WEBSOCKET_PORT 443

// Webserver port for webpage and contents.
#define WEBSERVER_PORT 80

char ssid[] = "Replace With Your SSID";        // your network SSID (name)
char pass[] = "Replace With Your Password";    // your network password (use for WPA, or use as key for WEP)

ShellminatorWebServerThemedOffline htmlServer( WEBSERVER_PORT );
ShellminatorWebSocket ws( WEBSOCKET_PORT );

// Create a Shellminator object, and initialize it to use WebSocketsServer
Shellminator shell( &ws );

// We will need a buffer to avoid flickering.
uint8_t printBuffer[ 100 ];
int printBufferSize = sizeof( printBuffer );

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


// Callbacks for connect and disconnect events.
void userConnectedCallback( ShellminatorWebSocket* socket );
void userDisconnectedCallback( ShellminatorWebSocket* socket );


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


    // Attach the logo.
    shell.attachLogo( logo );

    // Enable buffering.
    shell.enableBuffering( printBuffer, printBufferSize );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    // initialize shell object.
    shell.begin( "arnold" );

    htmlServer.attachDebugChannel( &Serial );
    htmlServer.begin();

    ws.attachDebugChannel( &Serial );

    // Attach connect and disconnect callbacks.
    ws.attachConnectCallback( userConnectedCallback );
    ws.attachDisconnectCallback( userDisconnectedCallback );

    ws.begin();


}

// Infinite loop.
void loop(){

    ws.update();
    shell.update();
    htmlServer.update();

    // Give some time to the other tasks on RTOS systems.
    delay( 2 );


}

void userConnectedCallback( ShellminatorWebSocket* socket ){
    // Print 'welcome' screen after connection.
    shell.printLoginScreen();
}

void userDisconnectedCallback( ShellminatorWebSocket* socket ){
    // In case of disconnect event, close the terminal.
    shell.logOut();
}