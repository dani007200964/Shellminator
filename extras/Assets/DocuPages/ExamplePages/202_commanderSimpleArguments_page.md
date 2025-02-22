@page 202_commanderSimpleArguments_page Example 202 Commander Simple Arguments

@tableofcontents

## Handling Command Arguments

Every command-line system eventually faces the challenge of **parameterizing commands** —at some point, simple standalone commands aren’t enough.  

Let’s take the `sum` command from our previous example. I bet you’ve wondered:  
"Is there an easier way to extract those two numbers?"

Good news—yes, there is! But before we jump in, let’s go over a few key concepts.  

## Types of Arguments

In general, there are **three types of arguments**:  

1. **Positional** – Must appear in a specific order.  
2. **Position-independent** – Can appear anywhere in the command.  
3. **Optional** – Can be included but aren’t required.  

What’s cool is that these argument types can be **mixed**. For example, you might require two **positional** arguments at the start and allow a few **optional** ones at the end. From an implementation standpoint, **optional** and **non-positional** arguments can be handled similarly since their placement is flexible.  

## Using Positional Arguments in the sum Command 

Positional arguments are useful when:  
- The order of operations **doesn’t matter** or is **trivial** (e.g., addition, multiplication).  
- There is **only one** required argument.  

Let’s set up positional arguments for our `sum` function.  

First, you need to include the **Commander-Arguments** package:  

```cpp
#include "Commander-Arguments.hpp"
```   

For each argument, we create an `Argument` object. The constructor determines the type of argument.  

The basic syntax for a **positional argument** is:  

```cpp
Argument a(args, place);
```  

- `args` → The argument list received in the command callback.  
- `place` → The **index of the argument** (starting from `0`, left to right).  

For example, given the command:  

```bash
sum 10 20
```  

- The **0th argument** is `10`.  
- The **1st argument** is `20`.  

After configuring the argument, we need to extract its value. The **Commander** library supports three data types:  

- **int** (`parseInt()`)  
- **float** (`parseFloat()`)  
- **string** (`parseString()`)  

Since we need two integers, we’ll use `parseInt()`. The parsing functions return `true` if successful, or `false` if the argument could not be converted.  

```cpp
Argument a(args, 0);
if (!a.parseInt()) { 
    // Error handling 
    return false;  
}
```  

Now that we’ve successfully parsed the argument, we need to retrieve its value. Here’s where **C++ magic** comes into play—don’t worry, it’s simpler than it sounds!  

To get the argument value, **cast the argument object** to the desired type:  

```cpp
Serial.print((int)a);
```  

⚠️ **Important:** The cast **type** must match the **parsing method** you used. Since we used `parseInt()`, we cast the argument to `int`.  

This method makes it easy to work with **positional arguments** and helps keep command processing efficient. Stay tuned—we’ll explore **non-positional** and **optional** arguments next!

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/202_commanderSimpleArguments.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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

bool cat_func( char *args, CommandCaller* caller );
bool dog_func( char *args, CommandCaller* caller );
bool sum_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "cat", "Description for cat command.", cat_func ),
    systemCommand( "dog", "Description for dog command.", dog_func ),
    systemCommand( "sum", "This function sums two number from the argument list.", sum_func )
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

/// This is an example function for the cat command
bool cat_func(char *args, CommandCaller* caller ){

    caller -> print("Hello from cat function!\r\n");
    return true;

}

/// This is an example function for the dog command
bool dog_func(char *args, CommandCaller* caller ){

    caller -> print("Hello from dog function!\r\n");
    return true;

}

/// This is an example function for the sum command
bool sum_func(char *args, CommandCaller* caller ){
    int sum;
    Argument a( args, 0 );
    Argument b( args, 1 );

    if( !( a.parseInt() && b.parseInt() ) ){
        caller -> print( "Argument error! Two numbers required, separated with a blank space.\r\n" );
        return false;
    }
    
    // Calculate the sum.
    sum = (int)a + (int)b;
    
    // Print out the result.
    caller -> print( (int)a );
    caller -> print( " + " );
    caller -> print( (int)b );
    caller -> print( " = " );
    caller -> println( sum );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Optimizations Example](@ref 201_commanderOptimizations_page) | [Commander Argument Types Example](@ref 203_commanderArgumentTypes_page) |
 
</div>