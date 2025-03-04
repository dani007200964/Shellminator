@page 001_basic_page Example 001 Basic

@tableofcontents

This example shows the most basic configuration. Even though it's just 10 lines of code, you can already do quite a lot with it. We'll be building on this basic configuration in the upcoming examples. Essentially, we'll be setting up an empty Shellminator object. At this point, it will allow us to input text into the console, edit it, access command history, and search through it. Additionally, the help command will be functional, showing us the essential shortcuts needed for basic console use.

<hr>

__1.__ The first and most important thing to do is to include the library. This needs to be done in every file that will use Shellminator.
```cpp
#include "Shellminator.hpp"
```

__2.__ Next, we need to instantiate an object from the Shellminator class. During the object instantiation, we'll need to specify a channel. In this case, we'll use the default Serial for this purpose. It's important to note that we need to pass the address of the channel, so an `&` operator should precede it.
```cpp
// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );
```
It's not necessary to have only one shell object in a project. For example, if we want to make a terminal interface available both **wirelessly** and via **UART**, it's advisable to instantiate as many of these objects as we have channels. We'll delve into this further in a later example.

__3.__ Next, we need to initialize the system. First, we initialize the channel. For **Serial**, we must specify a baud rate. In general, **115200** is fast enough and a widely used value. After that, we clear the console. This can be useful if, for example, after a reset, we don't want to see remnants of the previous session. Typically, most terminal emulators handle this by scrolling up as many lines as the window height, preserving the previous information. Finally, we start the terminal with a name of our choice. It's advisable to use a short, concise name that does not exceed the length specified in the @ref SHELLMINATOR_BANNER_LEN define.
```cpp
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );

}
```

__4.__ Finally, but not least, you'll need to process the data arriving on the channel.
```cpp
// Infinite loop.
void loop(){
    // Process the new data.
    shell.update();
}
```
Shellminator is designed to never block the main program under any circumstances, so it can work without a multitasking system. However, it's important to note that if delays are introduced in the main program, it could slow down or even hang your terminal.

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>First, try out the <code>help</code> command, or if you prefer shortcuts, just type <code>?</code>. This will list the keyboard shortcuts for special operations. Don't forget to try out the arrow keys too! Since we haven't yet told the shell object what to do with the received command, it will simply echo it back to us.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/001_basic.html" style="height:500px;width:100%;border:none;display:block; margin-left:30px;"></iframe>
\endhtmlonly

## Whole Code

```cpp
/*
 * Created on June 8 2024
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Examples](@ref examples_page) | [Command Callback Example](@ref 002_commandCallback_page) |
 
</div>