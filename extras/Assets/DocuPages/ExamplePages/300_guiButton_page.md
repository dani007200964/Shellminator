@page 300_guiButton_page Example 300 GUI Button

@tableofcontents

## Creating a GUI in the Terminal  

Building a graphical user interface (GUI) is never an easy task, and things get even trickier when working in a **terminal**, which uses characters instead of pixels. But don’t worry—we have a few tricks up our sleeve! With the right approach, you can create a **surprisingly functional** user interface from scratch, and the best part? It all runs inside a **terminal emulator**.  

Of course, this method won’t offer the same level of polish as something like **Qt** or **LVGL**, but hey—you don’t need to develop a full PC application and port it to a million platforms. All you need is a **terminal emulator**, and you’re good to go!  

## How Does It Work?  

Every GUI is built from **basic building blocks (primitives)** that can be freely positioned on the screen. The **Shellminator GUI libraries** follow this exact approach.  

Let’s dive in!  

## Step 1: Creating a Button  

In almost every GUI framework, the **'Hello, World!'** example involves creating a **button**. This makes sense because a button is both an **input and output element** —you can display text on it, and it can trigger actions when pressed.  

To get started, include the necessary GUI library from the **Shellminator** source folder:  

```cpp
#include "GUI/Shellminator-Buttons.hpp"
```  

Next, create a **button object** and define the text that should appear on it:  

```cpp
// Create a button object and specify its label.
ShellminatorButton button("This is a button");
```

## Step 2: Handling User Interaction  

Buttons in **Shellminator GUI** can respond to **two types of interactions**:  

1. **Keyboard input** (hotkeys or direct key presses).  
2. **Mouse clicks** (yes, really!).  

If you're using **PuTTY** or the **browser-based demo**, mouse clicks will work. However, not all terminal emulators support this feature.
Because of this, it’s a good idea to **assign a keyboard shortcut** to every button.  

First, define an **event configuration** that tells the button which key should trigger it:  

```cpp
// Create an event configuration for the button.
Shellminator::shellEvent_t buttonEvent;
```  

Next, we’ll define a **callback function**—this is the function that will be executed when the button is pressed:  

```cpp
// This function will be called when the button is pressed.
void buttonClick(ShellminatorScreen* screen);
```  

The **callback function** must:  
- **Return** `void`.  
- **Accept** a single argument: `ShellminatorScreen* screen`.  

## Step 3: Setting Up the Event  

Now, let’s configure the event to trigger when the user presses the 'x' **key** (or clicks the button, if mouse support is available):  

```cpp
// Configure the event for the 'x' key on the keyboard.
buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
buttonEvent.data = (uint8_t)'x';
```  

Attach this event to the **button object**:  

```cpp
// Attach the event to the button.
button.attachEvent(buttonEvent);
```  

And don’t forget to assign the **callback function**:  

```cpp
// Attach the callback function to the button.
button.attachTriggerFunction(buttonClick);
```  

## Step 4: Customizing the Button  

You can also **change the button’s color**:  

```cpp
// Set the default color of the button to red.
button.setColor(Shellminator::RED);
```  

## Step 5: Switching to GUI Mode  

To switch **Shellminator** from **terminal mode** to **GUI mode**, we need a **screen** object. Luckily, **every GUI element** can act as a screen, so we can start with just our button:  

```cpp
// Start a new screen with the button and set a 100ms refresh rate.
shell.beginScreen(&button, 100);
```  

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>The <b>refresh rate</b> (100ms in this case) determines how often the GUI updates.
            A <b>low value</b> might cause flickering, while a <b>high value</b> could make the interface feel sluggish. <b>100ms works well for most cases.</b>  
        </p>
    </div>
</div>
\endhtmlonly

## Step 6: Writing the Button Click Callback  

Here’s the **full callback function** that toggles the button color between **red and green** each time it’s clicked:  

```cpp
void buttonClick(ShellminatorScreen* screen) {
    // Get the parent terminal for redraw requests.
    Shellminator* parent = screen->getParent();

    // If no parent found, exit.
    if (parent == NULL) {
        return;
    }

    // Toggle the button color.
    static bool toggle = false;
    toggle = !toggle;

    if (toggle) {
        button.setColor(Shellminator::GREEN);
    } else {
        button.setColor(Shellminator::RED);
    }

    // IMPORTANT: Always request a redraw after modifying the GUI!
    parent->requestRedraw();
}
```  

## Summary  

- We **created a button** and attached an event.  
- We **assigned a keyboard shortcut ('x')** and a **mouse click event** (if supported).  
- We **wrote a callback function** to toggle the button's color.  
- We **requested a screen refresh** to update the GUI properly.   

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/300_guiButton.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>When it comes to the Shellminator GUI, one important thing to keep in mind is that Shellminator is, first and foremost, a terminal. This means that you should always be able to exit the GUI at any time. Usually, pressing <b>Ctrl+C</b> or just hitting <b>Enter</b> will do the trick.
        </p>
    </div>
</div>
\endhtmlonly

In the example above, if you press Enter, the system will exit the GUI. This is intentional—it prevents a malfunctioning GUI from blocking access to the system. After all, it's incredibly frustrating if the only way to exit a dashboard is by restarting the entire system!

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

#include "math.h"

#include "Shellminator.hpp"
#include "GUI/Shellminator-Buttons.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// Create a button object and specify the text for it.
ShellminatorButton button( "This is a button" );

// Create an event handler for the button.
Shellminator::shellEvent_t buttonEvent;

// It will be used later when pressing the button.
bool toggle = false;

// This function will be called, when the button is pressed.
void buttonClick( ShellminatorScreen* screen );

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.println( "Program Start!" );

    // Configure the event for the x key on the keyboard.
    buttonEvent.type = Shellminator::SHELL_EVENT_KEY;
    buttonEvent.data = (uint8_t)'x';

    // Attach the event to the button.
    button.attachEvent( buttonEvent );

    // Attach the callback function for the button.
    // It will be called, when the button gets pressed.
    button.attachTriggerFunction( buttonClick );

    // Set the default color for the button to red.
    button.setColor( Shellminator::RED );

    // Start as usual but...
    shell.begin( "arnold" );

    // Start a screen right after to hide the console.
    shell.beginScreen( &button, 100 );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}

void buttonClick( ShellminatorScreen* screen ){
    // We need the parent terminal later for redraw request.
    Shellminator* parent;

    // Extract the parent terminal's reference from the screen object.
    parent = screen -> getParent();

    // If no parent found, we have to return.
    if( parent == NULL ){
        return;
    }

    // Toggling the toggle variable
    toggle = !toggle;

    // Change color according to toggle.
    if( toggle ){
        button.setColor( Shellminator::GREEN );
    }
    else{
        button.setColor( Shellminator::RED );
    }

    // This is very important! We have to request a redraw
    // each time, when we modified something on the GUI!
    parent -> requestRedraw();
}
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Commander Colorizer Example](@ref 208_commanderColorizer_page) | [GUI Progress Example](@ref 301_guiProgress_page) |
 
</div>