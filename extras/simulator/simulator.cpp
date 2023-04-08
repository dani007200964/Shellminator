#include <stdio.h>
#include <thread>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

#include "stdioStream.hpp"

stdioStream stdioChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &stdioChannel );


const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"

;


int main(){
  printf( "Simulator! Juhuuu!\r\n" );

  // Clear the terminal
  shell.clear();

  // Attach the logo.
  shell.attachLogo( logo );

  shell.begin( "arnold" );

  while( 1 ){

    stdioChannel.update();
    shell.update();

  }

  // The parameters to the function are put after the comma
  return 0;
}