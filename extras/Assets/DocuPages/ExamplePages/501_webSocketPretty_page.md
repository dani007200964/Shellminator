@page 501_webSocketPretty_page Example 501 WebSocket Pretty

@tableofcontents

As mentioned in the previous example, there’s definitely room for improvement when it comes to user experience. For example, why does the terminal flicker when you start typing? We touched on this issue in the Buffering example as well, but it’s much more noticeable here compared to using the Serial class—any idea why? The reason is that WebSocket frames have overhead, whereas Serial doesn’t. This means that when we send a single character over WebSocket, it doesn’t just move one byte; it moves a lot more since it also encrypts, packages, etc. If you're curious, you can read more about it [here](https://datatracker.ietf.org/doc/html/rfc6455).

Another important aspect for remote devices is that not everyone should have access to it. It’s not necessarily a bad thing if it's password-protected. This is actually pretty easy to do, and we showed how to set it up in the Password example.

Now, the important innovation here is that we have two callbacks we can use:  
```cpp
// Callbacks for connect and disconnect events.
void userConnectedCallback(ShellminatorWebSocket* socket);
void userDisconnectedCallback(ShellminatorWebSocket* socket);
```

These callbacks will be triggered when a user connects or disconnects. The names of the callbacks don’t matter, but the return type and the argument should be exactly the same. You need to attach these callbacks to the WebSocket object like this:  
```cpp
// Attach connect and disconnect callbacks.
ws.attachConnectCallback(userConnectedCallback);
ws.attachDisconnectCallback(userDisconnectedCallback);
```

Then, we implement the actual logic inside the callbacks:  
```cpp
void userConnectedCallback(ShellminatorWebSocket* socket) {
    // Print 'welcome' screen after connection.
    shell.printLoginScreen();
}

void userDisconnectedCallback(ShellminatorWebSocket* socket) {
    // In case of disconnect event, close the terminal.
    shell.logOut();
}
```

Shellminator has a `printLoginScreen` method designed specifically for the state after a wireless connection is established. It doesn’t matter whether there’s a logo or not, or if it’s password-protected or not—it's a good idea to call this method inside the connect callback.

Inside the disconnect callback, you absolutely must call the `logOut` method. Otherwise, if the connection drops due to a network issue, the terminal will stay logged in. This is very dangerous and could easily be exploited. 

## Live Demo

\htmlonly
    <iframe id="demoFrame" src="web_xterm_themed.html" style="height:650px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>It's worth noting that once you click the <b>Open</b> button, the terminal will redraw the logo and ask for the password every time. You can also try logging out using the <b>CTRL-D</b> combo to see if it works.
        </p>
    </div>
</div>
\endhtmlonly

## Whole Code

```cpp
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
#include "Shellminator-Websocket.hpp"

#define WEBSOCKET_PORT 443

char ssid[] = "Replace With Your SSID";        // your network SSID (name)
char pass[] = "Replace With Your Password";    // your network password (use for WPA, or use as key for WEP)

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
|[WebSocket Simple Example](@ref 500_webSocketSimple_page) | [WebSocket Self Hosted Example](@ref 502_webSocketSelfHosted_page) |
 
</div>