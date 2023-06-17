@page example_buffered Buffered

This example shows how to use the buffering feature of the library. It will try to allocate memory for a buffer
to accelerate the command rendering on the terminal side.

@note This method is using dynamic memory allocation in the setup process. It is not recommended on devices with low amount of dynamic memory.

If memory allocation fails for some reason, it will use the default method and it will still work. The allocation problem is detected and handled internally.
Of course it is detectable outside as well.

@include Buffered.cpp