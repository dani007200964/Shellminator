@page 100_keyOverride_page Example 100 Key Override

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Did you know that you can assign functionality to specific key combinations? Imagine, with the up, down, left, and right arrows, you could even control servos!
        That’s right, you have the freedom to decide what custom action the Ctrl-C combination (or any other) should trigger, if you need it. 
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/100_keyOverride.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

All you need to do is call the corresponding key override function. The table includes all the key combinations you can override.

| Function Name         | Key Combo     |
| --------------------- | ------------- |
| overrideUpArrow()     | Up Key        |
| overrideDownArrow()   | Down Key      |
| overrideLeftArrow()   | Left Key      |
| overrideRightArrow()  | Right Key     |
| overrideAbortKey()    | Ctrl-C Combo  |
| overridePageUpKey()   | Page Up Key   |
| overridePageDownKey() | Page Down Key |
| overrideHomeKey()     | Home Key      |
| overrideEndKey()      | End Key       |
| overrideLogoutKey()   | Ctrl-D Combo  |
| overrideSearchKey()   | Ctrl-R Combo  |

Key override functions work by redirecting control from the default functionality to a callback written by the user (essentially hooks). Because of this, every key override function requires a function that will be called when the specific key is pressed. For example create a callback:

```cpp
void upArrowCallback( Shellminator* caller ){
    // This will be called when the Up Arrow Key is pressed.
}
```

And override the up arrow key behavior:

```cpp
shell.overrideUpArrow( upArrowCallback );
```

Similar to key override functions, there are key free functions that restore the default behavior.

| Function Name     | Key Combo     |
| ----------------- | ------------- |
| freeUpArrow()     | Up Key        |
| freeDownArrow()   | Down Key      |
| freeLeftArrow()   | Left Key      |
| freeRightArrow()  | Right Key     |
| freeAbortKey()    | Ctrl-C Combo  |
| freePageUpKey()   | Page Up Key   |
| freePageDownKey() | Page Down Key |
| freeHomeKey()     | Home Key      |
| freeEndKey()      | End Key       |
| freeLogoutKey()   | Ctrl-D Combo  |
| freeSearchKey()   | Ctrl-R Combo  |

Both the key override and key free functions should have the following structure: they should return `void` and take one argument, which is `Shellminator* caller`. The function name is up to the user, but the return type and argument are fixed.

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


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

void upArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "up" );
}

void downArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "down" );
}

void leftArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "left" );
}

void rightArrowCallback( Shellminator* caller ){
    caller -> setBannerText( "right" );
}

void searchKeyCallback( Shellminator* caller ){
    caller -> freeSearchKey();
    caller -> freeUpArrow();
    caller -> freeDownArrow();
    caller -> freeLeftArrow();
    caller -> freeRightArrow();
    caller -> setBannerText( "default" );
}

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Use the arrow keys to change the banner text." );
    Serial.print( "To reset the functionality, press" );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    Serial.print( " Ctrl-R ");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "key." );
    Serial.println();

    shell.overrideUpArrow( upArrowCallback );
    shell.overrideDownArrow( downArrowCallback );
    shell.overrideLeftArrow( leftArrowCallback );
    shell.overrideRightArrow( rightArrowCallback );
    shell.overrideSearchKey( searchKeyCallback );

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
|[Banner Text Example](@ref 010_bannerText_page) | [QR Code Example](@ref 101_qrCode_page) |
 
</div>