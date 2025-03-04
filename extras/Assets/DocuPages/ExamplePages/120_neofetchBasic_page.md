@page 120_neofetchBasic_page Example 120 Neofetch Basic

@tableofcontents

The original **Neofetch** is a simple terminal program that displays key system information in a nice, colorful way. It shows details like your operating system, processor type, memory size, and much more. It’s mainly used for quickly checking your system specs or sharing cool-looking system info with others.  

Now, **Shellminator** can do the same thing, making your interface more colorful and user-friendly. It’s super easy to use: when you run the `neofetch` command, a callback function is triggered. You just need to attach this callback to your system. To make things even simpler, we’ve created a built-in implementation that you can customize with parameters if needed.  

Compared to the original Neofetch, our built-in version uses different parameters—mainly because many of the original options aren’t relevant for embedded systems. Here’s what we include:  

- **FW** → Firmware version (useful for identifying your system)  
- **CPU** → Microcontroller ID (handy if the same firmware runs on multiple platforms)  
- **Compiler** → Compiler type (and possibly version)  
- **Compile Date** → When the firmware was built  
- **Author** → Who owns the project  

## Getting it to work takes just two simple steps!  

First, include the necessary library:  
```cpp
#include "Shellminator-Neofetch.hpp"
```  

Then, attach the Neofetch callback to your terminal. We’ll use the built-in function for this:  
```cpp
shell.attachNeofetchFunc(defaultShellminatorNeofetch);
```  

If everything is set up correctly, the `neofetch` command should now work—go ahead and try it in the demo below!  

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/120_neofetchBasic.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>As you can see, Neofetch is running, but right now it’s displaying the default parameters and values. In the next example, we’ll show you how to customize these settings.
        </p>
    </div>
</div>
\endhtmlonly

## Whole Code

```cpp
/*
 * Created on June 8 2024
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/

#include "Shellminator.hpp"
#include "Shellminator-Neofetch.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the default neofetch callback.
    shell.attachNeofetchFunc( defaultShellminatorNeofetch );

    // Initialize shell object.
    shell.begin( "arnold" );

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
|[Buffering Example](@ref 102_buffering_page) | [Neofetch Advanced Example](@ref 121_neofetchAdvanced_page) |
 
</div>