@page 002_commandCallback_page Example 002 Command Callback

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>In the previous example, you saw how easy it is to bring the terminal to life, but we haven't yet established any connection between the main program and the Shellminator object. To associate functionality with events occurring during terminal use, we need hooks. Fortunately, Shellminator provides several hooks to customize its behavior according to your needs.
        </p>
    </div>
</div>
\endhtmlonly

\htmlonly
<iframe id="demoFrame" src="webExamples/002_commandCallback.html" style="height:500px;width:100%;border:none;display:block; margin-left:30px;"></iframe>
\endhtmlonly

We're making a slight modification to the code from the previous example by creating a function that we'll assign to the shell object in such a way that this function gets called every time a command arrives from the terminal.

First, you need to create this function. The return type is always `void`, and it must have two mandatory arguments: the first one `char* command` and the second `Shellminator* caller`. The name of the function is not critical; you can call it anything you like. In this case, its name will be execution_function.

```cpp
// Create a function, that can be called every time when a command gets entered.
void execution_function( char* command, Shellminator* caller ){
    // Just simply echo back the received command.
    caller -> channel -> print( "We have a command to execute: " );
    caller -> channel -> println( command );
}
```

Let's unpack what will happen when this function is called. The command typed by the user into the terminal will arrive to us as a plain C-string in the `command` argument. The `caller` argument is an interesting parameter that allows us to communicate with the Shellminator object that invoked the function. This becomes particularly useful when there are multiple Shellminator objects in the system.

Inside the function, you can do anything with the information; for simplicity's sake, we'll just print out that we received a command and also print the command itself.

It's really important to note here that we're not using the `Serial.print` method, but rather the `caller -> channel -> print` method. This isn't by accident; it allows us to ensure that when there are multiple terminals in the system, the terminal that sent the command receives our response. The usage is the same as before, just replace `.` with `->`.


We can add the functionality to our shell object by including the setup section with the attachExecFunc method.
```cpp
// System init section.
void setup(){
    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the execution function to the shell object.
    shell.attachExecFunc( execution_function );

    // Initialize shell object.
    shell.begin( "arnold" );
}
```

## Whole Code

```cpp
/*
 * Created on June 8 2024
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

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a function, that can be called every time when a command gets entered.
void execution_function( char* command, Shellminator* caller ){
    // Just simply echo back the received command.
    caller -> channel -> print( "We have a command to execute: " );
    caller -> channel -> println( command );
}

// System init section.
void setup(){
    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the execution function to the shell object.
    shell.attachExecFunc( execution_function );

    // Initialize shell object.
    shell.begin( "arnold" );

}

// Infinite loop.
void loop(){
    // Process the new data.
    shell.update();
}
```