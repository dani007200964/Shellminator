@page 301_guiProgress_page Example 301 GUI Progress

@tableofcontents

Sometimes, a process takes longer than just a blink of an eye. In such cases, it's helpful to have a **progress bar** to show how things are going. Creating a progress bar is super easy with the **ShellminatorProgress** class. You can find it in this library:  

```cpp
#include "GUI/Shellminator-Progress.hpp"
```

First, create an object from the **ShellminatorProgress** class:  

```cpp
ShellminatorProgress progress;
```

Whenever you want to update the progress bar, simply use the **setPercentage** method. This method takes a single argument, which should be a percentage between **0 and 100.0**. For example, to set the progress to **50%**, you would write:  

```cpp
progress.setPercentage(50.0);
```

To actually **display** the progress bar, just like in the previous example, use the **beginScreen** method:  

```cpp
shell.beginScreen(&progress);
```

To make the demo more interesting, the progress bar continuously counts up and down. This is achieved using a **non-blocking timer** inside the loop section.  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/301_guiProgress.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
|[GUI Button Example](@ref 300_guiButton_page) | [GUI Progress Advanced Example](@ref 302_guiProgressAdvanced_page) |
 
</div>