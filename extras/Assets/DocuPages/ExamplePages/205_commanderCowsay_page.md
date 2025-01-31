@page 205_commanderCowsay_page Example 205 Commander Cowsay

@tableofcontents

## Getting Started with Flags in Command-Line Arguments  

In this example, we’ll explore **flags**—a special type of command-line argument. Flags are **position-independent** and don’t require a value. At first, this might seem a little strange, but not all arguments need to have an associated value. Sometimes, simply **including the flag** in the command is enough to change how the program behaves.  

A great example of this is the [cowsay](https://en.wikipedia.org/wiki/Cowsay) command in Linux, which is actually a fun little *easter egg*! We’ll be implementing a simplified version of it to see flags in action.  

## What Should Our cowsay Command Do?  
The `cowsay` command is quite flexible. Here’s how it works:  
- If no arguments are provided, it simply prints a cow saying *booo*.  
- If the user provides a text argument, the cow will say that instead.  
- Additionally, we can change the **eyes** of the cow using **flags**:  
  - `--tired` or `-t` → gives the cow tired eyes.  
  - `--wink` or `-w` → makes the cow wink.  
  - `--greedy` or `-g` → gives the cow money-shaped eyes.  

## Implementing the Argument Handling  
We won’t dive into the graphics rendering here—you can check the full implementation below. Instead, let’s focus on how we handle command-line arguments using **flag detection**.  

First, we define our arguments:  

```cpp
// The text argument is optional but must be the first argument if provided.
Argument text(args, 0);

// These are the optional flags for modifying the cow’s eyes.
Argument tired_flag(args, 't', "tired");
Argument wink_flag(args, 'w', "wink");
Argument greedy_flag(args, 'g', "greedy");
```

For flags, the only thing that matters is **whether they are present or not**—we don’t parse values for them, we just check if they exist. That’s why we use the `find()` method:  

```cpp
// Check if the flags exist in the command.
tired_flag.find();
wink_flag.find();
greedy_flag.find();
```

When we need to determine **which flag was provided**, we use the `isFound()` method. Since multiple flags can be given at once, we should establish a **priority order** to avoid unpredictable behavior. In our case, the priority (from highest to lowest) is:  

1. **Tired** (`-t`)
2. **Wink** (`-w`)
3. **Greedy** (`-g`)  
4. **Default (normal eyes, if no flag is given)**  

Now, let’s apply this logic in our implementation:  

```cpp
// The eye type is determined based on priority.
if (tired_flag.isFound()) {
    caller->print(tired_eyes);
}
else if (wink_flag.isFound()) {
    caller->print(wink_eyes);
}
else if (greedy_flag.isFound()) {
    caller->print(greedy_eyes);
}
// If no flags are present, use the default eyes.
else {
    caller->print(default_eyes);
}
```
Flags are **a simple yet powerful way** to modify a program’s behavior without requiring additional values. In this example, we used them to change our cow’s eye expression dynamically.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/205_commanderCowsay.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Here are a few <b>suggested commands</b> you might want to try in the demo above:
        </p>
    </div>
</div>
\endhtmlonly

- `cowsay "hello bello" --tired` → Displays "hello bello" with tired eyes.  
- `cowsay --tired --wink` → Shows the default text with tired eyes (not winking), since the `tired` flag has higher priority.  
- `cowsay --wink --tired` → Same as the previous one, proving that flag order doesn’t matter.  
- `cowsay -g` → Displays the cow with money-shaped eyes.  
- `cowsay 10` → The text will be "10" because the first argument is treated as a string, not a number.  
- `cowsay 10.5` → Same as above, but with a decimal number.  
- `cowsay` → Runs with the default behavior.  

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
#include "Shellminator-Commander-Interface.hpp"
#include "Commander-API.hpp"
#include "Commander-Arguments.hpp"

// Not an average looking cow for sure
const char* cow_top =    "        \\   ^__^\r\n         \\  ";
const char* cow_bottom = "\\_______\r\n            (__)\\       )\\/\\\r\n                ||----w |\r\n                ||     ||\r\n";

// Eye configurations
const char* default_eyes = "(oo)";
const char* tired_eyes = "(--)";
const char* wink_eyes = "(-O)";
const char* greedy_eyes = "($$)";

// We have to create an object from Commander class.
Commander commander;

bool cowsay_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "cowsay", "Like the Linux command. Just give it a string.\r\n\tflags: --greedy\r\n\t         --tired", cowsay_func )
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
    commander.init();

    shell.attachCommander( &commander );

    // Initialize shell object.
    shell.begin( "arnold" );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}

bool cowsay_func(char *args, CommandCaller* caller ){
    char textBuffer[ 30 ];
    int i;

    // The text argument is place dependent but optional.
    // It has to be the first argument when it is given.
    Argument text( args, 0 );

    // These are the optional flags for the command.
    Argument tired_flag( args, 't', "tired" );
    Argument wink_flag( args, 'w', "wink" );
    Argument greedy_flag( args, 'g', "greedy" );

    // Try to find if these flags can be found in the command.
    tired_flag.find();
    wink_flag.find();
    greedy_flag.find();

    caller -> print( "< " );

    // Draw the user text when it is given.
    if( text.parseString( textBuffer ) ){
        caller -> print( textBuffer );
        caller -> println( " >" );
        caller -> print( ' ' );
        for( i = strlen( textBuffer ) + 2; i > 0 ; i-- ){
            caller -> print( '-' );
        }
    }

    // Draw the default text.
    else{
        caller -> print( "booo >\r\n ------" );
    }
    caller -> println();
    

    caller -> print( cow_top );

    // The eye drawing has a priority in case multiple flags are given.
    // Check each flag from top priority and draw the corresponding eyes.
    if( tired_flag.isFound() ){
        caller -> print( tired_eyes );
    }
    else if( wink_flag.isFound() ){
        caller -> print( wink_eyes );
    }
    else if( greedy_flag.isFound() ){
        caller -> print( greedy_eyes );
    }

    // If no flags are given, draw dhe default eyes.
    else{
        caller -> print( default_eyes );
    }
    caller -> print( cow_bottom );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Position Independent Arguments Example](@ref 204_commanderPositionIndependentArg_page) | [Commander System Variables Example](@ref 206_commanderSystemVariables_page) |
 
</div>