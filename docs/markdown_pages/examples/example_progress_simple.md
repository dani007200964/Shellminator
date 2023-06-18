@page example_progress_simple Progress - Simple

This example shows how to create a simple progress bar.

There are two ways to create a progress bar.

```cpp

// With a Stream object( this is the simplest )
ShellminatorProgress progress( &Serial );

// With a Shellminator object.
Shellminator shell( &Serial );
ShellminatorProgress progress( &shell );

```

If you make a simple system, you can use the simple solution with a Stream object, like Serial.
@note However it is recommended to use the advanced solution. It is more flexible, and it can be used with buffering.

The result will look like this:
```

-- Simple Progress Bar Demo --
 39.0% ################------------------------ | Test progress
```

This example shows the simple solution:
@include Progressbarsimple.cpp