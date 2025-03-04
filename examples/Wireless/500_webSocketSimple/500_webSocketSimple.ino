
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "WiFi.h"
#include "Shellminator.hpp"
#include "Shellminator-Websocket.hpp"


#define WEBSOCKET_PORT 443


char ssid[] = "Replace With Your SSID";        // your network SSID (name)
char pass[] = "Replace With Your Password";    // your network password (use for WPA, or use as key for WEP)

ShellminatorWebSocket ws( WEBSOCKET_PORT );

// Create a Shellminator object, and initialize it to use WebSocketsServer
Shellminator shell( &ws );




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

    // initialize shell object.
    shell.begin( "arnold" );

    ws.attachDebugChannel( &Serial );
    ws.begin();


}

// Infinite loop.
void loop(){

    ws.update();
    shell.update();

    // Give some time to the other tasks on RTOS systems.
    delay( 2 );


}
