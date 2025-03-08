@page 206_commanderSystemVariables_page Example 206 Commander System Variables

@tableofcontents

Commander isn’t just for handling commands—it can also manage **system variables**. The idea is inspired by **Linux environment variables**, but there are a few key differences.  

There are many parameters in a system that are useful to **query dynamically**. System variables can also be used directly as **arguments** for commands. In Commander, we can define three types of system variables:  
- **int** (integer values)  
- **float** (decimal values)  
- **string** (text values)  

The advantage of **int** and **float** types is that they can be **modified from the terminal** with simple commands. However, **modifying string variables at runtime is risky**, as improper handling can cause **memory overflow or bus faults**. Because of this, it’s best to **avoid modifying string variables dynamically**.  

System variables are stored in a **system variable tree**, just like commands are stored in a tree. Here’s how we define them:  

```cpp
// These will be the system variables.
char* VERSION = (char*)"V1.0.2a"; // Simple firmware version string.
int MILLIS = 0; // Stores milliseconds since boot.
float BATTERY = 100.0; // Stores battery percentage.

// Create system variable tree.
Commander::systemVariable_t System_variables[] = {
    // MILLIS variable is an int.
    systemVariableInt(MILLIS),

    // VERSION variable is a string.
    systemVariableString(VERSION),

    // BATTERY variable is a float.
    systemVariableFloat(BATTERY)
};
```

The setup is simple:  
1. **Define your variables** using standard C syntax.  
2. **Add them to the system variable array** using `systemVariableInt`, `systemVariableFloat`, or `systemVariableString` macros.  

⚠️ **Important:** The variable **name in your C code will be the same name used in the terminal**.  
- For example, the `BATTERY` variable in C can be accessed as `$BATTERY` in the terminal.  
- We’ll cover how to use these variables in commands later.  

It’s **highly recommended** (but not required) to use **uppercase names** for system variables, as this improves readability and follows common conventions.  

Once we’ve defined our system variables, we need to attach them to the **Commander object** using the `attachVariables()` method:  

```cpp
// Attach system variables to the commander object.
commander.attachVariables(System_variables);
```

System variables aren’t tied to a **specific object**, but rather to the **Commander class itself**. This means that if you have **multiple Commander objects** in your system, calling `attachVariables()` on **any one of them** makes the variables accessible to **all Commander instances**.  

This design helps **optimize resource usage** and allows the **Argument class** to access system variables linked to the Commander class.  
 
To see system variables in action, let’s create a simple **echo command** that prints out whatever it receives as input:  

```cpp
bool echo_func(char *args, CommandCaller* caller) {
    char textBuffer[30];

    // The text argument is required to parse system variables.
    Argument text(args, 0);

    // Try to parse string or system variables.
    if (text.parseString(textBuffer)) {
        caller->print(textBuffer);
        return true;
    }

    // If parsing fails, just print the raw input.
    caller->print(args);
    return true;
}
```

This `echo` command simply **prints back** what it receives as input. However, there’s a **twist**:  
- It **first tries to interpret the input** as a **string argument**.
- This is important because **the Argument object can access system variables** from Commander.
- Just for backup, if the string argument parsing had a problem, it will just print out the content in **args**.

## Try These Examples in the Demo

| Command                | Output                       | Explanation |
|------------------------|------------------------------|-------------|
| `echo VERSION`         | `VERSION`                    | Just prints the input as-is. |
| `echo $VERSION`        | `V1.0.2a`                    | `$VERSION` gets replaced with the system variable's value. |
| `echo MILLIS`          | `MILLIS`                     | Prints the input as-is. |
| `echo $MILLIS`         | (some number)                | Prints the current value of `MILLIS`, which increases over time. |
| `echo $Millis`         | `$Millis`                    | System variables are **case-sensitive**, so this doesn’t match `MILLIS`. |
| `echo "$MILLIS"`       | `$MILLIS`                    | Since it’s inside **quotes**, it’s treated as a string, not a variable. |

## Key Takeaways
- **System variables use the** `$` **prefix** to be recognized and replaced.  
- **They are case-sensitive** (`$MILLIS` works, but `$Millis` doesn’t).  
- **Quoted arguments aren’t parsed as variables** (`"$MILLIS"` stays as-is).  
- **Dynamic variables like** `$MILLIS` **change over time**, so running the command multiple times gives different values.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/206_commanderSystemVariables.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
        caller -> print( textBuffer );
        return true;
    }

    // If parsing failes, just print out the args.
    caller -> print( args );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Cowsay Example](@ref 205_commanderCowsay_page) | [Commander Set System Variables Example](@ref 207_commanderSetSystemVariables_page) |
 
</div>