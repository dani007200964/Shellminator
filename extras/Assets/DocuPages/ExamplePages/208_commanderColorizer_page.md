@page 208_commanderColorizer_page Example 208 Commander Colorizer

@tableofcontents

Modern terminals come with a super useful feature that helps guide your eyes while typing or debugging—colorization! Since we find this feature incredibly helpful, we designed **Shellminator** in a way that allows you to easily attach different colorization solutions to it.  

The easiest way to try this out is by enabling our pre-made colorizer, which is compatible with Commander-style syntax. To do this, simply create an instance of the `CommanderColorizer` object like this:  

```cpp
CommanderColorizer colorizer;
```  

Next, you need to tell your shell object to use this colorizer whenever you type a character. You can do that with the `attachColorizer` method:  

```cpp
shell.attachColorizer( &colorizer );
```  

And that’s it! Now, try typing this command in the demo below:  

```
--help -t "text" $VERSION
```  

This command doesn’t do anything particularly useful, but you’ll notice that your input is nicely colorized as you type. The colorizer also helps catch syntax errors! For example, try typing:  

```
-- help
```  

Oops! The colorizer highlights this mistake with a red background because spaces are not allowed after `--`.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/208_commanderColorizer.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
*/

#include "Shellminator.hpp"
#include "Shellminator-Commander-Interface.hpp"
#include "Commander-API.hpp"
#include "Commander-Arguments.hpp"

// We have to create an object from Commander class.
Commander commander;

// Create a colorizer object.
CommanderColorizer colorizer;

bool echo_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "echo", "Simple echo command.", echo_func )
};

// These will be the system variables.
char* VERSION = (char*)"V1.0.2a"; // Simple firmware version string.
int MILLIS = 0; // It will store the milliseconds since boot.
float BATTERY = 100.0; // It will store the battery percentage.

// Create system variable tree.
Commander::systemVariable_t System_variables[] = {
    // MILLIS variable is an int.
    systemVariableInt( MILLIS ),

    // VERSION variable is a string
    systemVariableString( VERSION ),

    // BATTERY variable is a float
    systemVariableFloat( BATTERY )
};

// Create a ShellminatorCommanderInterface object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    commander.attachDebugChannel( &Serial );
    commander.attachTree( API_tree );

    // Attach system variables to the commander object.
    commander.attachVariables( System_variables );

    commander.init();

    shell.attachCommander( &commander );

    // Attach the colorizer to the shell.
    shell.attachColorizer( &colorizer );

    // Initialize shell object.
    shell.begin( "arnold" );

}

// Infinite loop.
void loop(){

    // Calculate milliseconds since start.
    MILLIS = millis();

    // Fictive battery calculation. Just to make it change over time.
    BATTERY = ( MILLIS % 1000 ) / 10.0;

    // Process the new data.
    shell.update();

}

bool echo_func(char *args, CommandCaller* caller ){
    char textBuffer[ 30 ];

    // The text argument is required to parse system variables.
    Argument text( args, 0 );

    // Try to parse string or system variables
    if( text.parseString( textBuffer ) ){
        caller -> println( textBuffer );
        return true;
    }

    // If parsing failes, just print out the args.
    caller -> println( args );
    return true;
}
```

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Everyone has different tastes, and maybe these colors don’t quite fit your preferences. That’s totally fine—everyone sees things differently!
        But this raises an interesting question: Can we create our own custom colorizer?
        </p>
    </div>
</div>
\endhtmlonly

The answer is **yes**, of course! However, we should mention upfront that if you're new to C++, this challenge might not be the smoothest ride.
But don’t worry—we’ll leave you with some tips and guidance to help you along the way.  

Every **Colorizer** is built upon the base class DefaultColorizer. To create your own custom colorizer, you’ll need to inherit from this base class—just like we did with CommanderColorizer.  

If you want to see how it works, feel free to check out the source code in Shellminator-Commander-Interface.hpp, where you’ll find its full implementation.  


<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Set System Variables Example](@ref 207_commanderSetSystemVariables_page) | [GUI Button Example](@ref 300_guiButton_page) |
 
</div>