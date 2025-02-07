@page 400_simpleScreen_page Example 400 Simple Screen

@tableofcontents

In the previous examples, we explored basic GUI elements. Now, let's take things a step further and see how we can display multiple GUI elements on a single screen.  

Our approach is very similar to the layout systems used in Tkinter, Matlab, and Matplotlib. The idea is simple: we divide the screen into a grid with both horizontal and vertical sections. Then, we place elements into specific grid positions. Optionally, we can also define how many grid cells wide and tall each element should be.  

Let’s start with a simple grid!  

In this example, we'll create a grid with **3 rows** and **3 columns**:  

```cpp
// Create a grid with 3 rows and 3 columns
ShellminatorScreenGrid grid( 3, 3 );
```  

Each grid follows a structured **indexing system**. To keep things familiar, we follow the same logic as many popular APIs:  
- The **first index** represents the row.  
- The **second index** represents the column.  

Here’s an illustration to help visualize how grid indexing works:  

![Grid Indexing](screen_grid_addressing.svg)

Indexing starts from **0**, meaning the first row is `0`, the second row is `1`, and so on. The same applies to columns. Since we’re working with a **3x3 grid**, the highest index value for both rows and columns is **2** (because indexing starts from 0).  

To add elements to the grid, we use the `addWidget` method. Let’s see how it works:  

```cpp
// Place button_1 in the first row, second column
grid.addWidget( &button_1, 0, 1 );

// Place button_2 in the second row, second column
grid.addWidget( &button_2, 1, 1 );

// Place button_3 in the third row, second column
grid.addWidget( &button_3, 2, 1 );
```  

By default, each element takes up **one cell** in width and height. But what if we want an element to span multiple cells?  

To make an element **wider** or **taller**, we still use the `addWidget` method, but we pass two additional parameters:  

```cpp
// Place meter in the first row, first column
// Make it three rows tall and one column wide
grid.addWidget( &meter, 0, 0, 3, 1 );
```  

Finally, to start the GUI, we use the `beginScreen` method as usual. But this time, we pass our **grid** as the screen object to be drawn:  

```cpp
// Register the Screen object. The terminal will pass
// control to it until the user presses the ESC button.
shell.beginScreen( &grid );
```  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/400_simpleScreen.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
#include "Shellminator-Screen.hpp"
#include "GUI/Shellminator-ScreenGrid.hpp"
#include "GUI/Shellminator-Buttons.hpp"
#include "GUI/Shellminator-Notification.hpp"
#include "GUI/Shellminator-Level-Meter.hpp"
#include "GUI/Shellminator-Progress.hpp"

ShellminatorButton button_0( "BTN 0" );
ShellminatorButton button_1( "BTN 1" );
ShellminatorButton button_2( "BTN 2" );
ShellminatorButton button_3( "BTN 3" );
ShellminatorButton button_enter( "Enter" );
ShellminatorLevelMeter meter_1( "Audio L" );
ShellminatorLevelMeter meter_2( "Audio R" );

ShellminatorNotification notification;
ShellminatorProgress progress_1;
ShellminatorProgress progress_2;
ShellminatorProgress progress_3;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a grid 3 rows and 6 columns
ShellminatorScreenGrid grid( 3, 6 );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    notification.setText( "Hello!" );

    meter_1.setPercentage( 35.0 );
    meter_2.setPercentage( 73.0 );

    progress_1.setPercentage( 30.0 );
    progress_2.setPercentage( 80.0 );
    progress_3.setPercentage( 21.0 );

    // Place button_0 to the first row and the first column.
    // If no span specified it defaults to one grid cell size.
    grid.addWidget( &button_0, 0, 0 );

    // Place button_1 to the first row and the second column.
    grid.addWidget( &button_1, 0, 1 );

    // Place button_2 to the second row and the first column.
    grid.addWidget( &button_2, 1, 0 );

    // Place button_2 to the second row and the second column.
    grid.addWidget( &button_3, 1, 1 );

    // Place button_enter to the third row and the first column.
    // Make it one cells heigh and two cell wide.
    grid.addWidget( &button_enter, 2, 0, 1, 2 );

    // Place notification to the first row and the third column.
    // Make it two cells high and one cell wide.
    grid.addWidget( &notification, 0, 2, 2, 1 );

    grid.addWidget( &progress_1, 0, 5 );
    grid.addWidget( &progress_2, 1, 5 );
    grid.addWidget( &progress_3, 2, 5 );

    grid.addWidget( &meter_1, 0, 3, 3, 1 );
    grid.addWidget( &meter_2, 0, 4, 3, 1 );

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
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Plot Advanced Example](@ref 309_guiPlotAdvanced_page) | [Screen End Callback Example](@ref 401_screenEndCallback_page) |
 
</div>