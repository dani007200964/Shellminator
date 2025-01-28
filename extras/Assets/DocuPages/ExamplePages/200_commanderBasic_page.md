@page 200_commanderBasic_page Example 200 Commander Basic

@tableofcontents

Until now, we’ve focused on how to configure the terminal, but we haven’t yet talked about what to actually do with the commands that are entered.

Of course, you could take a simple approach and use a massive `if-else` chain in the execution function to check if the received text matches certain strings. However, once you have more than 10–15 commands, this becomes inefficient and hard to maintain. And let’s not even get started on the complications that arise if the command includes arguments.

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:500px; height:100px; margin-right: 20px;">
        <img src="commander_logo.svg" background="transparent" style="width: 100%; height: 100%;" direction="1" ></img>
    </div>
    <div>
        <p>While it’s certainly possible to handle this manually, we’re happy to tell you there’s a much better solution.
        We’ve developed a command processor called the <a href="https://www.commanderapi.org/html/index.html">Commander API</a>.
        This API is a standalone project, designed to be highly optimized while remaining user-friendly.
        It took quite a bit of effort to get it to this level, but the end result is something we think you’ll really appreciate.
        </p>
    </div>
</div>
\endhtmlonly

In the following examples, we’ll show you how to connect Shellminator with the Commander API to take your command-line functionality to the next level. Let’s get started!

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/200_commanderBasic.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly


First, we need to install the **Commander API**, just like we did with Shellminator. If you need extra guidance, the
[Commander API website](https://www.commanderapi.org/html/index.html) provides detailed instructions for installation.

Next, we include the necessary libraries:

```cpp
#include "Shellminator.hpp" // The familiar Shellminator library
#include "Shellminator-Commander-Interface.hpp" // The interface connecting Shellminator with Commander
#include "Commander-API.hpp" // The core Commander API library
```

Just like Shellminator, the Commander API operates by deriving an object that will handle tasks for us:

```cpp
// We have to create an object from the Commander class.
Commander commander;
```

## Designing the Commands
Now comes the design phase. We need to decide how many commands the system will support.
The great news is that the system can be expanded or reduced at any time (as long as backward compatibility is not an issue).
For this example, let’s define three commands: `cat`, `dog`, and `sum`. 

- `cat` and `dog` will simply print text.  
- `sum`, however, will perform a calculation—it will add two numbers if valid arguments are provided.

To implement these commands, we need to create callback functions. Similar to Shellminator, the Commander API is designed
with a non-blocking architecture, so you’ll use callbacks to define external hooks for each command:

```cpp
bool cat_func( char *args, CommandCaller* caller );
bool dog_func( char *args, CommandCaller* caller );
bool sum_func( char *args, CommandCaller* caller );
```

## Callback Function Structure
The structure of callback functions in Commander API is fixed:  
- **Return Type:** `bool`  
- **Arguments:** `char *args, CommandCaller* caller`  

You’re free to name your callback functions however you like. These callbacks will form the foundation for handling your commands efficiently.

## Setting Up the Command Tree

The next step is crucial for ensuring everything works properly. We need to tell our `Commander` object which commands it should recognize. The easiest way to do this is by creating a **command tree**. This "tree" is essentially an array where each element is a structure that contains:

1. The name of the command.
2. A short description of what the command does (this can greatly enhance the user experience).
3. The callback function that gets executed when someone runs the command.

At first, this might sound a bit abstract, but don’t worry—we’ve created a macro to make this process much simpler:

```cpp
Commander::systemCommand_t API_tree[] = {
    systemCommand("cat", "Description for cat command.", cat_func),
    systemCommand("dog", "Description for dog command.", dog_func),
    systemCommand("sum", "This function sums two numbers from the argument list.", sum_func)
};
```

The command tree must always be of type `Commander::systemCommand_t`. Each element should be defined using the `systemCommand` macro in the following format:

```cpp
systemCommand("command name", "short description", "callback function")
```

## Connecting Shellminator to Commander

Once the command tree is set up, it’s time to connect it to `Shellminator`. In the previous examples, we derived a class from the `Shellminator` class to create a terminal interface. However, when using Commander, we need to start from the `ShellminatorCommanderInterface` class instead. This class is built on top of `Shellminator` and comes pre-configured to work seamlessly with Commander. The usage is almost identical, so there’s no need to worry about any steep learning curve.

Here’s how to create the terminal object with Commander integration:

```cpp
// Create a ShellminatorCommanderInterface object and initialize it to use Serial
ShellminatorCommanderInterface shell(&Serial);
```

## Initializing Commander

For Commander to function correctly, it must be initialized. Under the hood, Commander performs some abstract but essential tasks during this initialization phase. During this time, it temporarily uses a fair amount of stack memory to create an optimized internal structure. This structure allows Commander to efficiently process commands later on.

To avoid issues with memory, it’s best to initialize Commander as early as possible in your program. Additionally, you can assign a debug channel for easier troubleshooting in case something doesn’t work as expected. Here’s how to initialize Commander:

```cpp
// Attach a debug channel for troubleshooting
commander.attachDebugChannel(&Serial);

// Attach the command tree
commander.attachTree(API_tree);

// Start the initialization phase
commander.init();
```

## Implementing the Callback Functions

The final step to complete the setup is implementing the callback functions we declared earlier. Let’s focus on the callback for the `sum` command, as it’s the most complex of the three:

```cpp
/// This is an example function for the sum command
bool sum_func(char *args, CommandCaller* caller) {
    // These variables will hold the two numbers to be summed.
    int a = 0;
    int b = 0;

    // This variable will store the result of parsing the arguments.
    int argResult;

    // This variable will store the sum of the two numbers.
    int sum = 0;

    // Parse the arguments to extract two integers.
    argResult = sscanf(args, "%d %d", &a, &b);

    // Check if the parsing was successful.
    if (argResult != 2) {
        // If parsing failed, print an error message to the response channel.
        caller->print("Argument error! Two numbers required, separated by a blank space.\r\n");

        // Stop the execution and return false.
        return false;
    }

    // Calculate the sum.
    sum = a + b;

    // Print the result.
    caller->print(a);
    caller->print(" + ");
    caller->print(b);
    caller->print(" = ");
    caller->println(sum);

    // Indicate successful execution.
    return true;
}
```

## Arguments and Caller
- `args`: This is a simple C-style string containing the arguments passed to the command (if any). For the `sum` command, it should contain two integers separated by a space.
- `caller`: This is a pointer to the Commander object that invoked this function. It acts as an interface for communicating back to the user. For example, you can print messages or errors using its `print` and `println` methods.

## What Does the sum Function Do?

1. **Parse Arguments:**  
   The function uses `sscanf` to extract two integers (`a` and `b`) from the `args` string.  
   - If the parsing fails (e.g., not enough numbers provided or invalid input), the function informs the user by printing an error message using `caller->print` and then returns `false` to indicate failure.

2. **Perform the Calculation:**  
   If the arguments are valid, the function calculates the sum of the two numbers.

3. **Output the Result:**  
   The function uses `caller->print` and `caller->println` to display the result in the following format:  
   `a + b = sum`

4. **Return the Status:**  
   Finally, the function returns `true` to indicate successful execution.

## Important Notes

- **Use `caller->print()` and `caller->println()`:** When writing output in Commander callbacks, always use these methods. They ensure the output is correctly routed to the response channel.
- **Robust Error Handling:** This function gracefully handles errors, like missing or invalid arguments, by providing helpful feedback to the user.
- Every command callback must return `true`, when the command was executed without problem. When any problem occurs, it must return with `false`.

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
    caller -> print( "Argument error! Two numbers required, separated with a blank space.\r\n" );

    // Sadly we have to stop the command execution and return.
    return false;

  }

  // Calculate the sum.
  sum = a + b;

  // Print out the result.
  caller -> print( a );
  caller -> print( " + " );
  caller -> print( b );
  caller -> print( " = " );
  caller -> println( sum );
    return true;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Buffering Example](@ref 102_buffering_page) | [Commander Basic Example](@ref 200_commanderBasic_page) |
 
</div>