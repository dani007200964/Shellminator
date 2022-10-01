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

#include <WebSocketsServer.h> // <- https://github.com/Links2004/arduinoWebSockets
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include "esp_wifi.h"

#include "Shellminator.hpp"
#include "Shellminator-Browser-Response.hpp" // <- It contains the webpage data


#define SERVER_PORT 81

// WiFi credentials.
const char* ssid     = "DIGI-b4vC";
const char* password = "vyFJ6mU8";



WebSocketsServer webSocket = WebSocketsServer(81);

WebServer server(80);

// Create a Shellminator object, and initialize it to use WiFiServer
Shellminator shell[ 3 ] = {
  Shellminator( &webSocket, 0 ),
  Shellminator( &webSocket, 1 ),
  Shellminator( &webSocket, 2 )
};

const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"
;

void handleIndex(){
  server.send_P(200, "text/html", shellminator_html_response, shellminator_html_response_len );
}

void handleXtermJs(){
  server.send_P(200, "application/javascript", shellminator_xterm_js_response, shellminator_xterm_js_response_len );
}

void handleXtermCss(){
  server.send_P(200, "text/css", shellminator_xterm_css_response, shellminator_xterm_css_response_len );
}

void handleXtermAddonWebLinks(){
  server.send_P(200, "application/javascript", shellminator_xterm_addon_web_links_js_response, shellminator_xterm_addon_web_links_js_response_len );
}


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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  if( num > 2 ){

    webSocket.sendTXT( num, "\r\nNo more connections allowed!\r\n" );
    webSocket.disconnect( num );
    return;
    
  }

  switch(type) {
  case WStype_DISCONNECTED:
  
    Serial.printf("[%u] Disconnected!\n", num);
    break;
    
  case WStype_CONNECTED:
    
    shell[num].clear();
    shell[num].drawLogo();
    shell[num].printBanner();
    break;
    
  case WStype_TEXT:
  
    Serial.printf("[%u] get Text: %s\n", num, payload);
    shell[num].webSocketPush( payload, length );
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

  //shell.beginServer();

  Serial.println( " [ OK ]" );

  Serial.println( "Connected!" );
  Serial.print( "Device IP: " );
  Serial.print( WiFi.localIP() );
  Serial.print( " at port: " );
  Serial.println( SERVER_PORT );

  shell[ 0 ].setBannerPathText( "ws-0 $" );
  shell[ 1 ].setBannerPathText( "ws-1 $" );
  shell[ 2 ].setBannerPathText( "ws-2 $" );

  // initialize shell object.
  shell[0].begin( "arnold" );
  shell[1].begin( "arnold" );
  shell[2].begin( "arnold" );

  server.on("/", handleIndex);
  server.on("/xterm.js", handleXtermJs);
  server.on("/xterm.css", handleXtermCss);
  server.on("/xterm-addon-web-links.js", handleXtermAddonWebLinks);
  server.onNotFound(handleNotFound);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop() {

  shell[ 0 ].update();
  shell[ 1 ].update();
  shell[ 2 ].update();
  server.handleClient();
  webSocket.loop();
  delay( 2 );

}
