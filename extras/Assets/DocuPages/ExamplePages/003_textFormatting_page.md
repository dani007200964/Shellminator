@page 003_textFormatting_page Example 003 Text Formattingggg

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Most modern terminal emulators support VT100 escape sequences. These sequences are special commands that allow you to manipulate various terminal elements, such as color, style, and cursor position. If you're interested in this topic, you can find a detailed explanation <a href='https://vt100.net/docs/vt100-ug/chapter3.html'>here</a>.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/003_textFormatting.html" style="height:500px;width:100%;border:none;display:block; margin-left:30px;"></iframe>
\endhtmlonly

Formatting text with this library is very simple. Much like a modern word processor, you can set different attributes for the text. Once you've set the attributes, anything you print to the console will be formatted accordingly until you change the formatting attributes again.

Let's take a look at the available formatting options. We'll start with the styles, as it's usually best to define these first:
| **Style** | **Description** |
|---|---|
| **Regular** | This is the most important formatting option. If you want to reset the console formatting to default, this is what you need. Typically, it is used before specifying a color. |
| **Bold** | This allows you to set the text to bold. |
| **Low Intensity** | This allows you to set the text to bold. In this case, the text appears slightly lighter than normal. |
| **Italic** | This allows you to set the text to italic. |
| **Underline** | This allows you to make the text underlined. |
| **Blinking** | If your terminal emulator supports this feature, your text will blink. Unfortunately, I've encountered few emulators that support it, but examples include Linux shell and Windows cmd. However, Xterm.js does not support it, so it won't work in browser-based examples or in VS Code. |
| **Reverse** | We're not quite sure what this does since we haven't come across an emulator that supports it yet. We implemented it to comply with the standard, but we'd be thrilled if someone could demonstrate its usefulness to us! |
| **Background** | This is a very useful formatting option that swaps the foreground and background colors. If you want to highlight text, this can be a great choice. |
| **Invisible** | You can create invisible text with this, where the text color matches the terminal background. The text will still be there, you just won't see it, but you can select it with the mouse, for example. |

Here's a simple example code for you:
```cpp
Shellminator::setFormat( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
```
The first thing we need is the address of a Stream object. This could be, for example, the default Serial object. The library will use this to generate the requested VT100 sequence needed to achieve the desired format. After this, you need to specify the formatting settings.

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

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Print some formatted text. Firstly, we need to specify the format,
    // then print the text as usual.
    shell.format( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.println( "Is it bold? Is it yellow?");

    // Finally, we need to set back the format to default.
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    // Some other example.
    shell.format( &Serial, Shellminator::BLINKING, Shellminator::CYAN );
    Serial.println( "Can it blink?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::UNDERLINE, Shellminator::RED );
    Serial.println( "Underline?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::ITALIC, Shellminator::GREEN );
    Serial.println( "Italic?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    shell.format( &Serial, Shellminator::ITALIC, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.println( "Multiple?");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );

    // How to disable formatting?
    shell.enableFormatting = false;
    shell.format( &stdioChannel, Shellminator::ITALIC, Shellminator::GREEN );
    stdioChannel.println( "Disable formatting?");
    shell.format( &stdioChannel, Shellminator::REGULAR, Shellminator::WHITE );

}

// Infinite loop.
void loop(){

}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Command Callback Example](@ref 002_commandCallback_page) | [Formatting Matrix Example](@ref 004_formatMatrix_page) |
 
</div>