@mainpage Introduction

## The most versatile terminal interface for your next robot

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="cpu.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Shellminator is an innovative terminal interface specially designed for embedded devices. Written in C++, it is fully compatible with the Arduino development environment. It's compatible with almost all microcontrollers, whether they are older models or the latest ones. We absolutely love creative minds, so we've made sure that you can easily and quickly build a versatile, feature-rich command line for your next amazing project.
        </p>
    </div>
</div>
\endhtmlonly

## But isn't the command line a bit outdated?

\htmlonly
<div style="display:flex; align-items: center;">
    <div>
        <p>That's a completely valid question. The terminal might not be a new concept, but it has the advantage that the necessary protocols for its operation can now be decoded by almost any device, whether it's a Windows PC, a browser on a smartwatch, or even the built-in screen command in Linux. We designed Shellminator to use widely adopted interfaces, ensuring you can easily communicate with your project. It can operate over serial port, TCP, and WebSocket, so you can access your project from anywhere, even with just a browser.
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
        <p>Right? It doesn't seem that complicated, does it? It was very important to us to keep things simple. We really wanted beginner programmer padawans to be able to create amazing things easily with Shellminator. That's why we made sure the API is as simple and straightforward as possible.
        </p>
    </div>
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Rocket.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
</div>
\endhtmlonly

## We'd love for you to learn all the ins and outs

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Graduation-cap.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>No need to worry, we've put together a lot of well-structured, interactive, and exciting examples to help you get the most out of this system. We believe it's much easier to understand a program when you can try it out. Just for you, we've created a simulator embedded in the documentation, so you can test every example code. This way, you can see exactly what each function demonstrated in the examples does. Cool, right? You know what's even cooler? This demo right here:</p>
    </div>
</div>
\endhtmlonly

\htmlonly
<iframe id="demoFrame" src="webExamples/999_techDemo.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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
|                   | [Install](@ref installation_page) |
 
</div>