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

// This function pointer tracks the last override function call's address.
void( *lastOverrideFuncPtr )( void ) = NULL;

// Put any init code here that needs to run before testing.
void setUp(void) {

}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void test_up_down_arrow(void) {

    const char* testData1 = "first\r";
    const char* testData2 = "second\r";
    const char* testData3 = "third\r";

    const char* expected = "\rarnold:$ \x1B[0Kthird\x1B[0K\rarnold:$ \x1B[0Ksecond\x1B[0K\rarnold:$ \x1B[0Kfirst\x1B[0K\rarnold:$ \x1B[0Ksecond\x1B[0K\rarnold:$ \x1B[0Kthird\x1B[0K\rarnold:$ \x1B[0K\x1B[0K";

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.pushRx( (uint8_t*)testData1, strlen( testData1 ) );
    shell.update();

    testChannel.pushRx( (uint8_t*)testData2, strlen( testData2 ) );
    shell.update();

    testChannel.pushRx( (uint8_t*)testData3, strlen( testData3 ) );
    shell.update();

    testChannel.clearTx();

    // Go up to the second command.
    testChannel.emulateUpArrowKey();
    testChannel.emulateUpArrowKey();
    testChannel.emulateUpArrowKey();
    testChannel.emulateUpArrowKey();
    shell.update();

    // Go down to the new command.
    testChannel.emulateDownArrowKey();
    testChannel.emulateDownArrowKey();
    testChannel.emulateDownArrowKey();
    testChannel.emulateDownArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

}

void test_left_right_arrow(void) {

    const char* testData = "eg";

    const char* expected = "\rarnold:$ \x1B[0Klego\x1B[0K";

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.clearRx();
    testChannel.pushRx( (uint8_t*)testData, strlen( testData ) );

    testChannel.emulateLeftArrowKey();
    testChannel.emulateLeftArrowKey();
    testChannel.emulateLeftArrowKey();

    testChannel.pushRx( (uint8_t)'l' );

    testChannel.emulateRightArrowKey();
    testChannel.emulateRightArrowKey();
    testChannel.emulateRightArrowKey();
    testChannel.emulateRightArrowKey();

    testChannel.pushRx( (uint8_t)'o' );
    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    testChannel.clearTx();

    testChannel.emulateUpArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

}

void test_home_key(void) {

    const char* testData = "hello bello";
    const char* expected = "\rarnold:$ \x1B[0Khello bello\x1B[0K\x1B[11D";

    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.clearRx();
    testChannel.pushRx( (uint8_t*)testData, strlen( testData ) );

    shell.update();

    testChannel.clearTx();

    testChannel.emulateHomeKey( 'H' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

    testChannel.emulateRightArrowKey();
    testChannel.emulateRightArrowKey();
    shell.update();

    testChannel.clearTx();

    testChannel.emulateHomeKey( '1' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

    testChannel.emulateRightArrowKey();
    testChannel.emulateRightArrowKey();
    shell.update();

    testChannel.clearTx();

    testChannel.emulateHomeKey( '-' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_end_key(void) {

    const char* testData = "hello bello";
    const char* expected = "\rarnold:$ \x1B[0Khello bello\x1B[0K";

    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.clearRx();
    testChannel.pushRx( (uint8_t*)testData, strlen( testData ) );

    shell.update();

    testChannel.clearTx();

    testChannel.emulateEndKey( 'F' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

    testChannel.emulateLeftArrowKey();
    testChannel.emulateLeftArrowKey();
    shell.update();

    testChannel.clearTx();

    testChannel.emulateEndKey( '4' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

    testChannel.emulateLeftArrowKey();
    testChannel.emulateLeftArrowKey();
    shell.update();

    testChannel.clearTx();

    testChannel.emulateEndKey( '-' );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_del_key(void) {

    const char* testData = "hello bello";
    const char* expected = "\rarnold:$ \x1B[0Khello bel\x1B[0K";

    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.clearRx();
    testChannel.pushRx( (uint8_t*)testData, strlen( testData ) );

    shell.update();

    testChannel.clearTx();

    testChannel.emulateDelKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

    testChannel.emulateLeftArrowKey();
    testChannel.emulateLeftArrowKey();
    testChannel.emulateDelKey();

    shell.update();

    testChannel.clearTx();
    testChannel.emulateDelKey();
    testChannel.emulateDelKey();

    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected, testChannel.txBuffer );

    // Invalid state
    testChannel.clearTx();
    testChannel.emulateDelKey( true );

    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void test_pgUp_pgDown_key(void) {

    const char* testData1 = "hello\r";
    const char* testData2 = "bello\r";

    const char* expected1 = "\rarnold:$ \x1B[0Khello\x1B[0K\x1B[5D";
    const char* expected2 = "\rarnold:$ \x1B[0Kbello\x1B[0K\x1B[5D";

    const char* expected11 = "\rarnold:$ \x1B[0Khello\x1B[0K\x1B[4D";
    const char* expected22 = "\rarnold:$ \x1B[0Kbello\x1B[0K\x1B[4D";

    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    testChannel.clearRx();

    testChannel.pushRx( (uint8_t*)testData1, strlen( testData1 ) );
    shell.update();

    testChannel.pushRx( (uint8_t*)testData2, strlen( testData2 ) );
    shell.update();

    testChannel.clearTx();

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected2, testChannel.txBuffer );

    testChannel.clearTx();

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected1, testChannel.txBuffer );

    testChannel.clearTx();

    testChannel.emulatePgDownKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected2, testChannel.txBuffer );

    // Search module
    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    testChannel.pushRx( (uint8_t)'h' );
    shell.update();
    testChannel.clearTx();

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected11, testChannel.txBuffer );

    testChannel.pushRx( (uint8_t)'\r' );
    shell.update();

    testChannel.pushRx( (uint8_t)'b' );
    shell.update();
    testChannel.clearTx();

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( expected22, testChannel.txBuffer );

    // Invalid state
    testChannel.clearTx();
    testChannel.emulatePgUpKey( true );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

    testChannel.clearTx();
    testChannel.emulatePgDownKey( true );
    shell.update();

    TEST_ASSERT_EQUAL_STRING ( "", testChannel.txBuffer );

}

void upKeyOverride(){
    lastOverrideFuncPtr = &upKeyOverride;
}

void downKeyOverride(){
    lastOverrideFuncPtr = &downKeyOverride;
}

void leftKeyOverride(){
    lastOverrideFuncPtr = &leftKeyOverride;
}

void rightKeyOverride(){
    lastOverrideFuncPtr = &rightKeyOverride;
}

void homeKeyOverride(){
    lastOverrideFuncPtr = &homeKeyOverride;
}

void endKeyOverride(){
    lastOverrideFuncPtr = &endKeyOverride;
}

void abortKeyOverride(){
    lastOverrideFuncPtr = &abortKeyOverride;
}

void pageUpKeyOverride(){
    lastOverrideFuncPtr = &pageUpKeyOverride;
}

void pageDownKeyOverride(){
    lastOverrideFuncPtr = &pageDownKeyOverride;
}

void logoutKeyOverride(){
    lastOverrideFuncPtr = &logoutKeyOverride;
}

void searchKeyOverride(){
    lastOverrideFuncPtr = &searchKeyOverride;
}

void test_up_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideUpArrow( upKeyOverride );

    testChannel.emulateUpArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( upKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeUpArrow();

    testChannel.emulateUpArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_down_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideDownArrow( downKeyOverride );

    testChannel.emulateDownArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( downKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeDownArrow();

    testChannel.emulateDownArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_left_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideLeftArrow( leftKeyOverride );

    testChannel.emulateLeftArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( leftKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeLeftArrow();

    testChannel.emulateLeftArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_right_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideRightArrow( rightKeyOverride );

    testChannel.emulateRightArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( rightKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeRightArrow();

    testChannel.emulateRightArrowKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_home_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideHomeKey( homeKeyOverride );

    testChannel.emulateHomeKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( homeKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeHomeKey();

    testChannel.emulateHomeKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_end_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideEndKey( endKeyOverride );

    testChannel.emulateEndKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( endKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeEndKey();

    testChannel.emulateEndKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_abort_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideAbortKey( abortKeyOverride );

    testChannel.emulateAbortKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( abortKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeAbortKey();

    testChannel.emulateAbortKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_page_up_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overridePageUpKey( pageUpKeyOverride );

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( pageUpKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freePageUpKey();

    testChannel.emulatePgUpKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_page_down_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overridePageDownKey( pageDownKeyOverride );

    testChannel.emulatePgDownKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( pageDownKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freePageDownKey();

    testChannel.emulatePgDownKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_logout_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideLogoutKey( logoutKeyOverride );

    testChannel.emulateLogoutKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( logoutKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeLogoutKey();

    testChannel.emulateLogoutKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

void test_search_key_override(void) {

    testChannel.clearRx();

    shell.enableFormatting = false;
    shell.begin( "arnold" );

    lastOverrideFuncPtr = NULL;
    shell.overrideSearchKey( searchKeyOverride );

    testChannel.emulateSearchKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( searchKeyOverride, lastOverrideFuncPtr );

    lastOverrideFuncPtr = NULL;
    shell.freeSearchKey();

    testChannel.emulateSearchKey();
    shell.update();

    TEST_ASSERT_EQUAL_PTR ( NULL, lastOverrideFuncPtr );

}

int main(){

    UNITY_BEGIN();

    RUN_TEST( test_up_down_arrow );
    RUN_TEST( test_left_right_arrow );
    RUN_TEST( test_home_key );
    RUN_TEST( test_end_key );
    RUN_TEST( test_del_key );
    RUN_TEST( test_pgUp_pgDown_key );

    // Key overrides
    RUN_TEST( test_up_key_override );
    RUN_TEST( test_down_key_override );
    RUN_TEST( test_left_key_override );
    RUN_TEST( test_right_key_override );
    RUN_TEST( test_home_key_override );
    RUN_TEST( test_end_key_override );
    RUN_TEST( test_abort_key_override );
    RUN_TEST( test_page_up_key_override );
    RUN_TEST( test_page_down_key_override );
    RUN_TEST( test_logout_key_override );
    RUN_TEST( test_search_key_override );

    // Todo: regular key behaviours: logout, clear screen, reverse search, end of line, beginning of line, auto complete

    return UNITY_END();

}