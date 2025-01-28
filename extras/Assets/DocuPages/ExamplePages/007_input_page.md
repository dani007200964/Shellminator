@page 007_input_page Example 007 Input

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Of course, a terminal interface isn’t very useful if it can’t bridge the gap between the user and the system. In nearly every system, there are situations where an event requires user interaction. The simplest way to handle this is by allowing the user to input text. This can be handy in a question-and-answer scenario, or even for implementing a simple yes/no query.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/007_input.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly


First, you’ll need a buffer to store the text that the user types in after pressing Enter. It’s a good idea to make the buffer large enough to fit a few words, but not so large that it uses up too much memory. Based on experience, a size of around __30 characters__ is usually sufficient for this purpose.

```cpp
// We need a buffer to store the characters from the input.
char inputBuffer[ 30 ];
```

The input works by asking the terminal to create an input field, which remains active until the user presses Enter. While the input is active, no other actions can take place. Once the user presses Enter, a callback function is triggered. This function is customizable and needs to be implemented by the user.

The callback should have a return type of `void` and the following arguments: `char* buffer`, `int bufferSize`, and `Shellminator* parent`. In the example, the callback simply outputs the text entered by the user.

```cpp
void inputCallback( char* buffer, int bufferSize, Shellminator* parent ){
    // The input does not generate a new line before the callback,
    // if it is required, it needs to be done manually.
    parent -> channel -> println();

    // Echo back the name.
    parent -> channel -> print( "Your name is: " );
    parent -> channel -> print( buffer );
}
```

The final step is to ask the terminal to create the input for us. For this, you’ll need the buffer you created earlier, its size, a short prompt to guide the user on what the system expects them to do, and, finally, the callback function.

```cpp
// Generate an input prompt.
shell.input( inputBuffer, sizeof( inputBuffer ), "Please enter your name: ", inputCallback );
```

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

void inputCallback( char* buffer, int bufferSize, Shellminator* parent ){
    // The input does not generate a new line before the callback,
    // if it is required, it needs to be done manually.
    parent -> channel -> println();

    // Echo back the name.
    parent -> channel -> print( "Your name is: " );
    parent -> channel -> print( buffer );
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
    shell.input( inputBuffer, sizeof( inputBuffer ), "Please enter your name: ", inputCallback );

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
        <lottie-player src="Lock.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Here are a few tips for working with input: If you need to collect something highly confidential from the user, you can hide the characters, similar to how passwords work. To do this, simply include an optional argument to indicate that the input should be hidden. For example, you can modify the previous example like this:
        </p>
    </div>
</div>
\endhtmlonly

```cpp
// Generate a secret input prompt.
shell.input( inputBuffer, sizeof( inputBuffer ), "Please enter your name: ", inputCallback, true );
```

Here’s another helpful tip: You can interrupt an input at any time by pressing the `Ctrl-C` key combination.

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Password Example](@ref 006_password_page) | [Input Number Example](@ref 008_input_number_page) |
 
</div>