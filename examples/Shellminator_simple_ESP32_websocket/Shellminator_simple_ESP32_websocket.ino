/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.05.08
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup forShellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/

// Todo home, end not work with xterm

#include <WebSocketsServer.h>
#include <WiFi.h>
#include "esp_wifi.h"

#include "Shellminator.hpp"


#define SERVER_PORT 81

// WiFi credentials.
const char* ssid     = "DIGI-b4vC";
const char* password = "vyFJ6mU8";



WebSocketsServer webSocket = WebSocketsServer(81);

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell( &webSocket, 0 );

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"                                                              \r\n"
;


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

                shell.clear();
                shell.begin( "arnold" );
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            shell.webSocketPush( payload, length );

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
      case WStype_ERROR:      
      case WStype_FRAGMENT_TEXT_START:
      case WStype_FRAGMENT_BIN_START:
      case WStype_FRAGMENT:
      case WStype_FRAGMENT_FIN:
      break;
    }

}

void setup() {

  // Initialize Serial with 115200 baudrate.
  Serial.begin( 115200 );

  // If you using Atmega32U4, the code will wait, until
  // you open the serial port.
  while( !Serial );

  // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  // Print start message
  Serial.println( "Program begin..." );

  // WiFi configuration section
  Serial.print( "Connect to  WiFi: " );
  Serial.print( ssid );

  WiFi.mode( WIFI_STA );
  WiFi.setSleep(WIFI_PS_NONE);
  WiFi.begin( ssid, password );

  while( WiFi.status() != WL_CONNECTED ){

    delay( 1000 );
    Serial.print( "." );

  }

  //shell.beginServer();

  Serial.println( " [ OK ]" );

  Serial.println( "Connected!" );
  Serial.print( "Device IP: " );
  Serial.print( WiFi.localIP() );
  Serial.print( " at port: " );
  Serial.println( SERVER_PORT );

  // initialize shell object.
  shell.begin( "arnold" );

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop() {

  shell.update();
  webSocket.loop();

}
