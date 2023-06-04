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

// Use test Stream as Channel.
testStream testChannel;

ShellminatorBufferedPrinter printer;
int printerBufferSize = 30;


// Put any init code here that needs to run before testing.
void setUp(void) {

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void test_small_buffer_size(void) {

    // Test too small buffer size.
    printer = ShellminatorBufferedPrinter( &testChannel );
    printer.allocate( 10 );
    TEST_ASSERT_EQUAL_INT( -1, printer.getBufferSize() );

}

void test_set_get_channel(void) {

    printer.setChannel( &testChannel );
    TEST_ASSERT_EQUAL_PTR( &testChannel, printer.getChannel() );

}

void test_normal_constructor(void) {

    // Test normal buffer size.
    printer = ShellminatorBufferedPrinter( &testChannel );
    printer.allocate( printerBufferSize );
    TEST_ASSERT_EQUAL_INT( printerBufferSize, printer.getBufferSize() );

}

void test_printf(void) {

    const char *data = "Hello World!";

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer.printf( data );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( data, testChannel.txBuffer );

}

void test_printf_large(void) {

    const char *data1 = "Hello World!";
    const char *data2 = "I like coffee!";
    const char *data3 = "I like to move it!";
    const char *data4 = "Merry Christmas!";

    char expected[ 200 ];

    snprintf( expected, sizeof( expected ), "%s%s%s%s", data1, data2, data3, data4 );

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer.printf( data1 );
    printer.printf( data2 );
    printer.printf( data3 );
    printer.printf( data4 );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

}

void test_printf_too_large(void) {

    char data[ 200 ];
    int i;
    char c = 'A';

    for( i = 0; i < sizeof( data ); i++ ){

        if( c > 'z' ){
            c = 'A';
        }

        data[ i ] = c;

        c++;

    }

    data[ sizeof( data ) - 1 ] = '\0';

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer.printf( data );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_printf_invalid(void) {

    const char *data = "Hello World!";

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer.printf( data );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_set_channel_invalid(void) {

    const char *data = "Hello World!";

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer.setChannel( (Stream*)NULL );

    printer.printf( data );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_constructor_invalid(void) {

    const char *data = "Hello World!";

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    printer = ShellminatorBufferedPrinter( (Stream*)NULL );
    printer.allocate( printerBufferSize );

    printer.printf( data );
    printer.flush();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_deallocate(){

    printer.deallocate();

}

int main(){

    UNITY_BEGIN();

    // Test invalid buffer cases.
    RUN_TEST( test_small_buffer_size );
    RUN_TEST( test_printf_invalid );

    // Test normal cases
    RUN_TEST( test_set_get_channel );
    RUN_TEST( test_normal_constructor );
    RUN_TEST( test_printf );
    RUN_TEST( test_printf_large );
    RUN_TEST( test_printf_too_large );
    RUN_TEST( test_constructor_invalid );
    RUN_TEST( test_deallocate );
    RUN_TEST( test_deallocate );

    return UNITY_END();

}