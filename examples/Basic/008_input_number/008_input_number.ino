
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

// We need a buffer to store the characters from the input.
char inputBuffer[ 30 ];

// These variables will store the numbers from the user input.
int width = 0;
int height = 0;


// This function will be called when the height input finishes.
void heightCallback( char* buffer, int bufferSize, Shellminator* parent ){

    // The scanf like functions will return with a number. This number represents
    // how many tokens are processed correctly. We will store the result of
    // the sscanf process to this variable.
    int scan_result;

    // Try to process one number token and save the result to the height variable.
    scan_result = sscanf( buffer, "%d", &height );

    // Check for user errors. Firstly check if the input string was empty.
    if( buffer[ 0 ] == '\0' ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // We only wanted to process exactly one number. Check if the result is not 1.
    if( scan_result != 1 ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // Also, check if the parsed number is correct. This case if it is negative or zero,
    // something went wrong.
    if( height <= 0 ){
        // This case inform the user about the problem and restart the input.
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
        return;
    }

    // Calculate the area
    int area = width * height;

    // Print the result
    parent -> channel -> println();
    parent -> channel -> print( "The area of the plane is: " );
    parent -> channel -> print( area );
    parent -> channel -> print( "mm2" );
}

// This function will be called when the width input finishes.
void widthCallback( char* buffer, int bufferSize, Shellminator* parent ){

    // This works exactly like the upper callback. The only difference is that,
    // when this callback finishes, it will start the next input.
    int scan_result;

    scan_result = sscanf( buffer, "%d", &width );

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The input is empty. The input must be a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( scan_result != 1 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a number. " );
        parent -> channel -> print( buffer );
        parent -> channel -> print( " is not a number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    if( width <= 0 ){
        parent -> channel -> println();
        parent -> channel -> print( "The input must be a positive, non-zero number." );

        parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );
        return;
    }

    // Create a new prompt for the height.
    parent -> input( inputBuffer, sizeof( inputBuffer ), "Plane height[ mm ]: ", heightCallback );
}


// System init section.
void setup(){

    Serial.begin(115200);

    // Initialize shell object.
    shell.begin( "arnold" );

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Generate an input prompt.
    shell.input( inputBuffer, sizeof( inputBuffer ), "Plane width[ mm ]: ", widthCallback );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
