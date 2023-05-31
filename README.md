# Shellminator ![Build Status](https://github.com/dani007200964/Shellminator/actions/workflows/compile-examples.yml/badge.svg?branch=main)
![Coverage](/docs/images/coverage_badge.svg)

Shellminator is a simple-to-use terminal interface library. You can use it with any __VT-100__
terminal emulator like [PuTTY](https://www.putty.org/), [Terraterm](https://ttssh2.osdn.jp/index.html.en) or [minicom](https://linux.die.net/man/1/minicom).
With this library, you can create user-friendly command line interfaces for your embedded projects.
Shellminator has command history support, that means you can browse your previous commands with the arrow keys on the keyboard.
The library is Arduino compatible out of the box, but if you want to use it with other platforms,
you can do that in Shellminator-IO source files.

__Key changes in V1.2:__
* New shortcut keys.
* Option to set the timeout of the internal client.
* Advanced search functions( Ctrl-R, Page-Up, Page-Down )
* Banner text change option.
* Path text change option. Also change it to blue color
* Websocket channel
* Terminal buzzer command, with mute option
* QR-code generation
* History command
* Help command

__Breaking changes in V1.2:__
* The response channel is now uses the Stream class.
  This way it is more flexible and you doesn't have to
  create a class for every peripheral.

## Video Tutorial( currently for the older version )

[![Tutorial VIdeo](https://img.youtube.com/vi/O2su8kXg1X8/0.jpg)](https://www.youtube.com/watch?v=O2su8kXg1X8)

## Documentation

The full documentation can be found [here](https://dani007200964.github.io/Shellminator/html/index.html).

## Donation
If this project help you reduce time to develop, you can give me a cup of coffee :coffee: :coffee: :coffee:

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate?hosted_button_id=YFGZD78H6K2CS)

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License & copyright
© Daniel Hajnal

:email: hajnal.daniel96@gmail.com

Licensed under the [MIT License](LICENSE).
