@page 006_password_page Example 006 Password

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:100px; height:100px; margin-right: 20px;">
        <lottie-player src="Lock.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>There are situations where it might be a good idea to restrict access for unauthorized users. It doesn't mean anything bad is expected, but it's always wise to prevent any unwanted incidents that could occur if someone has free reign in the system.
        </p>
    </div>
</div>
\endhtmlonly

\htmlonly
<iframe id="demoFrame" src="webExamples/006_password.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

The first thing you'll need is a password hash. A password hash is a series of bytes generated from the password. What's the point of this? If we wanted to check the password directly to see if it's correct, we'd have to save it in the program memory. A hacker could easily read it from there and break into the system in no time. However, if we save the hash calculated from the password instead of the password itself, it becomes much harder for anyone to reverse-engineer our system.

The library primarily calculates a CRC32 hash from the password and uses it for 'encryption'. We chose this method because we wanted to find a solution that is secure enough without requiring significant computational power. Of course, if you need a more secure method, you can always replace our implementation with your own using a hook, which we'll cover in the advanced section.

To make things easier for you, we've also created a calculator that computes the hash needed for your password.
\htmlonly
<iframe id="pwHashGenerator" src="pw_hash_generator.html" style="height:300px;width:100%;border:none;display:block;"></iframe>
\endhtmlonly

After using the calculator to compute the hash of your password, you need to save the hash byte-by-byte into an array. We've calculated the worst possible password hash for you, which is for 'Password'. The corresponding CRC32 hash is 0xCC, 0xB4, 0x24, 0x83.

```cpp
uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };
```

Next, all we need to do is assign the password to the shell in the setup section, which we can do using the `setPassword` method. The `setPassword` function will need the hash array and the size of the array in bytes.

```cpp
shell.setPassword( passwordHash, sizeof( passwordHash ) );
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
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup for Shellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/

#include "Shellminator.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

uint8_t passwordHash[] = { 0xCC, 0xb4, 0x24, 0x83 };

// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    Serial.print( "What could be the password? Maybe " );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::YELLOW );
    Serial.print( "Passwod");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "?" );

    Serial.print( "Oh, and please log out after you finished with" );
    shell.format( &Serial, Shellminator::BOLD, Shellminator::BG_WHITE, Shellminator::BLACK );
    Serial.print( " Ctrl-D ");
    shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
    Serial.println( "key!" );

    // Enable password protection.
    shell.setPassword( passwordHash, sizeof( passwordHash ) );


    // Initialize shell object.
    shell.begin( "arnold" );

}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();

}
```