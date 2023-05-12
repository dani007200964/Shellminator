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

// Put any init code here that needs to run before testing.
void setUp(void) {

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

// In this test we will push a known data to the Tx buffer and check it.
// In this case, the pushing should not cause buffer overflow.
void test_channel_pushTx(void) {

    // Known string.
    const char* testDataTx = "testDataTx";

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Push the known data to the Tx buffer.
    testChannel.pushTx( (uint8_t*)testDataTx, strlen( (const char*)testDataTx ) );

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)testDataTx, testChannel.txBuffer );

}


void test_channel_pushRx(void) {

    const char* testDataRx = "testDataRx";

    testChannel.clearRx();
    testChannel.pushRx( (uint8_t*)testDataRx, strlen( (const char*)testDataRx ) );

    TEST_ASSERT_EQUAL_STRING ( (const char*)testDataRx, testChannel.rxBuffer );

}

void test_channel_clearTx(void) {

    testChannel.clearTx();
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_channel_clearRx(void) {

    testChannel.clearRx();
    TEST_ASSERT_EQUAL_STRING ( "", testChannel.rxBuffer );

}

void test_channel_pushTx_overflow(void) {

    uint32_t i;
    uint8_t lastNumber;
    testChannel.clearTx();

    for( i = 0; i <= TEST_STREAM_BUFFER_SIZE; i++ ){
        lastNumber = i;
        testChannel.pushTx( i );
    }

    TEST_ASSERT_EQUAL_UINT8( testChannel.txBuffer[ 0 ], lastNumber );
    TEST_ASSERT_EQUAL_UINT8( testChannel.txBuffer[ 1 ], 1 );

}

void test_channel_pushRx_overflow(void) {

    uint32_t i;
    uint8_t lastNumber;
    testChannel.clearRx();

    for( i = 0; i <= TEST_STREAM_BUFFER_SIZE; i++ ){
        lastNumber = i;
        testChannel.pushRx( i );
    }

    TEST_ASSERT_EQUAL_UINT8( testChannel.rxBuffer[ 0 ], lastNumber );
    TEST_ASSERT_EQUAL_UINT8( testChannel.rxBuffer[ 1 ], 1 );

}

int main(){

    UNITY_BEGIN();
    RUN_TEST( test_channel_pushTx );
    RUN_TEST( test_channel_pushRx );
    RUN_TEST( test_channel_clearTx );
    RUN_TEST( test_channel_clearRx );
    RUN_TEST( test_channel_pushTx_overflow );
    RUN_TEST( test_channel_pushRx_overflow );
    return UNITY_END();

}