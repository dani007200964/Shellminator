@page 204_commanderPositionIndependentArg_page Example 204 Commander Position Independent Arguments

@tableofcontents

## Handling Position-Independent Arguments

Now, let’s talk about **position-independent arguments**.  

Have you ever compiled a program using **GCC from the command line**? If so, you might already be familiar with this concept! Position-independent arguments **always come in key-value pairs**.  

In the past, argument names were often a **single letter preceded by a dash**, like `-h` for help. However, modern applications allow **more human-friendly** argument names that can be **full words**, as long as they don’t contain spaces. These longer names are usually **prefixed with two dashes** (`--`).  

For example:  
- The old-school way: `-h`
- The modern way: `--help`  

The **argument value** follows the argument name, separated by at least one **space**.  

---

## Example: Implementing a "random" Command

Let's say we need to implement a command called `random`. As the name suggests, its job is to **generate a random number**.  

The order of parameters **matters**, because swapping them could change the result. That’s why **position-independent arguments** are useful—they eliminate confusion by **explicitly naming each parameter**.  

Additionally, modern programming languages often implement `random` in **two ways**:
1. **One argument** → This is treated as the **upper limit**, with `0` as the default lower limit.
2. **Two arguments** → Both a **lower and an upper limit** are specified.  

We'll build a command that supports **both** approaches.  

## Defining Arguments

Each argument needs a **short name** (required) and an optional **long name**.  
To keep things consistent, either **use short names for all arguments or enable long names support for all arguments**.  

For simplicity:  
- The **lower limit** argument will have a **short name** `-l` and a **long name** `--low`.
- The **upper limit** argument will have a **short name** `-h` and a **long name** `--high`.  

Here’s how we define these arguments:  

```cpp
Argument min_number( args, 'l', "low" );
Argument max_number( args, 'h', "high" );
```

## Handling Different Cases

We need to check **four scenarios**:  

1. Both arguments are provided → ✅ Perfect! We can generate a random number within the range.  
2. Only the `high` argument is provided → ✅ No problem! We assume the **lower limit is 0**.  
3. Only the `low` argument is provided → ❌ This doesn't make sense—we should **inform the user** that something went wrong.  
4. Neither argument is provided → ❌ The user probably doesn’t understand how the command works, so we should **display usage instructions**.  

## Optimizing Performance

Previously, we used `parseInt()` to check if an argument was parsed correctly. However, if you **call it multiple times**, your command might run **hundreds of times slower than necessary**!  

Instead, once `parseInt()`, `parseFloat()`, or `parseString()` has been **successfully called**, the argument object **remembers its parsed value**.  

To quickly check if an argument was successfully parsed, you can **cast the argument object to a** `bool`.  

Example:  

```cpp
min_number.parseInt();
max_number.parseInt();

// Check if both parameters are found
if ((bool)min_number && (bool)max_number) {
    caller->print(random((int)min_number, (int)max_number));
    return true;
}
```

To make things a bit more **visually appealing**, we can also **add colors to error messages**! This helps **draw the user’s attention** to the important part of the message—which might even be the **solution itself**.  

```cpp
// Format the error message to guide the user in the right direction.
caller->print("Argument error! Please check ");
shell.format(caller, Shellminator::UNDERLINE, Shellminator::RED);
caller->print("random?");
shell.format(caller, Shellminator::REGULAR, Shellminator::WHITE);
caller->println(" for help!");
```

The `shell.format()` method works **seamlessly** with the `caller` argument, so you can **mix and match them** without any issues!

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/204_commanderPositionIndependentArg.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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

✅ **Basic Usage**  
- `random?` – Displays a help message explaining how to use the command.  
- `random -h 10` – Generates a random number between **0 and 10**.  
- `random --high 10` – Does the same as the previous command.  

✅ **Custom Range**  
- `random -h 10 -l 5` – Generates a random number between **5 and 10**.  
- `random -h 10 --low 5` – Same as above.  
- `random -l 5 -h 10` – Again, generates a number between **5 and 10**. This shows that **argument order doesn’t matter**!  

❌ **Error Cases**  
- `random` – **Error:** No arguments provided.  
- `random -l 5` – **Error:** Only the lower limit is given; missing the upper limit.  
- `random -l tz5 -h 10` – This is an interesting case! 🤔  

You **won't get an error**, but the lower limit won't be parsed because it's not a valid number. The code will treat it as if only the upper limit was provided, meaning the lower limit defaults to **0**.

Give them a try and see what happens!

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

bool random_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "random", "Generates a random number.\r\n\targs: --low:Minimum number\r\n\t      --high: Maximum number", random_func )
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

bool random_func(char *args, CommandCaller* caller ){
    Argument min_number( args, 'l', "low" );
    Argument max_number( args, 'h', "high" );

    min_number.parseInt();
    max_number.parseInt();

    // Check if both parameters are found
    if( (bool)min_number && (bool)max_number ){
        caller -> print( random( (int)min_number, (int)max_number ) );
        return true;
    }

    // Check if only high bound is given
    else if( (bool)max_number && !(bool)min_number ){
        caller -> print( random( (int)max_number ) );
        return true;
    }

    // Check if only low bound is given. Because this doesn't
    // makes much sense generate an error.
    else if( !(bool)max_number && (bool)min_number ){
        caller -> println( "Only low bound is given. Low bound is also neccesary!" );
        return false;
    }

    // Create some formatting on the error message to navigate
    // the user to the right direction.
    caller -> print( "Argument error! Please check " );
    shell.format( caller, Shellminator::UNDERLINE, Shellminator::RED );
    caller -> print( "random?");
    shell.format( caller, Shellminator::REGULAR, Shellminator::WHITE );
    caller -> println( " for help!" );
    return false;
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Argument Types Example](@ref 203_commanderArgumentTypes_page) | [Commander Cowsay Example](@ref 205_commanderCowsay_page) |
 
</div>