@page 010_bannerText_page Example 010 Banner Text

@tableofcontents

Modern terminals often have a banner that can display useful information. For example, it might show which user is
currently logged in or which directory is active in the file system.

Unfortunately, Shellminator isn't quite advanced enough to manage users and file systems just yet. However, the good
news is that you have the option to modify the banner text anytime during runtime to suit your needs.
All you need to do is call the `setBannerText` or `setBannerPathText` functions with the desired text.

## Live Demo

\htmlonly
<iframe id="demoFrame" src="webExamples/010_bannerText.html" style="height:500px;width:100%;border:none;display:block;"></iframe>
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

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

// We need a buffer to store the characters from the input.
char nameBuffer[ SHELLMINATOR_BANNER_LEN ];
int nameBufferSize = SHELLMINATOR_BANNER_LEN;

char pathBuffer[ SHELLMINATOR_BANNER_PATH_LEN ];
int pathBufferSize = SHELLMINATOR_BANNER_PATH_LEN;


void pathCallback( char* buffer, int bufferSize, Shellminator* parent ){

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The path input is empty." );

        parent -> input( pathBuffer, pathBufferSize, "Please enter the path: ", pathCallback );
        return;
    }

    // Just in case terminate the string in the buffer.
    buffer[ pathBufferSize - 1 ] = '\0';

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Create a new prompt for the height.
    parent -> setBannerText( nameBuffer );
    parent -> setBannerPathText( pathBuffer );
}

void nameCallback( char* buffer, int bufferSize, Shellminator* parent ){

    if( buffer[ 0 ] == '\0' ){
        parent -> channel -> println();
        parent -> channel -> print( "The name input is empty." );

        parent -> input( nameBuffer, nameBufferSize, "Please enter your name: ", nameCallback );
        return;
    }

    // Just in case terminate the string in the buffer.
    buffer[ nameBufferSize - 1 ] = '\0';

    // Create a new prompt for the height.
    parent -> input( pathBuffer, pathBufferSize, "Please enter the path: ", pathCallback );
}


// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal to remove the empty input prompt.
    // Uncomment the line below and try it, if it is not clear
    // why this is needed.
    shell.clear();

    // Initialize shell object.
    shell.begin( "default" );

    // Generate an input prompt.
    shell.input( nameBuffer, nameBufferSize, "Please enter your name: ", nameCallback );

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
|[Beep Example](@ref 009_beep_page) | [Key Override Example](@ref 100_keyOverride_page) |
 
</div>