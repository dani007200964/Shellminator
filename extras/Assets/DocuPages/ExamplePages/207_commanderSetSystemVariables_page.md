@page 207_commanderSetSystemVariables_page Example 207 Commander Set System Variables

@tableofcontents

In this example, we introduce a **new function**: the `export` command.  
This command is used to **modify system variables**, just like the `export` command in **Linux systems**.  

The `export` command follows this syntax:  
```
export $VARIABLE_NAME VARIABLE_VALUE
```
- The **first argument** must be a **system variable**, using the `$VARIABLE_NAME` format (just like in our previous example).  
- The **second argument** is the **new value** we want to assign to the variable.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/207_commanderSetSystemVariables.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

**Try these steps first** 

| **Command**             | **Expected Output / Behavior**                                         | **Explanation** |
|-------------------------|------------------------------------------------------------------------|----------------|
| `echo $VERSION`         | `V1.0.2a`                                                           | Prints the initial value of `VERSION`. |
| `echo $AREA`           | `1.41`                                                              | Prints the initial value of `AREA`. |
| `echo $SPEED`          | `0`                                                                 | Prints the initial value of `SPEED`. |
| **Modifying an Integer Variable** | | |
| `export $SPEED 10`      | No output, but updates `SPEED`                                   | Assigns `10` to `SPEED`. |
| `echo $SPEED`          | `10`                                                               | Confirms that `SPEED` is now `10`. |
| **Modifying a Float Variable** | | |
| `export $AREA 24.56`    | No output, but updates `AREA`                                   | Assigns `24.56` to `AREA`. |
| `echo $AREA`           | `24.56`                                                            | Confirms that `AREA` is now `24.56`. |
| **Invalid Input (Non-Numeric Value)** | | |
| `export $AREA cat`     | `Variable needs a new value!`                                      | Error: `cat` is not a valid number. |
| `echo $AREA`           | `24.56`                                                            | Confirms that `AREA` remains unchanged. |
| **Testing Integer Precision** | | |
| `export $SPEED 10.5`    | No output, but updates `SPEED`                                   | Assigns `10.5` to `SPEED`, expecting truncation. |
| `echo $SPEED`          | `10`                                                               | Confirms that decimals were truncated (integer type). |
| **Transferring Data Between Variables** | | |
| `export $SPEED $AREA`  | No output, but updates `SPEED`                                   | Moves `AREA`'s value (`24.56`) into `SPEED`. |
| `echo $SPEED`          | `24`                                                               | Confirms that `SPEED` now holds `24` (decimal truncated). |
| **Modifying a String Variable (Not Allowed)** | | |
| `export $VERSION "cat"` | `String variable cannot be modified!`                              | Error: `VERSION` is a string and cannot be changed. |

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
bool export_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommand( "echo", "Simple echo command.", echo_func ),
    systemCommand( "export", "Simple export command: [TARGET][VALUE]", export_func )
};

// These will be the system variables.
char* VERSION = (char*)"V1.0.2a";
int SPEED = 0;
float AREA = 1.41;

// Create system variable tree.
Commander::systemVariable_t System_variables[] = {
    systemVariableString( VERSION ),
    systemVariableInt( SPEED ),
    systemVariableFloat( AREA )
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

bool export_func(char *args, CommandCaller* caller ){
    // The first argument will be the system variable itself.
    Argument variable_object( args, 0 );
    // The second argument will be the new value for the system variable.
    Argument variable_value( args, 1 );

    // This will hold the reference of the system variable (if it can be found).
    Commander::systemVariable_t* variable;

    // The parser will try to interpret the data as a system variable first.
    variable_object.parseFloat();

    // Parse the new value as a float (can be cast back to int if needed).
    variable_value.parseFloat();

    // Try to extract the reference of a system variable.
    // If it is found, `variable` will not be NULL.
    variable = variable_object.getSystemVariable();

    // Check if the first argument is a system variable.
    if( variable != NULL ){

        // Check if the found system variable is a string.
        if( variable -> data.type == Commander::VARIABLE_STRING ){
            caller -> println( "String variable can not be modified!" );
            return false;
        }

        // Check if the new value cannot be parsed.
        if( !(bool)variable_value ){
            caller -> println( "Variable needs a new value!" );
            return false;
        }

        if( variable -> data.type == Commander::VARIABLE_FLOAT ){
            // Assign the float value to the system variable.
            *( variable -> data.data.floatData ) = (float)variable_value;
            return true;
        }

        // Assign integer value by converting float to int.
        *( variable -> data.data.intData ) = (int)( (float)variable_value );
        return true;

    }

    // We are here, because the first argument was not a system variable.
    caller -> println( "Variable not found!" );
    return false;
}
```

## Function Overview
The `export_func` function allows modifying **system variables** (integers or floats) dynamically. However, **string variables cannot be modified** due to memory safety concerns.  

The function follows these main steps:
1. **Extract the system variable name** from the first argument.  
2. **Extract the new value** from the second argument.  
3. **Check if the first argument is actually a system variable**.  
4. **Verify if the system variable is modifiable** (i.e., not a string).  
5. **Check if a valid new value is provided**.  
6. **Update the system variable** (handling integers and floats separately).  

**Step-by-Step Explanation**

### 1️⃣ Extract Arguments
```cpp
// The first argument will be the system variable itself.
Argument variable_object(args, 0);

// The second argument will be the new value for the system variable.
Argument variable_value(args, 1);
```
- The function expects **two arguments**:  
  - **The first argument (** `variable_object` **)** is the name of the system variable.  
  - **The second argument (** `variable_value` **)** is the new value we want to assign.  

### 2️⃣ Declare a Variable to Hold the System Variable Reference
```cpp
// This will hold the reference of the system variable (if it can be found).
Commander::systemVariable_t* variable;
```
- We define a pointer `variable`, which will later store the reference to the system variable **if it exists**.  

### 3️⃣ Attempt to Parse Arguments as Float
```cpp
// The parser will try to interpret the data as a system variable first.
variable_object.parseFloat();

// Parse the new value as a float (can be cast back to int if needed).
variable_value.parseFloat();
```
- The function **parses both arguments as floats**, because:  
  - Floats can be easily converted to integers.
  - If the arguments **aren't numbers**, this won't break anything.  

### 4️⃣ Retrieve System Variable Reference
```cpp
// Try to extract the reference of a system variable.
// If it is found, `variable` will not be NULL.
variable = variable_object.getSystemVariable();
```
- The function **tries to retrieve the system variable** associated with the first argument.  
- If a matching variable exists, `variable` will store its reference.  
- If not, `variable` will be `NULL`.  


### 5️⃣ Check if the Variable Exists
```cpp
// Check if the first argument is a system variable.
if (variable != NULL) {
```
- If `variable` is **NULL**, it means no system variable was found.  
- If `variable` **is not NULL**, we proceed with modifying its value.  

### 6️⃣ Prevent Modification of String Variables
```cpp
// Check if the found system variable is a string.
if (variable->data.type == Commander::VARIABLE_STRING) {
    caller->println("String variable cannot be modified!");
    return false;
}
```
- **Strings cannot be modified** because:  
  - If the string is **stored in read-only memory**, modifying it **causes a crash (bus fault)**.  
  - If the string **buffer is too small**, writing a longer value can cause **buffer overflow**.  
- If the system variable is a **string**, we **abort** the function with an error message.  


### 7️⃣ Ensure the New Value is Valid
```cpp
// Check if the new value cannot be parsed.
if (!(bool)variable_value) {
    caller->println("Variable needs a new value!");
    return false;
}
```
- If **no valid new value** is provided, the function **returns an error**.  

### 8️⃣ Assign the New Value (Handling Floats and Integers Separately)
If we reach this step, we have:  
✅ A **valid system variable** (not NULL)  
✅ A **valid new value**  

Now, we **assign the new value** depending on whether the variable is a **float** or an **integer**.  

#### Handling Float Variables
```cpp
if (variable->data.type == Commander::VARIABLE_FLOAT) {
    // Assign the float value to the system variable.
    *(variable->data.data.floatData) = (float)variable_value;
    return true;
}
```
- **If the system variable is a float**, we assign the new value as a **float**.  
- The `*(variable->data.data.floatData)` syntax **dereferences the pointer** and updates the actual variable.  

#### Handling Integer Variables
```cpp
// Assign integer value by converting float to int.
*(variable->data.data.intData) = (int)((float)variable_value);
return true;
```
- **If the system variable is an integer**, we:  
  - Convert the **new float value** to an **integer** (`(int)((float)variable_value)`).  
  - Assign the new **integer value** to the variable.  


### 9️⃣ Handle the Case Where No System Variable Was Found
```cpp
// If we reach here, the first argument was not a system variable.
caller->println("Variable not found!");
return false;
```
- If **no matching system variable was found**, we print an error message and return `false`.  

## Final Summary
The function follows this logic:  

| Step | Action | Purpose |
|------|--------|---------|
| 1️⃣ | Extract arguments | Get the system variable name and new value. |
| 2️⃣ | Declare `variable` pointer | Store reference to system variable (if found). |
| 3️⃣ | Parse values as float | Allow easy conversion between int and float. |
| 4️⃣ | Get system variable reference | Check if the variable exists. |
| 5️⃣ | Verify the variable exists | Prevent errors when modifying a non-existent variable. |
| 6️⃣ | Prevent string modification | Avoid crashes and memory corruption. |
| 7️⃣ | Check for valid new value | Ensure a proper value is provided. |
| 8️⃣ | Assign new value | Modify the system variable (handling float/int separately). |
| 9️⃣ | Handle errors | Return error if no matching variable was found. |

## Final Thoughts
- **This function allows modifying integer and float system variables safely.**  
- **String variables are protected from modification** to prevent crashes.  
- **It follows a structured error-handling approach** to guide users when something goes wrong.  


<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander System Variables Example](@ref 206_commanderSystemVariables_page) | [Commander Colorizer Example](@ref 208_commanderColorizer_page) |
 
</div>