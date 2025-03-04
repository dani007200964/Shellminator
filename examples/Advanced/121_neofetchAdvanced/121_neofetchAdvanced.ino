
/*
 * Created on June 8 2024
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"
#include "Shellminator-Neofetch.hpp"


// Header
const char* neofetch_header_text = "Arnold@T800.local";
int         neofetch_header_text_color = Shellminator::GREEN;
int         neofetch_header_text_style = Shellminator::BOLD;

// Header Underline
char        neofetch_header_line_char = '=';
int         neofetch_header_line_color = Shellminator::WHITE;
int         neofetch_header_line_style = Shellminator::BOLD;

// Field Format
int         neofetch_field_text_color = Shellminator::YELLOW;
int         neofetch_field_text_style = Shellminator::BOLD;

// Version
const char* neofetch_fw_text = SHELLMINATOR_VERSION;
int         neofetch_fw_text_color = Shellminator::WHITE;
int         neofetch_fw_text_style = Shellminator::BOLD;

// CPU
const char* neofetch_cpu_text = "AVR";
int         neofetch_cpu_text_color = Shellminator::WHITE;
int         neofetch_cpu_text_style = Shellminator::BOLD;

// Compiler
const char* neofetch_compiler_text = "GCC " __VERSION__;
int         neofetch_compiler_text_color = Shellminator::WHITE;
int         neofetch_compiler_text_style = Shellminator::BOLD;

// Compile Date
const char* neofetch_compile_date_text = __DATE__;
int         neofetch_compile_date_text_color = Shellminator::WHITE;
int         neofetch_compile_date_text_style = Shellminator::BOLD;

// Author
const char* neofetch_author_text = "Daniel Hajnal";
int         neofetch_author_text_color = Shellminator::WHITE;
int         neofetch_author_text_style = Shellminator::BOLD;

// User Field 1
const char* neofetch_user_field_1_label = "License: ";
const char* neofetch_user_field_1_text = "MIT";
int         neofetch_user_field_1_text_color = Shellminator::WHITE;
int         neofetch_user_field_1_text_style = Shellminator::BOLD;

// User Field 2
const char* neofetch_user_field_2_label = "Manufacturer: ";
const char* neofetch_user_field_2_text = "Cyberdyne";
int         neofetch_user_field_2_text_color = Shellminator::WHITE;
int         neofetch_user_field_2_text_style = Shellminator::BOLD;

// User Field 3
const char* neofetch_user_field_3_label = "Parser: ";
const char* neofetch_user_field_3_text = "Commander-API";
int         neofetch_user_field_3_text_color = Shellminator::WHITE;
int         neofetch_user_field_3_text_style = Shellminator::BOLD;

// User Field 4
const char* neofetch_user_field_4_label = "System Memory: ";
const char* neofetch_user_field_4_text = "2kByte";
int         neofetch_user_field_4_text_color = Shellminator::WHITE;
int         neofetch_user_field_4_text_style = Shellminator::BOLD;

// User Field 5
const char* neofetch_user_field_5_label = "OS: ";
const char* neofetch_user_field_5_text = "mulatOS";
int         neofetch_user_field_5_text_color = Shellminator::WHITE;
int         neofetch_user_field_5_text_style = Shellminator::BOLD;

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
