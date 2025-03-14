@page 005_logo_page Example 005 Logo

@tableofcontents

If you want to create something really awesome to show off to your colleagues or friends, you definitely need a super cool logo for your project. Online, there are plenty of free tools for this purpose, and we've put together a list of them for you:
* [ChatGPT](https://chatgpt.com/)
* [patorjk ASCII Text](http://patorjk.com/software/taag/#p=display&f=Slant&t=Shellminator)
* [ASCII-Generator](https://ascii-generator.site/)

In the demo below, you’ll see that the logo **nicely appears above the prompt** after startup. Everything else works just the same as before.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/005_logo.html" style="height:500px;width:100%;border:none;display:block; margin-left:30px;"></iframe>
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
#include "Shellminator-Colorizer.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a pretty logo for the terminal.
const char logo[] =

    "   _____ __         ____          _             __            \r\n"
    "  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
    "  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
    " ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
    "/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
    "\r\n"
    "Visit on GitHub: https://www.shellminator.org/html/index.html\r\n\r\n"

;

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

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
|[Formatting Matrix Example](@ref 004_formatMatrix_page) | [Password Example](@ref 006_password_page) |
 
</div>