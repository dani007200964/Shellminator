@page 500_webSocketSimple_page Example 500 WebSocket Simple

@tableofcontents

These days, wireless solutions are becoming more and more popular, and this trend has also made its way into the affordable embedded systems sector over the years. It all started with the ESP8266, but today we have a variety of microcontrollers and development boards available with WiFi connectivity options, such as the Raspberry Pi Pico W, ESP32, and Arduino. The Shellminator currently offers three solutions for this: WebSocket, TCP, and Bluetooth Low Energy.

Let’s start by looking at the WebSocket implementation, as it’s the easiest of the three and all you’ll need is a browser. First, you’ll need a microcontroller that supports WiFi communication. We tested the Raspberry Pi Pico W, ESP32, ESP8266, and the Arduino Uno R4 in-house, and with these, we were able to create a working terminal.

## Step 1: Import the required libraries
```cpp
#include "WiFi.h"
#include "Shellminator.hpp"
#include "Shellminator-Websocket.hpp"
```

Since we wanted the system to be available across multiple platforms, we implemented our own WebSocket driver. This is basically a WebSocket server optimized for message sending and working as a Stream, which makes it easy to integrate into the Shellminator ecosystem.

## Step 2: Choose a port for the server
You’ll need to specify a port for the server to run on. The commonly accepted WebSocket default port is 443, but it can be anything else if your requirements differ. Once you’ve chosen a port, create an object of the `ShellminatorWebSocket` class and specify the port in the constructor.
```cpp
#define WEBSOCKET_PORT 443
ShellminatorWebSocket ws( WEBSOCKET_PORT );
```

## Step 3: Create the Shellminator object and link it with the WebSocket
Now, in the constructor of the Shellminator object, you can easily link the WebSocket object as the communication channel.
```cpp
Shellminator shell( &ws );
```

## Step 4: Start the WebSocket server
At the end of the `init` section, it’s a good idea to start the WebSocket server. It’s enough to start it after everything else is initialized. Additionally, if you want, you can enable debug messages with the `attachDebugChannel` method (but be careful with this, especially in security-critical products, since it will print encryption keys).
```cpp
ws.attachDebugChannel( &Serial );
ws.begin();
```

## Step 5: Process WebSocket messages in the loop
Finally, the only thing left is to continuously process the WebSocket messages in the `loop` section. For devices managing networks, it's good practice to add at least a 2ms delay at the end of the `loop`, so the microkernel running on these devices has time to handle networking tasks.
```cpp
ws.update();
delay( 2 );
```

## Some tips for using the demo:
Once the example code is uploaded to the desired platform, it will print a few useful messages to the Serial after boot, such as the device’s IP address. You’ll need this to use the demo. The demo window will load the WebTerminal app, which we’ve included for convenience, but you can always access it separately from the flashing WebTerminal menu at the bottom of the left-side list.

For the WebTerminal to connect to your device, you need to enter the WebSocket server’s address in the **TargetIP** text field at the top right corner, in the format: `ws://DEVICE-IP:WS-PORT`. For example, in our case, it looks like this: `ws://192.168.0.223:443`.

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
        <p>It’s a bit slow and flickering, right? In this example, we just wanted to showcase the basic functionality and the framework, but don't worry, in the next example we’ll fix those aesthetic issues 😉
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
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Plot Advanced Example](@ref 309_guiPlotAdvanced_page) | [WebSocket Pretty Example](@ref 501_webSocketPretty_page) |
 
</div>