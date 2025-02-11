@page 203_commanderArgumentTypes_page Example 203 Commander Argument Types

@tableofcontents

In the previous example, we mentioned that aside from `int`, there are also `float` and `string`(char*) argument types.  

- The `float` type works just like `int`—nothing complicated there.  
- The `string` type, however, requires a little extra care because we need a **buffer** to store the parsed text.  

Here’s how to handle it:  

```cpp
// This buffer will hold the content of the string argument.
char stringBuffer[30];

// Create a positional argument.
Argument stringArg(args, 0);

// Check if the argument was parsed correctly. 
// If not, it may indicate that the buffer is too small.
if (!stringArg.parseString(stringBuffer)) {
    caller->print("Argument error! A string is required!");
    return false;
}
```  

If `parseString()` **returns `true`**, that means:
* The argument was successfully parsed.  
* The string **fits** in the buffer.  

If it **fails**, the buffer might be **too small**, or the provided argument isn't a valid string.  

This approach ensures that string arguments are safely stored and processed!

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/203_commanderArgumentTypes.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

## Quick Tip for Using String Arguments 

Here’s a **quick tip** for handling **string arguments** as a user:  

If you try the command:  

```sh
upper hello
```  

The result will be:  

```
HELLO
```  

But if you run:  

```sh
upper hello bello
```  

You’ll **still** get:  

```
HELLO
```  

Why? Because **by default, command-line arguments are separated by spaces**.  

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>If your string <b>contains spaces</b>, you need to <b>enclose it in quotation marks</b>, just like in the C programming language.  
        </p>
    </div>
</div>
\endhtmlonly

For example:  

```sh
upper "hello bello"
```  

This will correctly return:  

```
HELLO BELLO
```  

This small trick helps ensure your full string argument is passed correctly!

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

// We have to create an object from Commander class.
Commander commander;

bool upper_func( char *args, CommandCaller* caller );
bool area_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "upper", "Replaces all characters in a string to upper-case.", upper_func ),
    systemCommand( "area", "Calculates the volume of two float numbers", area_func ),
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

bool upper_func(char *args, CommandCaller* caller ){
    // In case of a string argument, a buffer is needed.
    // This buffer will store the the extracted string.
    char stringBuffer[ 30 ];

    int i;

    // Create a positional argument.
    Argument stringArg( args, 0 );

    if( !stringArg.parseString( stringBuffer ) ){
        caller -> print( "Argument error! A string is required!" );
        return false;
    }
    
    for( i = 0; i < strlen( stringBuffer ); i++ ){
        stringBuffer[ i ] = toupper( stringBuffer[ i ] );
    }

    caller -> print( "Upper case string: " );
    caller -> println( stringBuffer );
    return true;
}

bool area_func(char *args, CommandCaller* caller ){
    float area;
    Argument a( args, 0 );
    Argument b( args, 1 );

    if( !( a.parseFloat() && b.parseFloat() ) ){
        caller -> print( "Argument error! Two float numbers are required, separated with a blank space.\r\n" );
        return false;
    }
    
    // Calculate the area.
    area = (float)a * (float)b;
    
    // Print out the result.
    caller -> print( (float)a );
    caller -> print( " * " );
    caller -> print( (float)b );
    caller -> print( " = " );
    caller -> println( area );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Simple Arguments Example](@ref 202_commanderSimpleArguments_page) | [Commander Position Independent Arguments Example](@ref 204_commanderPositionIndependentArg_page) |
 
</div>