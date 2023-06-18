@page example_progress_advanced Progress - Advanced

This example shows how to create an advanced progress bar.

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

There is an option to customize your progress bar. You can define the done and the remaining characters for the bar.
You can use unicode characters like this:
```cpp

progress.drawProgressBar( i, "Counting Up...", "\u2588", " " );

```

The result will look like this:
```

-- Advanced Progress Bar Demo --
 44.0% ██████████████████                       | Counting Up...
```

if you want to get very fancy, you can specify the colors as well:
```cpp

progress.drawProgressBar( i, "Counting Up...", "\u2588", "-", Shellminator::GREEN, Shellminator::RED );

```

This example shows the advanced solution:
@include Progressbaradvanced.cpp