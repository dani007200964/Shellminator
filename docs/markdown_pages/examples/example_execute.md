@page example_execute Execute

This example shows how to use the execution-function with the terminal object. If a terminal can't execute any command, it is useless.
The simplest way to connect the terminal interface to your code is to attach an execution-function to the terminal. It will be called
every time when a command typed and the return key is pressed.

There are two kind of supported execution functions:

```cpp

// Simple solution.
void executionFunction( char* command );

// Advanced solution.
void executionFunction( char* command, Shellminator* shell );

```

If you make a simple system, you can use the simple solution.
@note However it is recommended to use the advanced solution. It is more flexible, and you can only access the caller terminal
object this way.

@include Execute.cpp