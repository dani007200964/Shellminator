@mainpage Introduction

## The most versatile terminal interface for your next robot

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Rocket.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Shellminator is an innovative terminal interface specially designed for embedded devices. Written in C++, it is fully compatible with the Arduino development environment. We absolutely love creative minds, so we've made sure that you can easily and quickly build a versatile, feature-rich command line for your next amazing project.
        </p>
    </div>
</div>
\endhtmlonly

## Just a few lines of code and it is done!

```cpp
#include "Shellminator.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// System init section.
void setup(){
    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Initialize shell object.
    shell.begin( "arnold" );
}

// Infinite loop.
void loop(){
    // Process the new data.
    shell.update();
}
```

\htmlonly
<div style="display:flex; align-items: center;">
    <div>
        <p>Right? It doesn't seem that complicated, does it? It was very important to us to keep things simple if that's what you prefer. After all, if you don't appreciate the small things, you don't deserve the big ones. But of course, if you love taking things to a more complex level, the sky's the limit!
        </p>
    </div>
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Comet.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
</div>
\endhtmlonly



## Let me show you what I mean


\htmlonly
<iframe id="demoFrame" src="webExamples/200_commanderBasic.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

## We'd love for you to learn all the ins and outs

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Graduation-cap.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>No need to worry, we've put together a lot of well-structured, interactive, and exciting examples to help you get the most out of this system. Don't hesitate, check out the examples section!</p>
    </div>
</div>
\endhtmlonly

## But isn't the command line a bit outdated?

\htmlonly
<div style="display:flex; align-items: center;">
    <div>
        <p>That's a completely valid question in a world where almost any microcontroller rolling off the production line can drive a display. However, as IoT continues to grow, so do the number of devices that don't have any kind of user interface, except maybe a USB port for configuration.
        </p>
    </div>
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Thinking-face.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
</div>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Alien.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>There are also situations where it's physically impossible for someone to be next to the device. Just think about the Mars rover – no one is standing next to it all day because it would be quite a long commute if that were someone's job!
        </p>
    </div>
</div>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div>
        <p>Another important aspect to consider with a device that has a command line interface is the ability to interactively manipulate the hardware without reprogramming it. Imagine how awesome that is! If you set up such a system smartly, you can adjust GPIO pins, request and read data from sensors, take measurements with the ADC with just a few keystrokes.
        </p>
    </div>
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Arm-mechanical.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
</div>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Bug.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Just imagine how much easier it is to debug a system this way. You can not only control it but also diagnose issues. Anyone who has ever built a robot knows that unexpected things always pop up in the field. Sometimes it's funny, but other times it can make you want to cry.
        </p>
    </div>
</div>
\endhtmlonly

\htmlonly
<div style="display:flex; align-items: center;">
    <div>
        <p>Currently, a small team is working on this project, and we warmly welcome anyone who wants to contribute. Of course, we've prepared all the necessary guides for you in advance, and you can find them in the Developer Zone.
        </p>
    </div>
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Constructio.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
</div>
\endhtmlonly

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|                   | [Examples](examples_page.md) |
 
</div>