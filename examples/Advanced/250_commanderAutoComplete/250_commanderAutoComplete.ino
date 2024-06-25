
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
 *
 * To test this example, you need a terminal emulator like PuTTY or Minicom.
 * This example shows a simple setup for Shellminator. It will create an
 * interactive interface, but it does not execute any command.
 * See Shellminator_execute example for further information.
*/


#include "Shellminator.hpp"
#include "Shellminator-Commander-Interface.hpp"
#include "Commander-API.hpp"


// We have to create an object from Commander class.
Commander commander;

// Generated with Commander Trie Generator
// Trie name: cat
uint16_t cat_element_0_indexes[] = { 1 };
uint16_t cat_element_1_indexes[] = { 2 };
uint16_t cat_element_2_indexes[] = { 3, 19, 44 };
uint16_t cat_element_3_indexes[] = { 4 };
uint16_t cat_element_4_indexes[] = { 5 };
uint16_t cat_element_5_indexes[] = { 6 };
uint16_t cat_element_6_indexes[] = { 7 };
uint16_t cat_element_7_indexes[] = { 8 };
uint16_t cat_element_8_indexes[] = { 9 };
uint16_t cat_element_9_indexes[] = { 10 };
uint16_t cat_element_10_indexes[] = { 11 };
uint16_t cat_element_11_indexes[] = { 12 };
uint16_t cat_element_12_indexes[] = { 13 };
uint16_t cat_element_13_indexes[] = { 14 };
uint16_t cat_element_14_indexes[] = { 15 };
uint16_t cat_element_15_indexes[] = { 16 };
uint16_t cat_element_16_indexes[] = { 17 };
uint16_t cat_element_17_indexes[] = { 18 };
uint16_t cat_element_19_indexes[] = { 20, 39 };
uint16_t cat_element_20_indexes[] = { 21 };
uint16_t cat_element_21_indexes[] = { 22 };
uint16_t cat_element_22_indexes[] = { 23 };
uint16_t cat_element_23_indexes[] = { 24 };
uint16_t cat_element_24_indexes[] = { 25 };
uint16_t cat_element_25_indexes[] = { 26 };
uint16_t cat_element_26_indexes[] = { 27 };
uint16_t cat_element_27_indexes[] = { 28 };
uint16_t cat_element_28_indexes[] = { 29 };
uint16_t cat_element_29_indexes[] = { 30 };
uint16_t cat_element_30_indexes[] = { 31 };
uint16_t cat_element_31_indexes[] = { 32 };
uint16_t cat_element_32_indexes[] = { 33 };
uint16_t cat_element_33_indexes[] = { 34 };
uint16_t cat_element_34_indexes[] = { 35 };
uint16_t cat_element_35_indexes[] = { 36 };
uint16_t cat_element_36_indexes[] = { 37 };
uint16_t cat_element_37_indexes[] = { 38 };
uint16_t cat_element_39_indexes[] = { 40 };
uint16_t cat_element_40_indexes[] = { 41 };
uint16_t cat_element_41_indexes[] = { 42 };
uint16_t cat_element_42_indexes[] = { 43 };
uint16_t cat_element_44_indexes[] = { 45 };
uint16_t cat_element_45_indexes[] = { 46 };

AutoComplete::TrieElement_t cat_trie[] = {
	{ '\0', false, 1, cat_element_0_indexes },
	{ ' ', false, 1, cat_element_1_indexes },
	{ '-', false, 3, cat_element_2_indexes },
	{ 'n', false, 1, cat_element_3_indexes },
	{ ' ', false, 1, cat_element_4_indexes },
	{ '"', false, 1, cat_element_5_indexes },
	{ 'J', false, 1, cat_element_6_indexes },
	{ 'a', false, 1, cat_element_7_indexes },
	{ 'c', false, 1, cat_element_8_indexes },
	{ 'k', false, 1, cat_element_9_indexes },
	{ ' ', false, 1, cat_element_10_indexes },
	{ 'T', false, 1, cat_element_11_indexes },
	{ 'h', false, 1, cat_element_12_indexes },
	{ 'e', false, 1, cat_element_13_indexes },
	{ ' ', false, 1, cat_element_14_indexes },
	{ 'C', false, 1, cat_element_15_indexes },
	{ 'a', false, 1, cat_element_16_indexes },
	{ 't', false, 1, cat_element_17_indexes },
	{ '"', true, 0, NULL },
	{ '-', false, 2, cat_element_19_indexes },
	{ 'n', false, 1, cat_element_20_indexes },
	{ 'a', false, 1, cat_element_21_indexes },
	{ 'm', false, 1, cat_element_22_indexes },
	{ 'e', false, 1, cat_element_23_indexes },
	{ ' ', false, 1, cat_element_24_indexes },
	{ '"', false, 1, cat_element_25_indexes },
	{ 'J', false, 1, cat_element_26_indexes },
	{ 'a', false, 1, cat_element_27_indexes },
	{ 'c', false, 1, cat_element_28_indexes },
	{ 'k', false, 1, cat_element_29_indexes },
	{ ' ', false, 1, cat_element_30_indexes },
	{ 'T', false, 1, cat_element_31_indexes },
	{ 'h', false, 1, cat_element_32_indexes },
	{ 'e', false, 1, cat_element_33_indexes },
	{ ' ', false, 1, cat_element_34_indexes },
	{ 'C', false, 1, cat_element_35_indexes },
	{ 'a', false, 1, cat_element_36_indexes },
	{ 't', false, 1, cat_element_37_indexes },
	{ '"', true, 0, NULL },
	{ 'a', false, 1, cat_element_39_indexes },
	{ 'g', false, 1, cat_element_40_indexes },
	{ 'e', false, 1, cat_element_41_indexes },
	{ ' ', false, 1, cat_element_42_indexes },
	{ '2', true, 0, NULL },
	{ 'a', false, 1, cat_element_44_indexes },
	{ ' ', false, 1, cat_element_45_indexes },
	{ '2', true, 0, NULL }
};

// Generated with Commander Trie Generator
// Trie name: dog
uint16_t dog_element_0_indexes[] = { 1 };
uint16_t dog_element_1_indexes[] = { 2 };
uint16_t dog_element_2_indexes[] = { 3, 19, 44 };
uint16_t dog_element_3_indexes[] = { 4 };
uint16_t dog_element_4_indexes[] = { 5 };
uint16_t dog_element_5_indexes[] = { 6 };
uint16_t dog_element_6_indexes[] = { 7 };
uint16_t dog_element_7_indexes[] = { 8 };
uint16_t dog_element_8_indexes[] = { 9 };
uint16_t dog_element_9_indexes[] = { 10 };
uint16_t dog_element_10_indexes[] = { 11 };
uint16_t dog_element_11_indexes[] = { 12 };
uint16_t dog_element_12_indexes[] = { 13 };
uint16_t dog_element_13_indexes[] = { 14 };
uint16_t dog_element_14_indexes[] = { 15 };
uint16_t dog_element_15_indexes[] = { 16 };
uint16_t dog_element_16_indexes[] = { 17 };
uint16_t dog_element_17_indexes[] = { 18 };
uint16_t dog_element_19_indexes[] = { 20, 39 };
uint16_t dog_element_20_indexes[] = { 21 };
uint16_t dog_element_21_indexes[] = { 22 };
uint16_t dog_element_22_indexes[] = { 23 };
uint16_t dog_element_23_indexes[] = { 24 };
uint16_t dog_element_24_indexes[] = { 25 };
uint16_t dog_element_25_indexes[] = { 26 };
uint16_t dog_element_26_indexes[] = { 27 };
uint16_t dog_element_27_indexes[] = { 28 };
uint16_t dog_element_28_indexes[] = { 29 };
uint16_t dog_element_29_indexes[] = { 30 };
uint16_t dog_element_30_indexes[] = { 31 };
uint16_t dog_element_31_indexes[] = { 32 };
uint16_t dog_element_32_indexes[] = { 33 };
uint16_t dog_element_33_indexes[] = { 34 };
uint16_t dog_element_34_indexes[] = { 35 };
uint16_t dog_element_35_indexes[] = { 36 };
uint16_t dog_element_36_indexes[] = { 37 };
uint16_t dog_element_37_indexes[] = { 38 };
uint16_t dog_element_39_indexes[] = { 40 };
uint16_t dog_element_40_indexes[] = { 41 };
uint16_t dog_element_41_indexes[] = { 42 };
uint16_t dog_element_42_indexes[] = { 43 };
uint16_t dog_element_44_indexes[] = { 45 };
uint16_t dog_element_45_indexes[] = { 46 };

AutoComplete::TrieElement_t dog_trie[] = {
	{ '\0', false, 1, dog_element_0_indexes },
	{ ' ', false, 1, dog_element_1_indexes },
	{ '-', false, 3, dog_element_2_indexes },
	{ 'n', false, 1, dog_element_3_indexes },
	{ ' ', false, 1, dog_element_4_indexes },
	{ '"', false, 1, dog_element_5_indexes },
	{ 'B', false, 1, dog_element_6_indexes },
	{ 'e', false, 1, dog_element_7_indexes },
	{ 'a', false, 1, dog_element_8_indexes },
	{ 'r', false, 1, dog_element_9_indexes },
	{ ' ', false, 1, dog_element_10_indexes },
	{ 'T', false, 1, dog_element_11_indexes },
	{ 'h', false, 1, dog_element_12_indexes },
	{ 'e', false, 1, dog_element_13_indexes },
	{ ' ', false, 1, dog_element_14_indexes },
	{ 'D', false, 1, dog_element_15_indexes },
	{ 'o', false, 1, dog_element_16_indexes },
	{ 'g', false, 1, dog_element_17_indexes },
	{ '"', true, 0, NULL },
	{ '-', false, 2, dog_element_19_indexes },
	{ 'n', false, 1, dog_element_20_indexes },
	{ 'a', false, 1, dog_element_21_indexes },
	{ 'm', false, 1, dog_element_22_indexes },
	{ 'e', false, 1, dog_element_23_indexes },
	{ ' ', false, 1, dog_element_24_indexes },
	{ '"', false, 1, dog_element_25_indexes },
	{ 'B', false, 1, dog_element_26_indexes },
	{ 'e', false, 1, dog_element_27_indexes },
	{ 'a', false, 1, dog_element_28_indexes },
	{ 'r', false, 1, dog_element_29_indexes },
	{ ' ', false, 1, dog_element_30_indexes },
	{ 'T', false, 1, dog_element_31_indexes },
	{ 'h', false, 1, dog_element_32_indexes },
	{ 'e', false, 1, dog_element_33_indexes },
	{ ' ', false, 1, dog_element_34_indexes },
	{ 'D', false, 1, dog_element_35_indexes },
	{ 'o', false, 1, dog_element_36_indexes },
	{ 'g', false, 1, dog_element_37_indexes },
	{ '"', true, 0, NULL },
	{ 'a', false, 1, dog_element_39_indexes },
	{ 'g', false, 1, dog_element_40_indexes },
	{ 'e', false, 1, dog_element_41_indexes },
	{ ' ', false, 1, dog_element_42_indexes },
	{ '2', true, 0, NULL },
	{ 'a', false, 1, dog_element_44_indexes },
	{ ' ', false, 1, dog_element_45_indexes },
	{ '2', true, 0, NULL }
};

AutoComplete cat( cat_trie );
AutoComplete dog( dog_trie );

char autoCompleteBuffer[ 100 ];
int autoCompleteBufferSize = sizeof( autoCompleteBuffer );

bool cat_func( char *args, CommandCaller* caller );
bool dog_func( char *args, CommandCaller* caller );

Commander::systemCommand_t API_tree[] = {
    systemCommandWithHelp( "cat", "Description for cat command.", cat_func, &cat ),
    systemCommandWithHelp( "dog", "Description for dog command.", dog_func, &dog )
};

// Create a ShellminatorCommanderInterface object, and initialize it to use Serial
ShellminatorCommanderInterface shell( &Serial );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    commander.attachDebugChannel( &Serial );
    commander.attachTree( API_tree );
    commander.init();

    shell.attachCommander( &commander );
    shell.attachAutoCompleteBuffer( autoCompleteBuffer, autoCompleteBufferSize );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}

/// This is an example function for the cat command
bool cat_func(char *args, CommandCaller* caller ){

  caller -> print("Hello from cat function!\r\n");
  return true;

}

/// This is an example function for the dog command
bool dog_func(char *args, CommandCaller* caller ){

  caller -> print("Hello from dog function!\r\n");
  return true;

}

/// This is an example function for the sum command
bool sum_func(char *args, CommandCaller* caller ){

  // These variables will hold the value of the
  // two numbers, that has to be summed.
  int a = 0;
  int b = 0;

  // This variable will hold the result of the
  // argument parser.
  int argResult;

  // This variable will hold the sum result.
  int sum = 0;

  argResult = sscanf( args, "%d %d", &a, &b );

  // We have to check that we parsed successfully the two
  // numbers from the argument string.
  if( argResult != 2 ){

    // If we could not parse two numbers, we have an argument problem.
    // We print out the problem to the response channel.
    caller -> print( "Argument error! Two numbers required, separated with a blank space.\r\n" );

    // Sadly we have to stop the command execution and return.
    return false;

  }

  // Calculate the sum.
  sum = a + b;

  // Print out the result.
  caller -> print( a );
  caller -> print( " + " );
  caller -> print( b );
  caller -> print( " = " );
  caller -> println( sum );
    return true;
}