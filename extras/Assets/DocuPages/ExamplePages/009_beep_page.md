@page 009_beep_page Example 009 Beeeeep

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Speaker.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Some people hate it when the terminal beeps, others find it useful, and there’s even a small group of users who find it oddly soothing.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/009_beeeeep.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

Of course, the choice is entirely up to you, but we provide the option to enable your terminal to make a beep sound if you ever need it.
In this example, we’ve made a small change to the [Command Callback Example](@ref 002_commandCallback_page) by adding a call to the `beep()` function at the end of the `execution_function`.

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

int beep_counter = 0;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a function, that can be called every time when a command gets entered.
void execution_function( char* command, Shellminator* caller ){
    if( beep_counter >= 3 ){
        caller -> channel -> println( "It was enough beeping for today..." );
        return;
    }
    caller -> channel -> println( "Beeeep!" );
    caller -> beep();
    beep_counter++;
}

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "It will only work, when you type something into the prompt." );
    Serial.println();

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

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Of course, you can globally disable this feature within the terminal object, so even if the <code>beep()</code> function is called, it won’t produce any sound.
        All you need to do in this case is set the <code>shell.mute</code> variable to <code>true</code>.
        </p>
    </div>
</div>
\endhtmlonly

shell.mute = true;

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Input Number Example](@ref 008_input_number_page) | [Banner Text Example](@ref 010_bannerText_page) |
 
</div>