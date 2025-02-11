@page 008_input_number_page Example 008 Input Number

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Of course, there are times when you might want to ask the user for more than just text—like a number, for example. You might also need more than one input parameter. In this example, we’ll show how to ask the user for two numbers, one after the other, while also including error handling in the process.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/008_input_number.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

This example is primarily focused on demonstrating how to process the C-style string provided by the input while incorporating error handling. A useful reference for this is the description of the 
[scanf](https://cplusplus.com/reference/cstdio/scanf/) function.


Another trick here is that once one input is completed, you can seamlessly start another. Using this approach, you can even create a list-like flow to collect various types of data from the user.

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

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// We need a buffer to store the characters from the input.
char inputBuffer[ 30 ];

// These variables will store the numbers from the user input.
int width = 0;
int height = 0;

// This function will be called when the height input finishes.
void heightCallback( char* buffer, int bufferSize, Shellminator* parent ){

    // The scanf like functions will return with a number. This number represents
    // how many tokens are processed correctly. We will store the result of
    // the sscanf process to this variable.
    int scan_result;

    // Try to process one number token and save the result to the height variable.
    scan_result = sscanf( buffer, "%d", &height );

    // Check for user errors. Firstly check if the input string was empty.
    if( buffer[ 0 ] == '\0' ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // We only wanted to process exactly one number. Check if the result is not 1.
    if( scan_result != 1 ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // Also, check if the parsed number is correct. This case if it is negative or zero,
    // something went wrong.
    if( height <= 0 ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // Calculate the area
    int area = width * height;

    // Print the result
    parent -> channel -> println();
    parent -> channel -> print( "The area of the plane is: " );
    parent -> channel -> print( area );
    parent -> channel -> print( "mm2" );
}

// This function will be called when the width input finishes.
void widthCallback( char* buffer, int bufferSize, Shellminator* parent ){

    // This works exactly like the upper callback. The only difference is that,
    // when this callback finishes, it will start the next input.
    int scan_result;

    scan_result = sscanf( buffer, "%d", &width );

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( scan_result != 1 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( width <= 0 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    // Create a new prompt for the height.
    parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
}


// System init section.
void setup(){

    Serial.begin(115200);

    // Initialize shell object.
    shell.begin( "arnold" );

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Generate an input prompt.
    shell.input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );

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
|[Input Example](@ref 007_input_page) | [Beep Example](@ref 009_beep_page) |
 
</div>