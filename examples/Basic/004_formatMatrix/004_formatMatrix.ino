
/*
 * Created on Aug 10 2020
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2023.05.13
*/


#include "Shellminator.hpp"


// Create a Shellminator object, and initialize it to use Serial
Shellminator shell( &Serial );

int format_table[] = {
    Shellminator::BOLD,
    Shellminator::LOW_INTENSITY,
    Shellminator::ITALIC,
    Shellminator::UNDERLINE,
    Shellminator::BLINKING,
    Shellminator::REVERSE,
    Shellminator::BACKGROUND,
    Shellminator::INVISIBLE
};

int format_table_size = sizeof( format_table ) / sizeof( format_table[ 0 ] );

const char* bold_str = "Bold";
const char* low_int_str = "Low Intensity";
const char* italic_str = "Italic";
const char* underline_str = "Underline";
const char* blink_str = "Blink";
const char* reverse_str = "Reverse";
const char* background_str = "Background";
const char* invisible_str = "Invisible";

const char* name_table[] = {
    bold_str,
    low_int_str,
    italic_str,
    underline_str,
    blink_str,
    reverse_str,
    background_str,
    invisible_str
};

int color_table[] = {
    Shellminator::BLACK,
    Shellminator::RED,
    Shellminator::GREEN,
    Shellminator::YELLOW,
    Shellminator::BLUE,
    Shellminator::MAGENTA,
    Shellminator::CYAN,
    Shellminator::WHITE 
};

int color_table_size = sizeof( color_table ) / sizeof( color_table[ 0 ] );

int background_color_table[] = {
    Shellminator::BG_BLACK,
    Shellminator::BG_RED,
    Shellminator::BG_GREEN,
    Shellminator::BG_YELLOW,
    Shellminator::BG_BLUE,
    Shellminator::BG_MAGENTA,
    Shellminator::BG_CYAN,
    Shellminator::BG_WHITE 
};





// System init section.
void setup(){

    Serial.begin(115200);

    int x;
    int y;

    // Clear the terminal
    shell.clear();
    Serial.println( "--- Style Matrix ---");
    Serial.println();

    for( y = 0; y < format_table_size; y++ ){
        for( x = 0; x < color_table_size; x++ ){
            shell.format( &Serial, color_table[ x ], format_table[ y ] );
            Serial.print( "X");
            shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        }
        Serial.print( "  " );
        Serial.println( name_table[ y ] );
    }

    Serial.println();
    Serial.println( "--- Background Matrix ---");
    Serial.println();

    for( y = 0; y < color_table_size; y++ ){
        for( x = 0; x < color_table_size; x++ ){
            shell.format( &Serial, color_table[ x ], background_color_table[ y ] );
            Serial.print( "X");
            shell.format( &Serial, Shellminator::REGULAR, Shellminator::WHITE );
        }
        Serial.println();
    }


}

// Infinite loop.
void loop(){



}
