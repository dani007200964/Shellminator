@page qr_code_support QR-code support

The library can generate QR-codes and draw them directly to the terminal screen.
To enable this functionality ypu have to follow the steps below.

1. Clone [nayuki's QR-Code-generator C library](https://github.com/nayuki/QR-Code-generator/tree/master/c) to your computer.
2. Copy __qrcodegen.c__ and __qrcodegen.h__ files to your project.
3. Open Shellminator.hpp and uncomment this line: __define SHELLMINATOR_ENABLE_QR_SUPPORT__

Now you can generate QR-codes with __generateQRText()__ function.
