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
    - stdioChannel pushTX: This is used to test the Stream object Tx buffer push functionality.

*/

#include <stdio.h>

// Shellminator library
#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Stream class for testing
#include "../testStream.hpp"

// Unit test module.
#include "unity.h"

// Use stdio as Channel.
testStream testChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &testChannel );

// Simple logo.
const char logo[] = "Shellminator Unit Test\r\n";

// Put any init code here that needs to run before testing.
void setUp(void) {

    // Attach logo to test it as well.
    shell.attachLogo( logo );

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void test_begin_simple_const( void ){

    testChannel.clearTx();
    shell.enableFormatting = false;
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "Shellminator Unit Test\r\narnold:$ ", testChannel.txBuffer );

}

void test_begin_simple( void ){

    char nameBuff[] = "arnold";

    testChannel.clearTx();
    shell.enableFormatting = false;
    shell.begin( nameBuff );

    TEST_ASSERT_EQUAL_STRING ( "Shellminator Unit Test\r\narnold:$ ", testChannel.txBuffer );

}
int main(){

    UNITY_BEGIN();
    RUN_TEST( test_begin_simple );
    RUN_TEST( test_begin_simple_const );
    return UNITY_END();

}