/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SHELLMINATOR_QR_CODE_MODULE_HPP_
#define SHELLMINATOR_QR_CODE_MODULE_HPP_

    #include "Shellminator.hpp"

    #ifdef ARDUINO
        #include "Arduino.h"
    #else
        #include "System.h"
    #endif

    #include "Stream.h"
    #include "stdint.h"


    #ifdef SHELLMINATOR_ENABLE_QR_SUPPORT
        #include "qrcodegen.h"
    #else
        // If the Nayuki library is not included, we have to create this enumeration
        // for compatibility reasons.
        enum qrcodegen_Ecc {
            qrcodegen_Ecc_LOW = 0 ,  // The QR Code can tolerate about  7% erroneous codewords
            qrcodegen_Ecc_MEDIUM  ,  // The QR Code can tolerate about 15% erroneous codewords
            qrcodegen_Ecc_QUARTILE,  // The QR Code can tolerate about 25% erroneous codewords
            qrcodegen_Ecc_HIGH    ,  // The QR Code can tolerate about 30% erroneous codewords
        };
    #endif

    /// QR-Code generator
    ///
    /// With this class, you can generate a QR code directly
    /// to the command line. Big thanks to Nayuki for
    /// [this](https://github.com/nayuki/QR-Code-generator)
    /// awesome library. The C implementation from it is
    /// required to make this object work.
    /// @note This object requires a relatively large amount
    ///       of dynamic memory. Sadly AVR boards are not
    ///       supported due to the lack of memory.
    class ShellminatorQR{

    public:

        /// Generates a QR-Code.
        ///
        /// Generates a QR-Code to the specified channel.
        /// @param channel_p Pointer to the output Stream object.
        /// @param text QR-Code will be generated from this text.
        ///             It has to be NULL terminated.
        /// @param ecc Error correction level. Possible options:
        ///            1. qrcodegen_Ecc_LOW - tolerate ~ __7%__ error
        ///            2. qrcodegen_Ecc_MEDIUM - tolerate ~ __15%__ error
        ///            3. qrcodegen_Ecc_QUARTILE - tolerate ~ __25%__ error
        ///            4. qrcodegen_Ecc_HIGH - tolerate ~ __30%__ error
        void generate( Stream* channel_p, const char* text, enum qrcodegen_Ecc ecc = qrcodegen_Ecc_MEDIUM );

    private:
        #ifdef SHELLMINATOR_ENABLE_QR_SUPPORT
            uint8_t qr_data[ qrcodegen_BUFFER_LEN_MAX ];
            uint8_t qr_tempBuff[ qrcodegen_BUFFER_LEN_MAX ];
        #endif

    };
    
#endif