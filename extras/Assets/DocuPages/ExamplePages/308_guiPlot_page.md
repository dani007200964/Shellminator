@page 308_guiPlot_page Example 308 GUI Plot

@tableofcontents

Sometimes, it's not just the **current value** of a measurement that matters, but also **how it changes over time**. Examples include **temperature trends, stability monitoring, or control system setpoints**.  

For such cases, a **plot** is extremely useful, as it allows us to **visualize changes over time**.  
Luckily, **creating a plot with Shellminator is easy**! Let's walk through the steps.  
First, you'll need to include the plot module:  

```cpp
#include "GUI/Shellminator-PlotModule.hpp"
```

Currently, **Shellminator’s plotter is quite simple** —it only supports **one-dimensional data vectors**.  
While it may not be as advanced as Matplotlib, **it runs on tiny AVR microcontrollers** with minimal memory!  

```cpp
// Data points
float plotData[] = { 10, 20, 30, 20, 10, 0, -10, -20, -30, -20 };

// Number of points in the data
int plotDataSize = sizeof(plotData) / sizeof(plotData[0]);
```

It’s a good practice to **calculate the number of data points automatically** like in the example above.  
Now, create a **ShellminatorPlot** object and pass the data:  

```cpp
// Create a plot object
ShellminatorPlot plot(plotData, plotDataSize, "My Plot");
```

The **plot name** can be anything you like—just make sure it describes your data well!  
Finally, to show the plot on the screen, just call:  

```cpp
shell.beginScreen(&plot);
```

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/308_guiPlot.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
#include "GUI/Shellminator-PlotModule.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Data points
float plotData[] = { 10, 20, 30, 20, 10, 0, -10, -20, -30, -20 };

// Number of points in the data
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

// Create a plot object
ShellminatorPlot plot( plotData, plotDataSize, "My Plot" );


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    shell.begin( "arnold" );
    shell.beginScreen( &plot );

}

// Infinite loop.
void loop(){

    shell.update();

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI List Example](@ref 306_guiList_page) | [GUI Plot Advanced Example](@ref 309_guiPlotAdvanced_page) |
 
</div>