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

#include <stdio.h>

// Shellminator library
#include "Shellminator.hpp"
#include "Shellminator-BufferedPrinter.hpp"

// Stream class for testing
#include "../testStream.hpp"

// Unit test module.
#include "unity.h"

// Use test Stream as Channel.
testStream testChannel;


uint8_t testBuffer[ 100 ];
int testBufferSize = sizeof( testBuffer );

class ShellminatorBufferedPrinterUT{
public:
    void test_empty_constructor();
    void test_constructor();
};

ShellminatorBufferedPrinterUT testerObject;

// Put any init code here that needs to run before testing.
void setUp(void) {

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void ShellminatorBufferedPrinterUT::test_empty_constructor(){

    ShellminatorBufferedPrinter testObject;

    TEST_ASSERT_NULL( testObject.channel );
    TEST_ASSERT_NULL( testObject.buffer );
    TEST_ASSERT_EQUAL_INT( 0, testObject.bufferSize );

}

void test_empty_constructor(){
    testerObject.test_empty_constructor();
}

void ShellminatorBufferedPrinterUT::test_constructor(){

    // Put some garbage into the buffer.
    testBuffer[ 0 ] = 1;
    testBuffer[ 1 ] = 2;
    testBuffer[ 2 ] = 3;

    ShellminatorBufferedPrinter testObject( &testChannel, testBuffer, testBufferSize );

    TEST_ASSERT_EQUAL_PTR( &testChannel, testObject.channel );
    TEST_ASSERT_EQUAL_PTR( testBuffer, testObject.buffer );
    TEST_ASSERT_EQUAL_INT( testBufferSize, testObject.bufferSize );
    TEST_ASSERT_EQUAL_INT( 0, testObject.bufferPointer );

    TEST_ASSERT_EACH_EQUAL_UINT8( '\0', testBuffer, sizeof( testBuffer ) );
    
}

void test_constructor(){
    testerObject.test_constructor();
}

int main(){

    UNITY_BEGIN();

    RUN_TEST( test_empty_constructor );
    RUN_TEST( test_constructor );

    return UNITY_END();

}