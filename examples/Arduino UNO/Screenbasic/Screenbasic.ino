
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
#include "Shellminator-IO.hpp"
#include "Shellminator-Screen.hpp"


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
    "Visit on GitHub: https://github.com/dani007200964/Shellminator\r\n\r\n"

;

class teszt : public ShellminatorScreen{

public:
    void draw( Shellminator* parent, int width, int  height ) override;
    void init()override;
private:
    int cntr;
};

void teszt::draw( Shellminator* parent, int width, int  height ){
    parent -> clear();
    parent -> channel -> print( "Counter: " );
    parent -> channel -> print( cntr );
    cntr++;
    if( cntr >= 100 ){
        parent -> endScreen();
    }
}

void teszt::init(){
    cntr=0;
}

teszt X;




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the logo.
    shell.attachLogo( logo );

    // Initialize shell object.
    shell.begin( "arnold" );
    shell.beginScreen( &X );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
