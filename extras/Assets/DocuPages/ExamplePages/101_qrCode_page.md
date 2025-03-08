@page 101_qrCode_page Example 101 QR Code

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Fortunately, today, every smartphone is capable of scanning QR codes. This can be really useful when you’re out in the field and need to display an error code via a QR code,
        or even generate a unique link to a configuration portal for wireless devices. Luckily, we've implemented this feature for you—just provide a C string, and the QR code will be
        generated automatically.
        </p>
    </div>
</div>
\endhtmlonly

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/101_qrCode.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

The library is built on the [Nayuki QR code generation codebase](https://www.nayuki.io/page/qr-code-generator-library#c), which is a widely used solution in the industry.
We specifically use the C implementation, as it is the most memory-efficient.

And speaking of memory efficiency, unfortunately, this feature does require a relatively large amount of memory. The recommendation is to use it on platforms with at least 16kBytes of dynamic memory. Because of this, the Arduino Uno R3, unfortunately, isn't suitable. But the good news is that platforms like the ESP32, Raspberry Pi Pico, and Arduino Uno R4 can easily handle this limitation.

First, you’ll need to include the library:
```cpp
#include "Shellminator-QR-Code-Module.hpp"
```

Then, create an object that handles memory management:
```cpp
ShellminatorQR qrCode;
```

Finally, all you need to do is specify which channel the output should go to and what text it should contain:
```cpp
qrCode.generate( &Serial, "https://www.shellminator.org/html/index.html" );
```

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
#include "Shellminator-QR-Code-Module.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

ShellminatorQR qrCode;

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();


    Serial.println( "Shellminator webpage:" );
    Serial.println();

    // Generate a link to the Github repo.
    qrCode.generate( &Serial, "https://www.shellminator.org/html/index.html" );

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
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Light-bulb.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>You can optionally specify the level of error correction to be included in the QR code.
        </p>
    </div>
</div>
\endhtmlonly

| ECC Setting Definition | Tolerance                                              |
| ---------------------- | ------------------------------------------------------ |
| qrcodegen_Ecc_LOW      | The QR Code can tolerate about  7% erroneous codewords |
| qrcodegen_Ecc_MEDIUM   | The QR Code can tolerate about 15% erroneous codewords |
| qrcodegen_Ecc_QUARTILE | The QR Code can tolerate about 25% erroneous codewords |
| qrcodegen_Ecc_HIGH     | The QR Code can tolerate about 30% erroneous codewords |

By default, the medium setting is used, which allows for about 15% error tolerance. However, if you'd like to increase that to 30%, all you need to do is look up the setting name for 30% in the table, which is `qrcodegen_Ecc_HIGH`, and then modify the previous code like this:

```cpp
// Generate a link to the Github repo with 30% ECC
qrCode.generate( &Serial, "https://www.shellminator.org/html/index.html", qrcodegen_Ecc_HIGH );
```

<div class="section_buttons">
 
| Previous          |                         Next |
|:------------------|-----------------------------:|
|[Key Override Example](@ref 100_keyOverride_page) | [Buffering Example](@ref 102_buffering_page) |
 
</div>