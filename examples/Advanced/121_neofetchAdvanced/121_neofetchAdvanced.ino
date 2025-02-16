
/*
 * Created on June 8 2024
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
#include "Shellminator-Neofetch.hpp"


// Header
const char* SHELLMINATOR_WEAK neofetch_header_text = "Arnold@T800.local";
int         SHELLMINATOR_WEAK neofetch_header_text_color = Shellminator::GREEN;
int         SHELLMINATOR_WEAK neofetch_header_text_style = Shellminator::BOLD;

// Header Underline
char        SHELLMINATOR_WEAK neofetch_header_line_char = '=';
int         SHELLMINATOR_WEAK neofetch_header_line_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_header_line_style = Shellminator::BOLD;

// Field Format
int         SHELLMINATOR_WEAK neofetch_field_text_color = Shellminator::YELLOW;
int         SHELLMINATOR_WEAK neofetch_field_text_style = Shellminator::BOLD;

// Version
const char* SHELLMINATOR_WEAK neofetch_fw_text = SHELLMINATOR_VERSION;
int         SHELLMINATOR_WEAK neofetch_fw_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_fw_text_style = Shellminator::BOLD;

// CPU
const char* SHELLMINATOR_WEAK neofetch_cpu_text = "AVR";
int         SHELLMINATOR_WEAK neofetch_cpu_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_cpu_text_style = Shellminator::BOLD;

// Compiler
const char* SHELLMINATOR_WEAK neofetch_compiler_text = "GCC " __VERSION__;
int         SHELLMINATOR_WEAK neofetch_compiler_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_compiler_text_style = Shellminator::BOLD;

// Compile Date
const char* SHELLMINATOR_WEAK neofetch_compile_date_text = __DATE__;
int         SHELLMINATOR_WEAK neofetch_compile_date_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_compile_date_text_style = Shellminator::BOLD;

// Author
const char* SHELLMINATOR_WEAK neofetch_author_text = "Daniel Hajnal";
int         SHELLMINATOR_WEAK neofetch_author_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_author_text_style = Shellminator::BOLD;

// User Field 1
const char* SHELLMINATOR_WEAK neofetch_user_field_1_label = "License: ";
const char* SHELLMINATOR_WEAK neofetch_user_field_1_text = "MIT";
int         SHELLMINATOR_WEAK neofetch_user_field_1_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_1_text_style = Shellminator::BOLD;

// User Field 2
const char* SHELLMINATOR_WEAK neofetch_user_field_2_label = "Manufacturer: ";
const char* SHELLMINATOR_WEAK neofetch_user_field_2_text = "Cyberdyne";
int         SHELLMINATOR_WEAK neofetch_user_field_2_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_2_text_style = Shellminator::BOLD;

// User Field 3
const char* SHELLMINATOR_WEAK neofetch_user_field_3_label = "Parser: ";
const char* SHELLMINATOR_WEAK neofetch_user_field_3_text = "Commander-API";
int         SHELLMINATOR_WEAK neofetch_user_field_3_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_3_text_style = Shellminator::BOLD;

// User Field 4
const char* SHELLMINATOR_WEAK neofetch_user_field_4_label = "System Memory: ";
const char* SHELLMINATOR_WEAK neofetch_user_field_4_text = "2kByte";
int         SHELLMINATOR_WEAK neofetch_user_field_4_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_4_text_style = Shellminator::BOLD;

// User Field 5
const char* SHELLMINATOR_WEAK neofetch_user_field_5_label = "OS: ";
const char* SHELLMINATOR_WEAK neofetch_user_field_5_text = "mulatOS";
int         SHELLMINATOR_WEAK neofetch_user_field_5_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_5_text_style = Shellminator::BOLD;

// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );




// System init section.
void setup(){

    Serial.begin(115200);

    // Clear the terminal
    shell.clear();

    // Attach the default neofetch callback.
    shell.attachNeofetchFunc( defaultShellminatorNeofetch );

    // Initialize shell object.
    shell.begin( "arnold" );


}

// Infinite loop.
void loop(){

    // Process the new data.
    shell.update();


}
