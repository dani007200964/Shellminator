@page 305_guiNotification_page Example 305 GUI Notification

@tableofcontents

Sometimes, it's important to **notify the user** when a certain event occurs. That’s exactly what the **ShellminatorNotification** class is for!  

It simply generates a **text box in the center of the screen**, where you can display any message you want. The notification stays on the screen **until the user presses Enter or Ctrl+C**, making sure they don’t miss important information.  

Using notifications is easy—just create an object from the **ShellminatorNotification** class:  

```cpp
ShellminatorNotification notification;
```

Before displaying it, set the text you want to show. This can be **multiple lines** if needed:  

```cpp
notification.setText("Hurray!\nYou pressed the button!");
```

To bring the notification to the foreground, use either the **beginScreen** or **swapScreen** method:  

```cpp
swapScreen(&notification);
```

- `beginScreen(&notification);` → Use this if you’re switching **from terminal mode to GUI mode**.  
- `swapScreen(&notification);` → Use this if you’re **already in GUI mode** and want to **replace the current content** with the notification.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/305_guiNotification.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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

#include "math.h"

#include "Shellminator.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-Notification.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorNotification notification;

// Create a plotter object.
ShellminatorButton button( "Press" );

Shellminator::shellEvent_t buttonEvent;

// This function will be called, when the button is pressed.
void buttonClick( ShellminatorScreen* screen );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    button.attachEvent( buttonEvent );
    button.attachTriggerFunction( buttonClick );
    button.setColor( Shellminator::RED );

    shell.begin( "arnold" );
    shell.beginScreen( &button, 100 );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}

void buttonClick( ShellminatorScreen* screen ){
    Shellminator* parent;
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }
    
    notification.setText( "Hurray!\nYou pressed the button!" );

    parent -> swapScreen( &notification );
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Level Meter Example](@ref 303_guiLevelMeter_page) | [GUI List Example](@ref 306_guiList_page) |
 
</div>