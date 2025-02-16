#include "Shellminator-Neofetch.hpp"

const char* SHELLMINATOR_WEAK neofetch_header_text = "Shellminator";
int         SHELLMINATOR_WEAK neofetch_header_text_color = Shellminator::GREEN;
int         SHELLMINATOR_WEAK neofetch_header_text_style = Shellminator::BOLD;

char        SHELLMINATOR_WEAK neofetch_header_line_char = '-';
int         SHELLMINATOR_WEAK neofetch_header_line_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_header_line_style = Shellminator::BOLD;

int         SHELLMINATOR_WEAK neofetch_field_text_color = Shellminator::YELLOW;
int         SHELLMINATOR_WEAK neofetch_field_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_fw_text = SHELLMINATOR_VERSION;
int         SHELLMINATOR_WEAK neofetch_fw_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_fw_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_cpu_text = "T-800";
int         SHELLMINATOR_WEAK neofetch_cpu_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_cpu_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_compiler_text = SHELLMINATOR_COMPILER;
int         SHELLMINATOR_WEAK neofetch_compiler_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_compiler_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_compile_date_text = __DATE__;
int         SHELLMINATOR_WEAK neofetch_compile_date_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_compile_date_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_author_text = "Daniel Hajnal";
int         SHELLMINATOR_WEAK neofetch_author_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_author_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_user_field_1_label = NULL;
const char* SHELLMINATOR_WEAK neofetch_user_field_1_text = NULL;
int         SHELLMINATOR_WEAK neofetch_user_field_1_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_1_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_user_field_2_label = NULL;
const char* SHELLMINATOR_WEAK neofetch_user_field_2_text = NULL;
int         SHELLMINATOR_WEAK neofetch_user_field_2_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_2_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_user_field_3_label = NULL;
const char* SHELLMINATOR_WEAK neofetch_user_field_3_text = NULL;
int         SHELLMINATOR_WEAK neofetch_user_field_3_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_3_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_user_field_4_label = NULL;
const char* SHELLMINATOR_WEAK neofetch_user_field_4_text = NULL;
int         SHELLMINATOR_WEAK neofetch_user_field_4_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_4_text_style = Shellminator::BOLD;

const char* SHELLMINATOR_WEAK neofetch_user_field_5_label = NULL;
const char* SHELLMINATOR_WEAK neofetch_user_field_5_text = NULL;
int         SHELLMINATOR_WEAK neofetch_user_field_5_text_color = Shellminator::WHITE;
int         SHELLMINATOR_WEAK neofetch_user_field_5_text_style = Shellminator::BOLD;

void defaultShellminatorNeofetch( Shellminator* parent ){
    int i;
    int rowCounter = 0;

    if( parent == NULL ){
        return;
    }
    if( parent -> channel == NULL ){
        return;
    }

    parent -> format( parent -> channel, Shellminator::BOLD, Shellminator::WHITE );
    parent -> channel -> print( __CONST_TXT__( SHELLMINATOR_DEFAULT_NEOFETCH_ART ) );

    // Move up the cursor as many lines as high the logo was
    parent -> channel -> print( __CONST_TXT__( "\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_HEIGHT );
    parent -> channel -> print( __CONST_TXT__( "A\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );

    // Header text
    parent -> format( parent -> channel, neofetch_header_text_style, neofetch_header_text_color );
    parent -> channel -> print( neofetch_header_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // Header text underline
    parent -> format( parent -> channel, neofetch_header_line_style, neofetch_header_line_color );
    for( i = 0; i < strlen( neofetch_header_text ); i++ ){
        parent -> channel -> print( neofetch_header_line_char );
    }
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // Firmware text
    parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
    parent -> channel -> print( __CONST_TXT__( "FW: " ) );
    parent -> format( parent -> channel, neofetch_fw_text_style, neofetch_fw_text_color );
    parent -> channel -> print( neofetch_fw_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // CPU text
    parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
    parent -> channel -> print( __CONST_TXT__( "CPU: " ) );
    parent -> format( parent -> channel, neofetch_cpu_text_style, neofetch_cpu_text_color );
    parent -> channel -> print( neofetch_cpu_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // Compiler text
    parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
    parent -> channel -> print( __CONST_TXT__( "Compiler: " ) );
    parent -> format( parent -> channel, neofetch_compiler_text_style, neofetch_compiler_text_color );
    parent -> channel -> print( neofetch_compiler_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // Compile date text
    parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
    parent -> channel -> print( __CONST_TXT__( "Compile Date: " ) );
    parent -> format( parent -> channel, neofetch_compile_date_text_style, neofetch_compile_date_text_color );
    parent -> channel -> print( neofetch_compile_date_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    parent -> format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );

    // Author text
    parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
    parent -> channel -> print( __CONST_TXT__( "Author: " ) );
    parent -> format( parent -> channel, neofetch_author_text_style, neofetch_author_text_color );
    parent -> channel -> print( neofetch_author_text );
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // User Field 1
    if( neofetch_user_field_1_label ){
        parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
        parent -> channel -> print( neofetch_user_field_1_label );
        parent -> format( parent -> channel, neofetch_user_field_1_text_style, neofetch_user_field_1_text_color );
        parent -> channel -> print( neofetch_user_field_1_text );
        parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
        parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
        parent -> channel -> print( __CONST_TXT__( "C" ) );
        rowCounter++;    
    }

    // User Field 2
    if( neofetch_user_field_2_label ){
        parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
        parent -> channel -> print( neofetch_user_field_2_label );
        parent -> format( parent -> channel, neofetch_user_field_2_text_style, neofetch_user_field_2_text_color );
        parent -> channel -> print( neofetch_user_field_2_text );
        parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
        parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
        parent -> channel -> print( __CONST_TXT__( "C" ) );
        rowCounter++;    
    }

    // User Field 3
    if( neofetch_user_field_3_label ){
        parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
        parent -> channel -> print( neofetch_user_field_3_label );
        parent -> format( parent -> channel, neofetch_user_field_3_text_style, neofetch_user_field_3_text_color );
        parent -> channel -> print( neofetch_user_field_3_text );
        parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
        parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
        parent -> channel -> print( __CONST_TXT__( "C" ) );
        rowCounter++;    
    }

    // User Field 4
    if( neofetch_user_field_4_label ){
        parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
        parent -> channel -> print( neofetch_user_field_4_label );
        parent -> format( parent -> channel, neofetch_user_field_4_text_style, neofetch_user_field_4_text_color );
        parent -> channel -> print( neofetch_user_field_4_text );
        parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
        parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
        parent -> channel -> print( __CONST_TXT__( "C" ) );
        rowCounter++;    
    }

    // User Field 5
    if( neofetch_user_field_5_label ){
        parent -> format( parent -> channel, neofetch_field_text_style, neofetch_field_text_color );
        parent -> channel -> print( neofetch_user_field_5_label );
        parent -> format( parent -> channel, neofetch_user_field_5_text_style, neofetch_user_field_5_text_color );
        parent -> channel -> print( neofetch_user_field_5_text );
        parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
        parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
        parent -> channel -> print( __CONST_TXT__( "C" ) );
        rowCounter++;    
    }

    // Dimmed colors
    for( i = 30; i < 38; i++ ){
        parent -> channel -> print( __CONST_TXT__( "\33[2;" ) );
        parent -> channel -> print( i );
        parent -> channel -> print( __CONST_TXT__( "m\u2588\u2588" ) );
    }
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    // Bright colors
    for( i = 30; i < 38; i++ ){
        parent -> channel -> print( __CONST_TXT__( "\33[1;" ) );
        parent -> channel -> print( i );
        parent -> channel -> print( __CONST_TXT__( "m\u2588\u2588" ) );
    }
    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH );
    parent -> channel -> print( __CONST_TXT__( "C" ) );
    rowCounter++;

    if( rowCounter > SHELLMINATOR_DEFAULT_NEOFETCH_ART_HEIGHT ){
        parent -> format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
        return;
    }

    parent -> channel -> print( __CONST_TXT__( "\r\n\033[" ) );
    parent -> channel -> print( SHELLMINATOR_DEFAULT_NEOFETCH_ART_HEIGHT - rowCounter );
    parent -> channel -> print( __CONST_TXT__( "B" ) );

    parent -> format( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );

}