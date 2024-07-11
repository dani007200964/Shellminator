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

#include "Shellminator-QR-Code-Module.hpp"

#ifdef SHELLMINATOR_ENABLE_QR_SUPPORT

    void ShellminatorQR::generate( Stream* channel_p, const char* text, enum qrcodegen_Ecc ecc ){

        // The result of the QR-code generation will be stored in this variable.
        bool result;

        // The actual size of the QR-code will be stored in this variable.
        uint32_t qr_size;

        // This variable helps to track the x direction while drawing the QR code.
        uint32_t x;

        // This variable helps to track the y direction while drawing the QR code.
        uint32_t y;

        // This variable will store the upper pixel while drawing.
        bool upper_pixel;

        // This variable will store the lower pixel while drawing.
        bool lower_pixel;

        // Detect if the channel was configured incorrectly.
        // In this case, we have to stop command execution.
        if( channel_p == NULL ){
            return;
        }

        // Generate the QR-code with default settings, and store the result to
        // the result variable.
        result = qrcodegen_encodeText(  text,
                                        qr_tempBuff,
                                        qr_data,
                                        ecc,
                                        qrcodegen_VERSION_MIN,
                                        qrcodegen_VERSION_MAX,
                                        qrcodegen_Mask_AUTO,
                                        true
                                        );

        // We have to check the result variable. If it is true,
        // the QR-code generation went well.
        if( !result ){
            return;
        }

        // Determinate the size of the QR-code.
        qr_size = qrcodegen_getSize( qr_data );

        // The unicode character table does not have a rectangular square,
        // but it has a half rectangular square on, top, bottom, and a full bar.
        // Check these links to make it more clear:
        //  -Full bar:      https://www.fileformat.info/info/unicode/char/2588/index.htm
        //  -Upper square:  https://www.fileformat.info/info/unicode/char/2580/index.htm
        //  -lower square:  https://www.fileformat.info/info/unicode/char/2584/index.htm
        // To draw a QR-code with a terminal emulator the easiest way to combine these
        // unicode characters. Because it is two 'pixels' high, we have to step the y
        // variable by two lines.
        for( y = 0; y < ( qr_size / 2 ); y++ ){

            // Print a new line at the begining of the horizontal drawing.
            channel_p -> print( "\r\n" );

            // Draw all horizontal 'pixels'.
            for( x = 0; x < qr_size; x++ ){

                // Determinate the upper pixel value.
                upper_pixel = qrcodegen_getModule( qr_data, x, ( y * 2 ) );

                // Determinate the lower pixel value.
                lower_pixel = qrcodegen_getModule( qr_data, x, ( ( y * 2 ) + 1 ) );

                // Draw the right pattern accordingly.

                // Both pixels are black.
                if( upper_pixel && lower_pixel ){

                    channel_p -> print( "\u2588" );
                    continue;

                }

                // Upper pixel is black.
                if( upper_pixel && ( !lower_pixel ) ){

                    channel_p -> print( "\u2580" );
                    continue;

                }

                // Lower pixel is black.
                if( ( !upper_pixel ) && lower_pixel ){

                    channel_p -> print( "\u2584" );
                    continue;

                }

                // If we get here we have to draw an empty bar.
                // The space character will do the job.
                channel_p -> print( " " );

            }

        }

        // If the QR code size is not even, we have to draw
        // the last line as well.
        if( ( qr_size % 2 ) != 0 ){

            // Print a new line at the begining of the horizontal drawing.
            channel_p -> print( "\r\n" );

            // Draw all horizontal 'pixels'.
            for( x = 0; x < qr_size; x++ ){

                // Determinate the pixel value. We store it to upper_pixel variable.
                upper_pixel = qrcodegen_getModule( qr_data, x, ( qr_size - 1 ) );

                // Check if we have to draw.
                if( upper_pixel ){

                    channel_p -> print( "\u2580" );
                    continue;

                }

                // If we get here we have to draw an empty bar.
                // The space character will do the job.
                channel_p -> print( " " );

            }

        }

        // Finally create a new line.
        channel_p -> print( "\r\n" );

    }

#else

    void ShellminatorQR::generate( Stream* channel_p, const char* text, enum qrcodegen_Ecc ecc ){

        // Detect if the channel was configured incorrectly.
        // In this case, we have to stop command execution.
        if( channel_p == NULL ){
            return;
        }

        #ifdef __AVR__
            channel_p -> println( __CONST_TXT__( "\r\nSorry! QR-Code functionality is not supported on AVR boards :(" ) );
        #else
            channel_p -> println( __CONST_TXT__( "\r\nNayuki QR-Code library is not included to the project!" ) );
        #endif


    }

#endif