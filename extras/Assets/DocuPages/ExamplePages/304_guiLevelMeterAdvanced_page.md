@page 304_guiLevelMeterAdvanced_page Example 304 GUI Level Meter Advanced

@tableofcontents

The **ShellminatorLevelMeter** is great for visualizing quantities such as **pressure, temperature, speed**, and more. These types of values typically have **upper and lower limits**, and using colors to represent different ranges makes them much easier for the human eye to understand.  

In many cases, the **exact numerical value isn’t important** —it’s enough to see at a glance whether something is in a good or bad range. Because of this, the **ShellminatorLevelMeter** allows you to divide the scale into three **color-coded** sections:  

- **Normal range** (default: **WHITE**)  
- **Warning range** (default: **YELLOW**)  
- **Danger range** (default: **RED**)  

Of course, you can **customize these colors**:  

```cpp
meter.setColor(Shellminator::YELLOW);      // Normal range color  
meter.setWarningColor(Shellminator::GREEN); // Warning range color  
meter.setErrorColor(Shellminator::RED);    // Danger range color  
```

You can also **define the boundaries** for when the warning and danger colors should appear:  

```cpp
// Show a warning above 50%
meter.setWarningPercentage(50.0);

// Show an error above 80%
meter.setErrorPercentage(80.0);
```

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/304_guiLevelMeterAdvanced.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
#include "GUI/Shellminator-Level-Meter.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorLevelMeter meter( "Oil Pressure" );

uint32_t timerStart = 0;
uint32_t period = 100;
float percentage = 1.0;
float step = 1.0;

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    meter.setColor( Shellminator::CYAN );
    meter.setWarningColor( Shellminator::BLUE );
    meter.setErrorColor( Shellminator::RED );

    meter.setWarningPercentage( 50.0 );
    meter.setErrorPercentage( 80.0 );

    shell.begin( "arnold" );
    shell.beginScreen( &meter );

}

// Infinite loop.
void loop(){

    if( ( millis() - timerStart ) > period ){
        timerStart = millis();
        percentage += step;

        if( percentage > 100.0 ){
            step = -1.0;
            percentage = 100.0;
        }

        if( percentage < 0.0 ){
            step = 1.0;
            percentage = 0.0;
        }

        meter.setPercentage( percentage );

    }

    shell.update();

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Level Meter Example](@ref 303_guiLevelMeter_page) | [GUI Notification Example](@ref 305_guiNotification_page) |
 
</div>