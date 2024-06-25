@page introduction_page Introduction

## The most versatile terminal interface for your next robot

Shellminator is an innovative terminal interface specially designed for embedded devices. Written in C++, it is fully compatible with the Arduino development environment. We absolutely love creative minds, so we've made sure that you can easily and quickly build a versatile, feature-rich command line for your next amazing project 🚀

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

Right? It doesn't seem that complicated, does it? It was very important to us to keep things simple if that's what you prefer. After all, if you don't appreciate the small things, you don't deserve the big ones. But of course, if you love taking things to a more complex level, the sky's the limit!

## Let me show you what I mean


\htmlonly

<iframe id="demoFrame" src="webExamples/200_commanderBasic.html" style="height:500px;width:100%;border:1px solid #38393b;border-radius: 4px;display:block;"></iframe>
\endhtmlonly