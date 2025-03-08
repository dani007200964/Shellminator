@page 302_guiProgressAdvanced_page Example 302 GUI Progress Advanced

@tableofcontents

In this example, we’ll customize the progress bar we created earlier.  

First, it’s always nice to **display some custom text** alongside the progress bar:  

```cpp
progress.setText("Working on something...");
```

If the process takes a while, it might be useful to **show how long it has been running** or even **estimate how much time is left**:  

```cpp
progress.setFormat("t");
```

The **setFormat** method takes a format string where each letter represents a specific function. Here are the available options:  

- `s` → Seconds since the process started  
- `m` → Minutes since the process started  
- `t` → Time elapsed since the process started `[mm:ss]`  
- `r` → Estimated remaining time `[mm:ss]`  
- `SPACE` → Prints a blank space  
- `|` → Prints a vertical separator  

For example, if you want to display the elapsed time and the estimated remaining time, separated by a **vertical line**, use:  

```cpp
progress.setFormat("t | r");
```

You can also **customize the progress bar color**:  

```cpp
progress.setColor(Shellminator::GREEN);
```

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/302_guiProgressAdvanced.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
*/

#include "Shellminator.hpp"
#include "GUI/Shellminator-Progress.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a progress bar object and connect it to the shell.
ShellminatorProgress progress;

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

    progress.setFormat( "t" );
    progress.setText( "Working on something..." );
    progress.setColor( Shellminator::GREEN );

    shell.begin( "arnold" );
    shell.beginScreen( &progress );

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

        progress.setPercentage( percentage );

    }

    shell.update();

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[GUI Progress Example](@ref 301_guiProgress_page) | [GUI Level Meter Example](@ref 303_guiLevelMeter_page) |
 
</div>