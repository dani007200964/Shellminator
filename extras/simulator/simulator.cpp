#include <stdio.h>
#include <thread>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell();


const char logo[] =

"   _____ __         ____          _             __            \r\n"
"  / ___// /_  ___  / / /___ ___  (_)___  ____ _/ /_____  _____\r\n"
"  \\__ \\/ __ \\/ _ \\/ / / __ `__ \\/ / __ \\/ __ `/ __/ __ \\/ ___/\r\n"
" ___/ / / / /  __/ / / / / / / / / / / / /_/ / /_/ /_/ / /    \r\n"
"/____/_/ /_/\\___/_/_/_/ /_/ /_/_/_/ /_/\\__,_/\\__/\\____/_/     \r\n"
"\r\n\033[0;37m"
"Visit on GitHub:\033[1;32m https://github.com/dani007200964/Shellminator\r\n\r\n"

;

void foo()
{
  printf( "Hello from thread!" );
}


int main(){
    printf( "Simulator! Juhuuu!\r\n" );
    // The parameters to the function are put after the comma
    thread thread_obj(foo);
    thread_obj.join();
    return 0;
}