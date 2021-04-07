# Shellminator

Shellminator is a simple to use library and helps to interface with VT100 compatible
terminal emulators like [PuTTY](https://www.putty.org/), [Terraterm](https://ttssh2.osdn.jp/index.html.en) or [minicom](https://linux.die.net/man/1/minicom).
With this library you can create user friendly command line interfaces for your embedded projects.
Shellminator has history support, that means you can brows your prevoius commands with the arrow keys on the keyboard.
The library is Arduino compatible out of the box, but if you want to use it with other platforms,
you just have to implement an [Arduino compatible Serial library](https://www.arduino.cc/reference/en/language/functions/communication/serial/).
Configuring the library is just a few minutes, and this documentation shows how to do that correctly.

# Doxygen documentation

The project has a Doxygen generated documentation. It can be found in Doc/html/index.html.
The theme that used with the documentation can be found [here](https://jothepro.github.io/doxygen-awesome-css/).

# Arduino installation

__1.__

Download the .zip file from the Arduino folder from this repository.

__2.__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/arduino_install_unzip.png)
Open the downloaded .zip file and copy the Shellminator folder from the zip into your Arduino library folder.
__The Arduino library folder usually located in '\Documents\Arduino\libraries\'.__

__3.__

If Arduino IDE is opened __save all your work,__ then restart Arduino IDE.

__4.__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/arduino_install_check_library.png)
Now you should see the Shellminator library in the examples.

__5.__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/arduino_install_putty_test.png)
Now pick an Arduino board and upload the __Simple__ example to it. You will need a terminal emulator. In this example I will use [PuTTY](https://www.putty.org/).

Congratulations you successfully installed the Shellminator library to your computer.

# Usage with Arduino

## basic_example Basic example

__1.__

The first thing you have to do is to include the library at the begining of your program.

Code:
```
#include "Shellminator.hpp"
```

__2.__

After you included the library you have to create an object of it. Usually it has to be made before setup().

Code:
```
Shellminator shell( &Serial );
```

The code above creates an object called shell and gives it Serial as a communication channel.

__3.__

In the __setup__ function you have to initialise the Serial object that you have passed to shell.

Code:
```
Serial.begin( 115200 );
while( !Serial );
```

In this example the Serial is configured with 115200 baudratem, but you can change it.

__4.__

After you initialised the serial port, it is very handy to clear the terminal. In my opinion it looks better, but this step is optional.

Code:
```
shell.clear();
```

The clear command not deletes the content of the terminal, it just scrolls down to make it looks like that the terminal is empty.
If you scroll up you will see the previus data.

__5.__

Now it is time to start the shell.

Code:
```
shell.begin( "arnold" );
```

The begin function initializes the object, but you have to feed it with characters, it is not automatic. The only argument for begin is the banner text.
This text can be your name, your companys name, your pets name, or simply empty( "" ).

__6.__

The last thing to do to make everything work is to put the update function in a periodic function. In Arduino environment the loop function is suitable for the job.

Code:
```
void loop() {

  shell.update();

}
```

You have to update quick enough to make sure that the Serial buffer wont overflow. But calling too frequently wastes your CPU time.

This code can be found in the examples as Simple.ino.

Code:
```
/*
* Created on Aug 10 2020
*
* Copyright (c) 2020 - Daniel Hajnal
* hajnal.daniel96@gmail.com
*
* This file is part of the Shellminator project.
*/

/// This is the bare minimum configuration to make Shellminator work.
/// All parameters are default.

#include "Shellminator.hpp"

/// Create a Shellminator object, and initialise it to use Serial
Shellminator shell( &Serial );

void setup() {

  /// Initialise Serial with 115200 baudrate
  Serial.begin( 115200 );

  /// Wait for connection
  while( !Serial );

  /// Clear the terminal
  shell.clear();

  /// Wait 2 seconds before initialising the program.
  /// It is optional, just simply give you some time to open
  /// a terminal application after sketch upload.
  delay( 2000 );

  /// Print start message
  Serial.println( "Program begin..." );

  /// Initialise shell object.
  shell.begin( "arnold" );

}

void loop() {

  /// Feed Shellminator's update function.
  shell.update();

}
```

##  Create costum logo

__1.__

The first step is the most difficult. You have to came up with an idea what you want to see at startup.

__2.__

After you have came up with the startup text, go to [this](https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino) website.

__3.__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/create_costum_logo_ascii_art_generator.png)
In this webpage you can generate all sorts of ASCII art. Just follow the steps in the image abowe.

__4.__

After you generated and copied the ASCII text art, go to [this](https://tomeko.net/online_tools/cpp_text_escape.php?lang=en) website.

__5.__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/create_costum_logo_c_string_generator.png)
In this webpage you can generate a c/c++ like string from regular text. Just follow the steps in the image abowe.

__6.__

Paste the generated c/c++ string to your code like this:

```
const char *Shellminator::logo =
"    ___             __      _           \n"
"   /   |  _________/ /_  __(_)___  ____ \n"
"  / /| | / ___/ __  / / / / / __ \\/ __ \\\n"
" / ___ |/ /  / /_/ / /_/ / / / / / /_/ /\n"
"/_/  |_/_/   \\__,_/\\__,_/_/_/ /_/\\____/ \n"
"                                        \n"
;
```

The only problem with this string is that the line termination is only a "\n" character.
We need to add a "\r" character before every "\n" character to make it pretty in the final result.

Code:

```
const char *Shellminator::logo =
"    ___             __      _           \r\n"
"   /   |  _________/ /_  __(_)___  ____ \r\n"
"  / /| | / ___/ __  / / / / / __ \\/ __ \\\r\n"
" / ___ |/ /  / /_/ / /_/ / / / / / /_/ /\r\n"
"/_/  |_/_/   \\__,_/\\__,_/_/_/ /_/\\____/ \r\n"
"                                        \r\n"
;
```

Example code in one peace:

```
#include "Shellminator.hpp"

/// To create costum startup logo: https://patorjk.com/software/taag/#p=display&f=Slant&t=Arduino
/// To make it to a c-string: https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
/// Add '\r' to all line end.
const char *Shellminator::logo =
"    ___             __      _           \r\n"
"   /   |  _________/ /_  __(_)___  ____ \r\n"
"  / /| | / ___/ __  / / / / / __ \\/ __ \\\r\n"
" / ___ |/ /  / /_/ / /_/ / / / / / /_/ /\r\n"
"/_/  |_/_/   \\__,_/\\__,_/_/_/ /_/\\____/ \r\n"
"                                        \r\n"
;

/// Create a Shellminator object, and initialise it to use Serial
Shellminator shell( &Serial, execution_function );

void setup(){
  .
  .
  .
}
```

## Add execution function

In a real life application the execution function is the most important function in your shell subsystem.
This functions job is to process the command that has been recived.
Shellminator is just a terminal interface, it does not process any command, just pass it to the execution function.
But it can be used as a glue between your terminal application and your interpreter( execution function ).

__The shape of the execution function__

The execution function is a void return type and has only one argument, and that argument is a char* type.

For example:

```
void execution_function( char *cmd );
```

The name of the function and its argument can be anything, but the __types has to match!__

Also good example:

```
void please_do_what_i_want( char *order );
```


Adding an execution function to a Shellminator object is simple. You have two options for that.

__1.__ Use the constructor:

```
Shellminator shell( &Serial, execution_function );
```

__2.__ Use a dedicated function:

```
addExecFunc( execution_function );
```

This function can be used also when you have to change the execution function runtime.

__Example__

![](https://github.com/dani007200964/Shellminator/blob/main/Doc/images/arduino_exec_func_test.png)
In the examples the Advanced.ino shows a basic implementation of a command parser.
It not very efficient but if you have a small amount of commands and a simple project, it will be enough.
You can easily modify it to add your costum commands.

## Configure the library

There are some parameters that can be changed in the library.

__1.__

To make this changes first you have to open the Shellminator.hpp file.
In the Arduino environment it can be located under your libraries\Shellminator folder.
__The Arduino library folder usually located in '\Documents\Arduino\libraries\'.__
You will need a good text editor for this. I can recommend [Atom](https://atom.io/) or [Notepad++](https://notepad-plus-plus.org/downloads/) for example.

__2.__

After you opened the header file, in the begining of the file you can find a section like this:

```
///  +------  Costum configuration  ------+
///  |                                    |
///  |  This is where you have to config  |
///  |           your defines!            |
///  |                                    |
///  +------------------------------------+


/// #define SHELLMINATOR_SERIAL_CLASS           // default: Serial_
/// #define SHELLMINATOR_BUFF_LEN               // default: 20
/// #define SHELLMINATOR_BUFF_DIM               // default: 5
/// #define SHELLMINATOR_BANNER_LEN             // default: 20
/// #define SHELLMINATOR_LOGO_FONT_STYLE        // default: BOLD
/// #define SHELLMINATOR_LOGO_COLOR             // default: RED
```

This is the section of the header that can modify the default configuration.
You can see the default values for every parameter.
If you want to change any of them, just uncomment it, __and give it a value!__
For example:

```
///  +------  Costum configuration  ------+
///  |                                    |
///  |  This is where you have to config  |
///  |           your defines!            |
///  |                                    |
///  +------------------------------------+


/// #define SHELLMINATOR_SERIAL_CLASS           // default: Serial_
/// #define SHELLMINATOR_BUFF_LEN               // default: 20
#define SHELLMINATOR_BUFF_DIM 10                // default: 5
/// #define SHELLMINATOR_BANNER_LEN             // default: 20
/// #define SHELLMINATOR_LOGO_FONT_STYLE        // default: BOLD
/// #define SHELLMINATOR_LOGO_COLOR             // default: RED
```

The modification above expands the history to 9 element.

## Changing text style and color

If you want to highlight something or just make your messages pretty this can be a useful function.

Code:

```
shell.setTerminalCharacterColor( Shellminator::BOLD, Shellminator::RED );
```

Available styles: REGULAR, BOLD, LOW_INTENSITY, ITALIC, UNDERLINE, BLINKING, REVERSE, BACKGROUND, INVISIBLE

Available colors: BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.


## License & copyright
© Daniel Hajnal
Licensed under the [MIT License](LICENSE).
