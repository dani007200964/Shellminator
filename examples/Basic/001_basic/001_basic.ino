
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
