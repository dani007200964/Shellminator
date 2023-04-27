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
This is a basic simulator for Shellminator. You can modify this code as you like, to test
your code. It is faster and easier, than working with an embedded device first.
Please note that, some functions are not working here.
*/

#include <stdio.h>

#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

#include "testStream.hpp"

#include "unity.h"

// Use stdio as Channel.
testStream testChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &testChannel );

const char logo[] = "Shellminator Unit Test\r\n";

void setUp(void) {

  // Attach the logo.
  shell.attachLogo( logo );

  // Begin the terminal.
  //shell.begin( "arnold" );
}

void tearDown(void) {
    // clean stuff up here
}

void test_channel_pushTx(void) {

    const char* testDataTx = "testDataTx";

    testChannel.clearTx();
    testChannel.pushTx( (uint8_t*)testDataTx, strlen( (const char*)testDataTx ) );

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

void test_begin_simple( void ){

    testChannel.clearTx();
    shell.enableFormatting = false;
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "Shellminator Unit Test\r\narnold:$ ", testChannel.txBuffer );

}

void test_begin_formatted( void ){

    testChannel.clearTx();
    shell.enableFormatting = true;
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "\x1B[1;31m\x1B[1;31mShellminator Unit Test\r\n\x1B[0;37m\x1B[1;32marnold\x1B[1;37m:\x1B[1;34m$\x1B[0;37m ", testChannel.txBuffer );

}


void test_begin_path_simple( void ){

    testChannel.clearTx();
    shell.enableFormatting = false;
    shell.setBannerPathText( "path" );
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "Shellminator Unit Test\r\narnold:path ", testChannel.txBuffer );

}

void test_begin_path_formatted( void ){

    testChannel.clearTx();
    shell.enableFormatting = true;
    shell.setBannerPathText( "path" );
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "\x1B[1;31m\x1B[1;31mShellminator Unit Test\r\n\x1B[0;37m\x1B[1;32marnold\x1B[1;37m:\x1B[1;34mpath\x1B[0;37m ", testChannel.txBuffer );

}

void test_begin_path_overload( void ){

    testChannel.clearTx();
    shell.enableFormatting = false;
    shell.setBannerPathText( "123456789abcdefghijk" );
    shell.begin( "arnold" );

    TEST_ASSERT_EQUAL_STRING ( "Shellminator Unit Test\r\narnold:123456789 ", testChannel.txBuffer );
    shell.setBannerPathText( "$" );

}

void test_sendBackspace(){

    testChannel.clearTx();
    shell.sendBackspace();

    TEST_ASSERT_EQUAL_STRING ( "\b \b", testChannel.txBuffer );

}

void test_clear(){

    testChannel.clearTx();
    shell.clear();

    TEST_ASSERT_EQUAL_STRING( "\x1B[H\x1B[J", testChannel.txBuffer );

}

void execFuncTest( char* args ){

    TEST_ASSERT_EQUAL_STRING( "This is a command\r\n", testChannel.txBuffer );
    TEST_ASSERT_EQUAL_STRING( "This is a command", args );

}

void test_executionFunction(){

    const char* testCommand = "This is a command\r";

    testChannel.clearTx();
    testChannel.clearRx();

    shell.addExecFunc( execFuncTest );

    testChannel.pushRx( (uint8_t*)testCommand, strlen( testCommand ) );
    shell.update();

}

void execFunc_up_down_arrow( char* args ){


}

//--- Testing history functions ---//
void test_up_down_arrow(){

    const char* testCommand_1 = "command 1\r";
    const char* testCommand_2 = "command 2\r";
    const char* testCommand_3 = "command 3\r";

    const char* upArrowCommand = "\033[A";
    const char* downArrowCommand = "\033[B";

    shell.addExecFunc( execFunc_up_down_arrow );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)testCommand_1, strlen( testCommand_1 ) );
    shell.update();

    testChannel.pushRx( (uint8_t*)testCommand_2, strlen( testCommand_2 ) );
    shell.update();

    testChannel.pushRx( (uint8_t*)testCommand_3, strlen( testCommand_3 ) );
    shell.update();


    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)upArrowCommand, strlen( upArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0Kcommand 3\x1B[0K", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)upArrowCommand, strlen( upArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0Kcommand 2\x1B[0K", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)upArrowCommand, strlen( upArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0Kcommand 1\x1B[0K", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)downArrowCommand, strlen( downArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0Kcommand 2\x1B[0K", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)downArrowCommand, strlen( downArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0Kcommand 3\x1B[0K", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)downArrowCommand, strlen( downArrowCommand ) );
    shell.update();

    TEST_ASSERT_EQUAL_STRING( "\rarnold:$ \x1B[0K\x1B[0K", testChannel.txBuffer );
}


int main(){

    UNITY_BEGIN();
    RUN_TEST( test_channel_pushTx );
    RUN_TEST( test_channel_pushRx );
    RUN_TEST( test_channel_clearTx );
    RUN_TEST( test_channel_clearRx );
    RUN_TEST( test_channel_pushTx_overflow );
    RUN_TEST( test_channel_pushRx_overflow );
    RUN_TEST( test_begin_simple );
    RUN_TEST( test_begin_formatted );
    RUN_TEST( test_begin_path_simple );
    RUN_TEST( test_begin_path_formatted );
    RUN_TEST( test_begin_path_overload );
    RUN_TEST( test_sendBackspace );
    RUN_TEST( test_clear );
    RUN_TEST( test_executionFunction );
    RUN_TEST( test_up_down_arrow );

    return UNITY_END();

}