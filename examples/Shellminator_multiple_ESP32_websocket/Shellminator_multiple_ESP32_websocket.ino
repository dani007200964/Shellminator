/*
 * Created on Sept 10 2022
 *
 * Copyright (c) 2022 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.09.30
 *
 * This example shows how to use the websocket implemenation of Shellminator
 * with multiple clients.
*/

#include <WebSocketsServer.h> // <- https://github.com/Links2004/arduinoWebSockets
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include "esp_wifi.h"

#include "Shellminator.hpp"
#include "Shellminator-Browser-Response.hpp" // <- It contains the webpage data

// Websocket port for Shellminator.
#define WEBSOCKET_PORT 81

// Webserver port for webpage and contents.
#define WEBSERVER_PORT 80

// WiFi credentials.
const char* ssid     = "your-wifi-ssid";
const char* password = "your-wifi-pass";

// Create websocket object.
WebSocketsServer webSocket = WebSocketsServer( WEBSOCKET_PORT );

// Create webserver object.
WebServer server( WEBSERVER_PORT );

// Create an array of Shellminator objects, and initialize them to
// use WebSocketsServer. Each have a different ID starting from 0.
Shellminator shell[ 3 ] = {
  Shellminator( &webSocket, 0 ),
  Shellminator( &webSocket, 1 ),
  Shellminator( &webSocket, 2 )
};

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

// This function generates a response for the index page.
void handleIndex(){
  server.send_P(200, "text/html", shellminator_html_response, shellminator_html_response_len );
}

// This function generates a response for /xterm.js
void handleXtermJs(){
  server.send_P(200, "application/javascript", shellminator_xterm_js_response, shellminator_xterm_js_response_len );
}

// This function generates a response for /xterm.css
void handleXtermCss(){
  server.send_P(200, "text/css", shellminator_xterm_css_response, shellminator_xterm_css_response_len );
}

// This function generates a response for xterm-addon-web-links.js
void handleXtermAddonWebLinks(){
  server.send_P(200, "application/javascript", shellminator_xterm_addon_web_links_js_response, shellminator_xterm_addon_web_links_js_response_len );
}

// This function generates a response for everything else.
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// This function will be called on websocket event.
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  // This implementation works with multiple clients. You have to check if you
  // are in the available range of clients.
  if( num > 2 ){

    // If we ran out of available clients, send
    // the problem to the client, than close it.
    webSocket.sendTXT( num, "\r\nNo more connections allowed!\r\n" );
    webSocket.disconnect( num );
    return;

  }

  // Depending on the event, we have to decide the next action.
  switch( type ){

    // The client disconnected.
    case WStype_DISCONNECTED:

      // Currently, just print out the event.
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    // New connection.
    case WStype_CONNECTED:

      // Clear the screen and print the logo with the banner.
      shell[ num ].clear();
      shell[ num ].drawLogo();
      shell[ num ].printBanner();
      break;

    // Text data incoming
    case WStype_TEXT:

      // Print to serial, just for debug.
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // In the Shellminator-IO files, the websocket channel
      // is implemented as a circular buffer. The incoming data
      // from the clients has to be pushed to this circular buffer
      // in the websocket event.
      shell[ num ].webSocketPush( payload, length );
      break;

  }

}

void setup() {

  // Initialize Serial with 115200 baudrate.
  Serial.begin( 115200 );

  // If you using Atmega32U4, the code will wait, until
  // you open the serial port.
  while( !Serial );

  // Attach the logo.
  shell[ 0 ].attachLogo( logo );
  shell[ 1 ].attachLogo( logo );
  shell[ 2 ].attachLogo( logo );

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

  Serial.println( " [ OK ]" );

  Serial.println( "Connected!" );
  Serial.print( "Device IP: " );
  Serial.println( WiFi.localIP() );

  // Give each shell object an identifier.
  // It is only for visual purpose.
  shell[ 0 ].setBannerPathText( "ws-0 $" );
  shell[ 1 ].setBannerPathText( "ws-1 $" );
  shell[ 2 ].setBannerPathText( "ws-2 $" );

  // initialize shell objects.
  shell[0].begin( "arnold" );
  shell[1].begin( "arnold" );
  shell[2].begin( "arnold" );

  // Attach page handlers.
  server.on("/", handleIndex);
  server.on("/xterm.js", handleXtermJs);
  server.on("/xterm.css", handleXtermCss);
  server.on("/xterm-addon-web-links.js", handleXtermAddonWebLinks);
  server.onNotFound(handleNotFound);

  // Start webserver.
  server.begin();

  // Start websocket server.
  webSocket.begin();

  // Attach the webSocketEvent function to the websocket server.
  webSocket.onEvent(webSocketEvent);

}

void loop() {

  // Process everything.
  shell[ 0 ].update();
  shell[ 1 ].update();
  shell[ 2 ].update();
  server.handleClient();
  webSocket.loop();

  // Give some time to the other tasks.
  delay( 2 );

}
