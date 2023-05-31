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

*/

#include <stdio.h>

// Shellminator library
#include "Shellminator.hpp"
#include "Shellminator-IO.hpp"

// Stream class for testing
#include "../testStream.hpp"

// Unit test module.
#include "unity.h"

// To test command highlight.
#include "Commander-API.hpp"
#include "Commander-IO.hpp"

// For timing.
#include "System.h"

// Use stdio as Channel.
testStream testChannel;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &testChannel );

// We have to create an object from Commander class.
Commander commander;

// We have to create the prototype functions for our commands.
// The arguments has to be the same for all command functions.
void test_func( char *args, Stream *response, void* parent ){

}

// Commander API-tree
Commander::API_t API_tree[] = {
    apiElement( "test", "Simple function to call.", test_func )
};

// Put any init code here that needs to run before testing.
void setUp(void) {

  // At start, Commander does not know anything about our commands.
  // We have to attach the API_tree array from the previous steps
  // to Commander to work properly.
  commander.attachTree( API_tree );

  // Initialize Commander.
  commander.init();
  
}

// Put any init code here that needs to run after testing.
void tearDown(void) {

}

void test_update_help_command(void) {

    // Known string.
    const char* help_command_data = "help\r";

    char expected[ 10000 ];
    char* expectedPointer = expected;

    strcpy( expected, "help\r\n" );
    expectedPointer += strlen( "help\r\n" );

    strcpy( expectedPointer, Shellminator::helpText );
    expectedPointer += strlen( Shellminator::helpText );

    strcpy( expectedPointer, "\x1B[1;32m\x1B[1;37m:\x1B[1;34m$\x1B[0;37m " );


    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Clear the Rx buffer before the test.
    testChannel.clearRx();

    // Push the known data to the Tx buffer.
    testChannel.pushRx( (uint8_t*)help_command_data, strlen( (const char*)help_command_data ) );

    // Send backspace command.
    shell.update();


    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

}

void test_update_syntax_highlight(void) {

    // Known string.
    const char* help_command_data = "test";

    unsigned long timerStart = millis();

    char expected[] = "\r\x1B[1;32m\x1B[1;37m:\x1B[1;34m$\x1B[0;37m \x1B[0K\x1B[1;32mtest\x1B[0K\x1B[0;37m";

    // Attach commander to the shell interface.
    shell.attachCommander( &commander );

    // Clear the Tx buffer before the test.
    testChannel.clearTx();

    // Clear the Rx buffer before the test.
    testChannel.clearRx();

    // Push the known data to the Tx buffer.
    testChannel.pushRx( (uint8_t*)help_command_data, strlen( (const char*)help_command_data ) );

    // Send backspace command.
    shell.update();

    // Clear Tx immediately.
    testChannel.clearTx();

    while( ( millis() - timerStart ) < 500 ){
        // Wait for 500ms
    }

    // Send backspace command.
    shell.update();

    // Check the buffer content. It should match with the known data string.
    TEST_ASSERT_EQUAL_STRING ( (const char*)expected, testChannel.txBuffer );

}

int main(){

    UNITY_BEGIN();
    RUN_TEST( test_update_help_command );
    RUN_TEST( test_update_syntax_highlight );
    return UNITY_END();

}