@page 201_commanderOptimizations_page Example 201 Commander Optimizations

@tableofcontents

When working with embedded systems, one of the most critical aspects to consider is memory usage. Unlike personal computers, these devices have very limited memory resources.  

Take, for example, the good old **Arduino Uno R3** —one of the most popular microcontrollers on the market even today. It has only **32KB of Flash** for program storage and a mere **2KB of dynamic (RAM) memory**. To make things even trickier, the **AVR core** inside the Uno doesn't allow direct access to program memory.  

This means that even if you declare a string as `const char*`, it will still be stored in **dynamic memory**, not in Flash. The same applies to the **ESP8266**. While the ESP8266 has significantly more RAM than the Uno R3, it’s still useful to have a way to store long strings (such as those used in terminal outputs) in **program memory** instead of consuming precious RAM.  

Based on our experience, microcontrollers built on **ARM Cortex cores** handle this automatically. The **linker** takes care of placing constants in the appropriate memory sections without requiring manual intervention.  

Let’s see how we can optimize terminal memory usage while keeping the same functionality as before.  

The first key difference is that we declare the **command tree** as a **global variable**, but we don't populate it with command data at the time of declaration (this is due to **linking constraints**). However, we **must** know in advance how many commands we will have, as we need to define the array size.  

```cpp
// We have 3 commands, so we create a command tree with 3 elements.
Commander::systemCommand_t API_tree[3];
```  

Next, at the **beginning of the initialization section**, we need to manually fill each array element with the corresponding command data. **The order doesn’t matter**, but be extra careful with **indexing errors** (accidental copy-paste mistakes can shift the indices and break things).  

Additionally, instead of using the `systemCommand` macro, we must use `systemCommand_P`. This macro is designed to work across different platforms, but this method introduces more potential issues compared to the approach described in [Example 200](@ref 200_commanderBasic_page).  

```cpp
// Initialize the Command Tree and store descriptions in program memory.
systemCommand_P(API_tree[0], "cat", "Description for cat command.", cat_func);
systemCommand_P(API_tree[1], "dog", "Description for dog command.", dog_func);
systemCommand_P(API_tree[2], "sum", "This function sums two numbers from the argument list.", sum_func);
```  

We’ve also created a **smart little macro** that helps optimize `print` and `println`-style functions in a **platform-independent** way.  

🚨 **Important:** This macro is generally meant to be used **only inside** `print` and `println` functions!  

Let’s see how it works:  

```cpp
Serial.println(__CONST_TXT__("Hooray, I’m stored in Flash memory!"));
```  

By wrapping your text inside the `__CONST_TXT__` macro, you can optimize how string literals are stored and used in your program.  

If everything is set up correctly, this should work just fine! Below, we’ve summarized the difference between the **optimized** and **non-optimized** versions:  

| **Version**       | **Program Memory Usage** | **Dynamic Memory Usage** |
|-------------------|------------------------|--------------------------|
| **Optimized**     | 10,502 bytes (34%)     | 655 bytes (31%)          |
| **Non-optimized** | 10,338 bytes (33%)     | 935 bytes (45%)          |  

By reducing **dynamic memory usage**, we free up valuable RAM, which is especially important on microcontrollers with limited resources.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/201_commanderOptimizations.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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

// We have to create an object from Commander class.
Commander commander;

bool cat_func( char *args, CommandCaller* caller );
bool dog_func( char *args, CommandCaller* caller );
bool sum_func( char *args, CommandCaller* caller );

// We have 3 commands, so create a 3 element long command tree.
Commander::systemCommand_t API_tree[ 3 ];

// Create a ShellminatorCommanderInterface object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );


// System init section.
void setup(){

    Serial.begin(115200);

    // Initialize Command Tree and place the description data in program memory.
    systemCommand_P( API_tree[ 0 ], "cat", "Description for cat command.", cat_func );
    systemCommand_P( API_tree[ 1 ], "dog", "Description for dog command.", dog_func );
    systemCommand_P( API_tree[ 2 ], "sum", "This function sums two number from the argument list.", sum_func  );

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

/// This is an example function for the cat command
bool cat_func(char *args, CommandCaller* caller ){

    caller -> print( __CONST_TXT__(  "Hello from cat function!\r\n" ) );
    return true;

}

/// This is an example function for the dog command
bool dog_func(char *args, CommandCaller* caller ){

    caller -> print( __CONST_TXT__( "Hello from dog function!\r\n" ) );
    return true;

}

/// This is an example function for the sum command
bool sum_func(char *args, CommandCaller* caller ){

    // These variables will hold the value of the
    // two numbers, that has to be summed.
    int a = 0;
    int b = 0;

    // This variable will hold the result of the
    // argument parser.
    int argResult;

    // This variable will hold the sum result.
    int sum = 0;

    argResult = sscanf( args, "%d %d", &a, &b );

    // We have to check that we parsed successfully the two
    // numbers from the argument string.
    if( argResult != 2 ){

        // If we could not parse two numbers, we have an argument problem.
        // We print out the problem to the response channel.
        caller -> print( __CONST_TXT__( "Argument error! Two numbers required, separated with a blank space.\r\n" ) );

        // Sadly we have to stop the command execution and return.
        return false;

    }

    // Calculate the sum.
    sum = a + b;

    // Print out the result.
    caller -> print( a );
    caller -> print( __CONST_TXT__( " + " ) );
    caller -> print( b );
    caller -> print( __CONST_TXT__( " = " ) );
    caller -> println( sum );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Basic Example](@ref 200_commanderBasic_page) | [Commander Simple Arguments Example](@ref 202_commanderSimpleArguments_page) |
 
</div>