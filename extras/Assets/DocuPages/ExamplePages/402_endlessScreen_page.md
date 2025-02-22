@page 402_endlessScreen_page Example 402 Endless Screen

@tableofcontents

In some rare cases, you might need to **prevent the terminal from exiting GUI mode**. However, this should be done **carefully** and only when absolutely necessary.  

The most common scenario for this is when the **terminal interface is not needed at all**—for example, in a **video game**, where the user should remain in the GUI environment without returning to the terminal.  

To achieve this, simply **assign an "end callback"** to the screen object. Inside this callback, use the `swapScreen` or `beginScreen` methods to reinitialize the screen object.   

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/402_endlessScreen.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

## Whole Code

```cpp
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * 
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/

#include "Shellminator.hpp"
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-ScreenGrid.hpp"
#include "GUI/Shellminator-Buttons.hpp"


ShellminatorButton button_1( "BTN 1" );
ShellminatorButton button_2( "BTN 2" );
ShellminatorButton button_3( "BTN 3" );
ShellminatorButton button_4( "BTN 4" );

// Create a grid 2 rows and 2 columns
ShellminatorScreenGrid grid( 2, 2 );

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

Shellminator::shellEvent_t button_1_event;
Shellminator::shellEvent_t button_2_event;
Shellminator::shellEvent_t button_3_event;
Shellminator::shellEvent_t button_4_event;

bool trigger = false;

void button_click( ShellminatorScreen* screen );

void endCallback( Shellminator* parent );

// System init section.
void setup(){

    Serial.begin(115200);


        // Clear the terminal
        shell.clear();

        Serial.println( "Program Start!" );

        // Place button_1 to the first row and the first column.
        grid.addWidget( &button_1, 0, 0 );

        // Place button_2 to the first row and the second column.
        grid.addWidget( &button_2, 0, 1 );

        // Place button_3 to the second row and the first column.
        grid.addWidget( &button_3, 1, 0 );

        // Place button_3 to the second row and the second column.
        grid.addWidget( &button_4, 1, 1 );


        button_1_event.type = Shellminator::SHELL_EVENT_KEY;
        button_1_event.data = (uint8_t)'a';
        button_2_event.type = Shellminator::SHELL_EVENT_KEY;
        button_2_event.data = (uint8_t)'b';
        button_3_event.type = Shellminator::SHELL_EVENT_KEY;
        button_3_event.data = (uint8_t)'c';
        button_4_event.type = Shellminator::SHELL_EVENT_KEY;
        button_4_event.data = (uint8_t)'d';

        button_1.attachEvent( button_1_event );
        button_2.attachEvent( button_2_event );
        button_3.attachEvent( button_3_event );
        button_4.attachEvent( button_4_event );

        button_1.attachTriggerFunction( button_click );
        button_2.attachTriggerFunction( button_click );
        button_3.attachTriggerFunction( button_click );
        button_4.attachTriggerFunction( button_click );

        grid.attachEndFunction( endCallback );

        // Initialize shell object.
        shell.begin( "arnold" );

        // Register the Screen object. The terminal will pass
        // the control to it, until the user presses the ESC button.
        shell.beginScreen( &grid );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}

void button_click( ShellminatorScreen* screen ){
    Shellminator* parent;
    Shellminator::textColor_t color;
    
    parent = screen -> getParent();
    if( parent == NULL ){
        return;
    }

    trigger = !trigger;

    if( trigger ){
        color = Shellminator::RED;
    }
    else{
        color = Shellminator::YELLOW;
    }

    button_1.setColor( color );
    button_2.setColor( color );
    button_3.setColor( color );
    button_4.setColor( color );

    parent -> requestRedraw();
    
}

void endCallback( Shellminator* parent ){
    if( parent == NULL ){
        return;
    }
    
    parent -> swapScreen( &grid );
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Screen End Callback Example](@ref 401_screenEndCallback_page) | [WebSocket Simple Example](@ref 500_webSocketSimple_page) |
 
</div>