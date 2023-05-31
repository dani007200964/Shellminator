/*
 * Created on April 08 2023
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.04.08
*/

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

/*


This code is used for unit test. It will test the following test cases:

    - sencBackspace() funcion.

    - clear() funcion.

    - setTerminalCharacterColor( uint8_t style, uint8_t color ) function:
        * formatting enabled
        * formatting disabled

    - setTerminalCharacterColor( char* buff, uint8_t buffSize, uint8_t style, uint8_t color ) function:
        * buffer is valid address
        * buffer is NULL
        * buffer size is less, than data

    - setTerminalCharacterColor( char* buff, uint8_t style, uint8_t color ) function:
        * formatting enabled
        * formatting disabled
        * buffer is valid address
        * buffer is NULL

    - setTerminalCharacterColor( Stream *stream_p, uint8_t style, uint8_t color ) function:
        * stream_p is valid address
        * stream_p is NULL

    - hideCursor() function:
        * formatting enabled
        * formatting disabled

    - hideCursor( char* buff, uint8_t bufferSize ) function:
        * buffer is valid address
        * buffer is NULL
        * buffer size is less, than data

    - hideCursor( Stream *stream_p ) function:
        * stream_p is valid address
        * stream_p is NULL

    - showCursor() function:
        * formatting enabled
        * formatting disabled

    - showCursor( char* buff, uint8_t bufferSize ) function:
        * buffer is valid address
        * buffer is NULL
        * buffer size is less, than data

    - showCursor( Stream *stream_p ) function:
        * stream_p is valid address
        * stream_p is NULL

*/

#include <stdio.h>

// Shellminator library
#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"
#include "Shellminator-BufferedPrinter.hpp"

// Stream class for testing
#include "../testStream.hpp"

// Unit test module.
#include "unity.h"

// Use stdio as Channel.
testStream testChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &testChannel );

ShellminatorBufferedPrinter printer;
int printerBufferSize = 100;


// Put any init code here that needs to run before testing.
void setUp(void) {

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void test_backspace_sending(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send backspace command.
    shell.sendBackspace();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\b \b", testChannel.txBuffer );

}

void test_clear(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send backspace command.
    shell.clear();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[H\033[J", testChannel.txBuffer );

}

void test_setTerminalCharacterColor_simple(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 20 ] = "";

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Clear the Tx buffer before the test.
            testChannel.clearTx();

            // Send Style command.
            shell.setTerminalCharacterColor( styleVariants[ i ], colorVariants[ j ] );

            // Check if formatting is enabled.
            if( shell.enableFormatting ){

                // Create reference string.
                snprintf( expected, sizeof( expected ), "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            }

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

        }

    }

}

void test_setTerminalCharacterColor_buffer(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 20 ];
    char result[ 20 ];

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Send Style command.
            Shellminator::setTerminalCharacterColor( result, sizeof( result ), styleVariants[ i ], colorVariants[ j ] );

            // Create reference string.
            snprintf( expected, sizeof( expected ), "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, result );

        }

    }

}

void test_setTerminalCharacterColor_buffer_less(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 3 ] = { '\0', '\0', '\0' };
    char result[ 3 ] = { '\0', '\0', '\0' };

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Send Style command.
            Shellminator::setTerminalCharacterColor( result, sizeof( result ) - 1, styleVariants[ i ], colorVariants[ j ] );

            // Create reference string.
            snprintf( expected, sizeof( expected ) - 1, "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, result );

        }

    }

}

void test_setTerminalCharacterColor_buffer_invalid(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 20 ];
    char *result = NULL;

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Send Style command.
            Shellminator::setTerminalCharacterColor( result, 1, styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)NULL, result );

        }

    }

}

void test_setTerminalCharacterColor_buffer_legacy(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 20 ] = "";
    char result[ 20 ];

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Send Style command.
            shell.setTerminalCharacterColor( result, styleVariants[ i ], colorVariants[ j ] );

            // Check if formatting is enabled.
            if( shell.enableFormatting ){

                // Create reference string.
                snprintf( expected, sizeof( expected ), "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            }

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, result );

        }

    }

}

void test_setTerminalCharacterColor_buffer_legacy_invalid(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char *result = NULL;

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Send Style command.
            shell.setTerminalCharacterColor( result, styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)NULL, result );

        }

    }

}

void test_setTerminalCharacterColor_stream(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    char expected[ 20 ];

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Clear the Tx buffer before the test.
            testChannel.clearTx();

            // Send Style command.
            Shellminator::setTerminalCharacterColor( &testChannel, styleVariants[ i ], colorVariants[ j ] );

            // Create reference string.
            snprintf( expected, sizeof( expected ), "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

        }

    }

}

void test_setTerminalCharacterColor_stream_invalid(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };
    

    int i;
    int j;

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Clear the Tx buffer before the test.
            testChannel.clearTx();

            // Send Style command.
            Shellminator::setTerminalCharacterColor( (Stream*)NULL, styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)"", testChannel.txBuffer );

        }

    }

}

void test_hideCursor_simple(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.hideCursor();

    // Check if formatting is enabled.
    if( shell.enableFormatting ){

        // Check the buffer content. It should match with the known data string.
        TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25l", testChannel.txBuffer );

    }

    else{

        // Check the buffer content. It should match with the known data string.
        TEST_ASSERT_EQUAL_STRING ( (const char*)"", testChannel.txBuffer );

    }

}

void test_hideCursor_buffered(void) {

    char result[ 20 ];

    // Send cursor hide command.
    shell.hideCursor( result, sizeof( result ) );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25l", result );


}

void test_hideCursor_buffered_less(void) {

    char result[ 3 ] = { '\0', '\0', '\0' };

    // Send cursor hide command.
    shell.hideCursor( result, sizeof( result ) - 1 );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[", result );


}

void test_hideCursor_buffered_invalid(void) {

    char *result = NULL;

    // Send cursor hide command.
    shell.hideCursor( result, sizeof( result ) );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)NULL, result );


}

void test_hideCursor_stream(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.hideCursor( &testChannel );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25l", testChannel.txBuffer );

}

void test_hideCursor_stream_invalid(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.hideCursor( (Stream*)NULL );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"", testChannel.txBuffer );

}

void test_showCursor_simple(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.showCursor();

    // Check if formatting is enabled.
    if( shell.enableFormatting ){

        // Check the buffer content. It should match with the known data string.
        TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25h", testChannel.txBuffer );

    }

    else{

        // Check the buffer content. It should match with the known data string.
        TEST_ASSERT_EQUAL_STRING ( (const char*)"", testChannel.txBuffer );

    }


}

void test_showCursor_buffered(void) {

    char result[ 20 ];

    // Send cursor hide command.
    shell.showCursor( result, sizeof( result ) );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25h", result );


}

void test_showCursor_buffered_invalid(void) {

    char *result = NULL;

    // Send cursor hide command.
    shell.showCursor( result, sizeof( result ) );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)NULL, result );


}

void test_showCursor_buffered_less(void) {

    char result[ 3 ] = { '\0', '\0', '\0' };

    // Send cursor hide command.
    shell.showCursor( result, sizeof( result ) - 1 );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[", result );


}

void test_showCursor_stream(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.showCursor( &testChannel );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"\033[?25h", testChannel.txBuffer );

}

void test_showCursor_stream_invalid(void) {

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Send cursor hide command.
    shell.showCursor( (Stream*)NULL );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)"", testChannel.txBuffer );

}

void test_setTerminalCharacterColor_bufferedPrinter(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };

    int i;
    int j;

    char expected[ 20 ];

    printer = ShellminatorBufferedPrinter( &testChannel, printerBufferSize );
    TEST_ASSERT_EQUAL_INT_MESSAGE( printerBufferSize, printer.getBufferSize(), "Memory Allocation Failed!" );


    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Clear the Tx buffer before the test.
            testChannel.clearTx();

            // Send Style command.
            Shellminator::setTerminalCharacterColor( &printer, styleVariants[ i ], colorVariants[ j ] );
            printer.flush();
            //printer.~ShellminatorBufferedPrinter();

            // Create reference string.
            snprintf( expected, sizeof( expected ), "\033[%d;%dm", styleVariants[ i ], colorVariants[ j ] );

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

        }

    }

}

void test_setTerminalCharacterColor_bufferedPrinter_invalid(void) {

    int styleVariants[] = {
        Shellminator::REGULAR,
        Shellminator::BOLD,
        Shellminator::LOW_INTENSITY,
        Shellminator::ITALIC,
        Shellminator::UNDERLINE,
        Shellminator::BLINKING,
        Shellminator::REVERSE,
        Shellminator::BACKGROUND,
        Shellminator::INVISIBLE
    };

    int colorVariants[] = {
        Shellminator::BLACK,
        Shellminator::RED,
        Shellminator::GREEN,
        Shellminator::YELLOW,
        Shellminator::BLUE,
        Shellminator::MAGENTA,
        Shellminator::CYAN,
        Shellminator::WHITE
    };

    int i;
    int j;

    char expected[ 20 ] = "";

    // Check for every combination
    // i -> style
    // j -> color
    for( i = 0; i < ( sizeof( styleVariants ) / sizeof( styleVariants[ 0 ] ) ); i++ ){

        // Check for every combination
        for( j = 0; j < ( sizeof( colorVariants ) / sizeof( colorVariants[ 0 ] ) ); j++ ){

            // Clear the Tx buffer before the test.
            testChannel.clearTx();

            // Send Style command.
            Shellminator::setTerminalCharacterColor( (ShellminatorBufferedPrinter*)NULL, styleVariants[ i ], colorVariants[ j ] );
            printer.flush();
            //printer.~ShellminatorBufferedPrinter();

            // Check the buffer content. It should match with the known data string.
            TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

        }

    }

}

int main(){

    UNITY_BEGIN();

    // Run the tests with formatting enabled.
    shell.enableFormatting = true;

    RUN_TEST( test_backspace_sending );
    RUN_TEST( test_clear );
    RUN_TEST( test_setTerminalCharacterColor_simple );
    RUN_TEST( test_setTerminalCharacterColor_buffer );
    RUN_TEST( test_setTerminalCharacterColor_buffer_less );
    RUN_TEST( test_setTerminalCharacterColor_buffer_legacy );
    RUN_TEST( test_setTerminalCharacterColor_buffer_legacy_invalid );
    RUN_TEST( test_setTerminalCharacterColor_stream );
    RUN_TEST( test_setTerminalCharacterColor_stream_invalid );
    RUN_TEST( test_setTerminalCharacterColor_buffer );
    RUN_TEST( test_setTerminalCharacterColor_buffer_invalid );
    RUN_TEST( test_setTerminalCharacterColor_bufferedPrinter );
    RUN_TEST( test_setTerminalCharacterColor_bufferedPrinter_invalid );
    RUN_TEST( test_hideCursor_simple );
    RUN_TEST( test_hideCursor_buffered );
    RUN_TEST( test_hideCursor_buffered_invalid );
    RUN_TEST( test_hideCursor_buffered_less );
    RUN_TEST( test_hideCursor_stream );
    RUN_TEST( test_hideCursor_stream_invalid );
    RUN_TEST( test_showCursor_simple );
    RUN_TEST( test_showCursor_buffered );
    RUN_TEST( test_showCursor_buffered_invalid );
    RUN_TEST( test_showCursor_buffered_less );
    RUN_TEST( test_showCursor_stream );
    RUN_TEST( test_showCursor_stream_invalid );

    // Disable formatting to and repeat relevant tests.
    shell.enableFormatting = false;

    RUN_TEST( test_setTerminalCharacterColor_simple );
    RUN_TEST( test_setTerminalCharacterColor_buffer_legacy );
    RUN_TEST( test_setTerminalCharacterColor_buffer_legacy_invalid );
    RUN_TEST( test_hideCursor_simple );
    RUN_TEST( test_showCursor_simple );

    return UNITY_END();

}