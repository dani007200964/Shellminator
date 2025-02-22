@page 502_webSocketSelfHosted_page Example 502 WebSocket Self Hosted

@tableofcontents

The previous example will only work if the computer running the browser has internet access. This is almost always possible on a developer machine, but there are situations where the user's computer might not have internet access (for example, out in the desert or on Mars, the coverage might not be that great). Luckily, we’ve got this covered as well. The Shellminator library can host the same WebTerminal you used earlier through its own web server, independent of the platform (there are a few minor differences, but we’ll go over them later).

This is really useful if you have a local network that’s not connected to the internet. However, you should know that this operation can be resource-intensive, but an ESP32 or a Pico W can handle it just fine. Starting from the previous example, all you need to do is import the required library:  
```cpp
#include "Shellminator-WebServer.hpp"
```

Then, create an object from the `ShellminatorWebServerThemedOffline` class. In its constructor, you’ll need to specify a port where the web server will be available. Since it’s a regular HTML server, **port 80* is recommended.  
```cpp
// Webserver port for webpage.
#define WEBSERVER_PORT 80
ShellminatorWebServerThemedOffline htmlServer( WEBSERVER_PORT );
```

In the `init` section, start the server (optionally, we log debug messages, but that’s not mandatory):  
```cpp
htmlServer.attachDebugChannel( &Serial );
htmlServer.begin();
```

Finally, in the `loop` section, you’ll need to periodically update the web server:  
```cpp
htmlServer.update();
```

If you’ve done everything right, all that’s left is to open a new tab in your browser and enter the **device's IP** address in the **URL**. You’ll see pretty much the same interface as the WebTerminal hosted in the documentation, except for a few differences:

- You don’t need to specify the WebSocket address since the WebTerminal can extract it from the URI.
- There are no tooltips to reduce the resources the page uses.
- The webpage has been minified to consume fewer resources.

## Whole Code

```cpp
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
#include "Shellminator-WebServer.hpp"
#include "Shellminator-Websocket.hpp"

#define WEBSOCKET_PORT 443

// Webserver port for webpage and contents.
#define WEBSERVER_PORT 80

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
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

    // Attach the logo.
    shell.attachLogo( logo );

    // Enable buffering.
    shell.enableBuffering( printBuffer, printBufferSize );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );

    // initialize shell object.
    shell.begin( "arnold" );

    // Uncomment if you want to enable html server debug messages.
    .attachDebugChannel( &Serial );
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
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[WebSocket Pretty Example](@ref 501_webSocketPretty_page) | [BLE Simple Example](@ref 510_bleSimple_page) |
 
</div>