@page 309_guiPlotAdvanced_page Example 309 GUI Plot Advanced

@tableofcontents

This is an **exciting example** where we demonstrate how to **dynamically update** a plot in real time.  
In this case, we've implemented a **capacitor charging simulator** that calculates the **ideal capacitor voltage over time**.  
Every **2 seconds**, the capacitor voltage is **recalculated** and added to the plot.  
To keep the simulation **real-time**, we use the `millis()` function. So, if you were to **build this circuit in real life** and measure the capacitor voltage with an ADC, you’d see **the exact same behavior**.  


Let's give our plot a **distinctive color**:  

```cpp
// Set a custom color for the plot
ShellminatorPlot plot(plotData, plotDataSize, "Capacitor Voltage [V]", Shellminator::RED);
```

We define a **timer** to control how often new values are calculated:  

```cpp
uint32_t timerStart = 0;
uint32_t timerPeriod = 2000;  // 2 seconds

// In the defined intervals, recalculate the capacitor's voltage
if ((millis() - timerStart) > timerPeriod) {
    timerStart = millis();
    capacitorSimulator();  

    // After updating the data, request the shell to redraw the plot
    shell.requestRedraw();
}
```

Whenever you update the plot **data**, you **must call**:  

```cpp
shell.requestRedraw();
```

Without this, the changes **won't appear** on the screen.  

Since **redrawing the plot is resource-intensive**, it's best **not to update it too frequently**—every **few seconds** is ideal.  
This means **Shellminator isn’t suited for oscilloscope-level speeds**, but it works **great for visualizing slower processes**.


## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/309_guiPlotAdvanced.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
#include <math.h>

// Simulation constants
#define R 5100.0      // Resistor (Ohm)
#define C 0.01    // Capacitance (Farad)
#define U0 5.0        // Supply Voltage (Volt)

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

float plotData[ 100 ];
int plotDataSize = sizeof( plotData ) / sizeof( plotData[ 0 ] );

uint32_t timerStart = 0;
uint32_t timerPeriod = 1000;

// Set a custom color for the plot.
ShellminatorPlot plot( plotData, plotDataSize, "Capacitor Voltage[ V ]", Shellminator::RED );

void capacitorSimulator();

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

    // In the timerPeriod defined intervals, recalculate the current
    // state of the simulated capacitor.
    if( ( millis() - timerStart ) > timerPeriod ){
        timerStart = millis();
        capacitorSimulator();

        // After calculation, we have to request the shell
        // to redraw the plot.
        shell.requestRedraw();
    }

    shell.update();

}

void capacitorSimulator(){
    int i;

    // With the for loop, we shift every element in the plot to the left
    for( i = 0; i < plotDataSize - 1; i++ ){
        plotData[ i ] = plotData[ i + 1 ];
    }

    // To the last element, we calculate the new value based on system time.
    plotData[ plotDataSize - 1 ] = U0 * ( 1.0 - exp( -( millis() / 1000.0 ) *  ( 1.0 / ( R * C ) ) ) );

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Plot Example](@ref 308_guiPlot_page) | [GUI Plot Advanced Example](@ref 309_guiPlotAdvanced_page) |
 
</div>