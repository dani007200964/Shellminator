@page 102_buffering_page Example 102 Buffering

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>We’re slowly getting to the examples where we’ll start using the terminal for cooler and cooler tasks. But before that, we need to understand how the terminal emulator
        works on our computer. The terminal emulator processes simple text commands that contain a few special characters. These special characters tell the terminal emulator what
        specific operation it needs to perform. The terminal emulator works best when it has to process large amounts of data at once because it can interpret the data all together.
        If, for example, it receives one command character by character instead of all at once, it’s not the end of the world. The only thing you might notice is that the screen
        might flicker since the rendering happens character by character, not in blocks.
        </p>
    </div>
</div>
\endhtmlonly

In the demo below, the speed improvement might not be super noticeable since it's running on a fast computer. You'll really see the difference when typing quickly on a **slower microcontroller**.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/102_buffering.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

This issue can be solved using buffering. You can think of it like having a relatively large buffer where Shellminator starts building the package that it would normally send to the terminal interface character by character. The data accumulates in this buffer until:

* __A:__ The buffer gets full, in which case its contents are automatically flushed, and the process starts over.
* __B:__ Shellminator finishes assembling the command and flushes the buffer on its own.

By default, buffering is turned off to save memory for the system. For example, on an Arduino Uno R3, it's not even recommended to enable it. However, if you have a microcontroller with 5-10kByte of free memory, you can comfortably set up a buffer for better terminal performance. We recommend that the buffer be at least 100 bytes in size, but of course, the larger it is, the better. However, it's rare for a buffer larger than 10kByte to result in significant improvements, so it's a good idea to experiment and find the ideal size for your specific environment.

It might seem a bit tricky at first, but we’ve put a lot of work into making this process as simple as possible for you. First, you'll need the buffer itself:

```cpp
// Create a buffer with 100 elements
uint8_t printBuffer[100];
```

Next, we calculate the buffer size (this step is optional, but trust me, it’s easier this way):

```cpp
// Calculate buffer size
int printBufferSize = sizeof(printBuffer);
```

Finally, in the initialization section, we enable buffering:

```cpp
// Enable buffering
shell.enableBuffering(printBuffer, printBufferSize);
``` 

This should make everything much smoother and easier to manage!

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

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

uint8_t printBuffer[ 100 ];
int printBufferSize = sizeof( printBuffer );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    shell.enableBuffering( printBuffer, printBufferSize );

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
|[QR Code Example](@ref 101_qrCode_page) | [Neofetch Basic Example](@ref 120_neofetchBasic_page) |
 
</div>