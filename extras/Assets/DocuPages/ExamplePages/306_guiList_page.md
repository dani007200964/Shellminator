@page 306_guiList_page Example 306 GUI List

@tableofcontents

Sometimes, a user needs to **choose from a fixed set of options**. The **ShellminatorList** class provides an easy way to handle this. It allows users to select from a **predefined list of choices** with minimal effort.  

First, include the necessary library:  

```cpp
#include "GUI/Shellminator-List.hpp"
```

Then, define the list of options the user can choose from:  

```cpp
// These options are available for selection.
const char* listOptions[] = {
    "Red Pill",
    "Blue Pill"
};
```

You’ll also need a **short instruction message** to guide the user:  

```cpp
// Instructions for the list.
const char* listText = "Choose your destiny, Neo";
```

Now, you can create a **ShellminatorList** object:  

```cpp
// Create a list.
ShellminatorList neoList(listOptions, 2, listText);
```

It’s a good idea to define a **callback function** that will be triggered when the user makes a selection. The function must have the following signature:  

```cpp
// Callback function for handling selection.
void listCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen*);
```

In the **init section**, attach this callback function to the list:  

```cpp
// Attach the callback function to the list.
neoList.attachCallback(listCallback);
```

Now, let’s define what happens when the user selects an option:  

```cpp
void listCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen) {
    Shellminator* parent = screen->getParent();
    if (parent == NULL) {
        return;
    }

    // Generate a response based on the selected option.
    if (selected == 0) {
        notification.setText("You stay in Wonderland and I show\nyou how deep the rabbit hole goes.");
    } else {
        notification.setText("The story ends, you wake up in your bed and\nbelieve whatever you want to believe.");
    }

    // Display the notification.
    parent->swapScreen(&notification);
}
```

**How It Works?**  

1. The user sees a **list of options**.  
2. When they make a selection, the **callback function is triggered**.  
3. The function **checks which option was chosen** and displays the appropriate **notification message**.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/306_guiList.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup for Shellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/

#include "Shellminator.hpp"
#include "GUI/Shellminator-List.hpp"
#include "GUI/Shellminator-Notification.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// This will be used to display the answer for the chosen answer.
ShellminatorNotification notification;

// These options are available for choose.
const char* listOptions[] = {
    "Red Pill",
    "Blue Pill"
};

// Instructions for the list.
const char* listText =  "Choose your destiny Neo";

// Create a list.
ShellminatorList neoList( listOptions, 2, listText );

// Create a callback for the list.
void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    // Attach the callback for the list.
    neoList.attachCallback( listCallback );

    shell.begin( "arnold" );
    shell.beginScreen( &neoList );

}

// Infinite loop.
void loop(){

    shell.update();

}

void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    // Generate answer based on the selected answer.
    if( selected == 0 ){
        notification.setText( "You stay in Wonderland and I show\nyou how deep the rabbit hole goes." );
    }
    else{
        notification.setText( "The story ends, you wake up in your bed and\nbelieve whatever you want to believe." );
    }

    parent -> swapScreen( &notification );

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Notification Example](@ref 305_guiNotification_page) | [GUI List Advanced](@ref 307_guiListAdvanced_page) |
 
</div>
