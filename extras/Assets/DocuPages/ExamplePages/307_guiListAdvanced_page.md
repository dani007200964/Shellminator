@page 307_guiListAdvanced_page Example 307 GUI List Advanced

@tableofcontents

In the previous example, we created a simple list for user selection. But what if the options **aren't descriptive enough**? That’s where **ShellminatorListDetailed** comes in!  

This class works **almost exactly like ShellminatorList**, with one key difference: **each option can have an additional description**. This makes it easier for users to understand their choices.  

Just like before, start by creating the list of choices:  

```cpp
// Generate a list of options
const char* listOptions[] = {
    "Aladdin",
    "The Iron Giant",
    "Treasure Planet"
};
```

A short description to guide the user:  

```cpp
// Simple instructions text.
const char* listText = "Choose a movie to watch:";
```

Just like with a regular list, you need a function to handle the user's selection:  

```cpp
// Callback for the list.
void listCallback(const char* optionsList[], int listSize, int selected, ShellminatorScreen*);
```

Now comes the trick! Create a **second list** that contains extra details for each option. **Make sure the order matches the main list!**  

```cpp
// Generate a list of details
const char* listDetails[] = {
    "Released: November 25, 1992",
    "Released: July 31, 1999",
    "Released: November 27, 2002"
};
```

Finally, use the constructor to **connect the options with their descriptions**:  

```cpp
// Create a detailed list.
ShellminatorListDetailed movieList(listOptions, listDetails, 3, listText);
```

Aside from adding descriptions, **everything else works the same** as with ShellminatorList! This is a great way to **provide more context** while keeping the selection process simple.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/307_guiListAdvanced.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
#include "GUI/Shellminator-List-Detailed.hpp"
#include "GUI/Shellminator-Notification.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorNotification notification;

// Generate a list of options
const char* listOptions[] = {
    "Aladdin",
    "The Iron Giant",
    "Treasure Planet"
};

// Generate a list of details
// The order must match with the options
const char* listDetails[] = {
    "1992. November 25.",
    "1999. June 31.",
    "2002. November 27."
};

// Simple instructions text.
const char* listText =  "Choose a movie to watch:";

// Create a detailed list.
ShellminatorListDetailed movieList( listOptions, listDetails, 3, listText );

// Callback for the list.
void listCallback( const char* optionsList[], int listSize, int selected, ShellminatorScreen* );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );
    movieList.attachCallback( listCallback );

    shell.begin( "arnold" );
    shell.beginScreen( &movieList );

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

    // Generate a notification based on the answer.
    if( selected == 0 ){
        notification.setText( "Good choice, you can watch Aladdin here:\nhttps://www.imdb.com/title/tt0103639/" );
    }
    else if( selected == 1 ){
        notification.setText( "Good choice, you can watch The Iron Giant here:\nhttps://www.imdb.com/title/tt0129167/" );
    }
    else{
        notification.setText( "Good choice, you can watch the Treasure Planet here:\nhttps://www.imdb.com/title/tt0133240/" );
    }

    parent -> swapScreen( &notification );

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI List Example](@ref 306_guiList_page) | [GUI Plot Example](@ref 308_guiPlot_page) |
 
</div>