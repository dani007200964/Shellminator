@page 510_bleSimple_page Example 510 Bluetooth Low Energy Simple

@tableofcontents

## Bluetooth Low Energy (BLE) in IoT  

Bluetooth Low Energy (BLE) is becoming increasingly popular in the world of IoT. It’s a fantastic solution for devices that need to run on battery power for extended periods, making it ideal for sensors, for example. However, one challenge with these gadgets is that they are often deployed in the field, installed in their final location, and after that, not a single wire can stick out of them. This makes configuring them—and potentially modifying their settings later on—quite difficult.  

A common feature of these devices is that they usually have at least one button. This is great because with a specific button sequence, they can be switched into configuration mode. Since we also love BLE devices, we created a class that enables all BLE-capable ESP32 boards and Arduino boards supported by ArduinoBLE to run a terminal in configuration mode.  

However, it's important to note that hosting a terminal in config mode significantly reduces battery life on such devices. So, we strongly recommend using this feature only in rare cases—maybe once or twice a year at most.  

## The BLE Terminal  

Just like the Web Terminal, we’ve also created a BLE Terminal, which you can find in the lower section of the left-side menu. Unfortunately, not all browsers support interaction with BLE devices. So far, Chrome and Edge have worked reasonably well for us. Firefox, on the other hand, blocks BLE and other external device interactions due to security concerns.  


\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Constructio.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>At this point, we’d also like to mention that running a BLE Terminal in an industrial or sensitive environment should be carefully considered, as <b>we cannot take responsibility for any issues that arise.</b>
        </p>
    </div>
</div>
\endhtmlonly


Both Chrome and Edge will only allow this feature if the **Experimental Web Platform features** setting is enabled. You can do that at: **chrome://flags/#enable-experimental-web-platform-features**

Once enabled, you’ll gain access to the browser’s Bluetooth module, which is available at: **chrome://bluetooth-internals/#adapter**

It's important to keep in mind that the BLE API used by browsers is still under development and may change over time. If you experience issues with the BLE Terminal, it might be due to a recent browser update that modified the BLE API. In that case, please report it as an issue.  

## Let's Get Started  

Now that we’ve clarified everything, let’s dive in! This implementation is very similar to the WebSocket version, with a few differences.  

First, you’ll need the Shellminator BLE software package:  
```cpp
#include "Shellminator-BLE-Stream.hpp"
```

Then, create an object from the `ShellminatorBleStream` class:  
```cpp
ShellminatorBleStream ble;
```

From this point on, usage is the same as in the WebSocket implementation, but there are some performance differences. BLE was not designed for continuous, high-speed data transfer, so you won’t get the same smooth experience as with other communication methods.  

## Best Practices for BLE  

Additionally:  
- BLE cannot transfer large text blocks in one go, so responses to commands like `help` might not arrive fully.  
- The **logo** function is not recommended with BLE, but if you do use it, keep it to a maximum of 1-2 lines.  
- The **GUI experience** over BLE is quite poor, so it's best to avoid it if possible.  

When using BLE, it’s best to stick to short **setter-getter** commands. Keep in mind that **data loss can happen at any time**, so plan accordingly.  

## Live Demo

\htmlonly
    <iframe id="demoFrame" src="ble_terminal.html" style="height:650px;width:100%;border:none;display:block;"></iframe>
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
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[WebSocket Pretty Example](@ref 501_webSocketPretty_page) | [WebSocket Self Hosted Example](@ref 502_webSocketSelfHosted_page) |
 
</div>