#include "Shellminator-Notification.hpp"

ShellminatorNotification::ShellminatorNotification(){
    text = NULL;
    textWidth = 0;
    textHeight = 0;
}

ShellminatorNotification::ShellminatorNotification( const char* text_p ){
    setText( text_p );
}

void ShellminatorNotification::setText( const char* text_p ){
    int i;
    int textWidthMax = 0;

    text = text_p;

    textWidth = 0;
    textHeight = 1;

    for( i = 0; i < strlen( text ); i++ ){

        if( text[ i ] == '\n' ){
            textHeight++;
            textWidth = 0;
            continue;
        }

        textWidth++;
        if( textWidth > textWidthMax ){
            textWidthMax = textWidth;
        }

    }

    textWidth = textWidthMax;
}

void ShellminatorNotification::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
}

void ShellminatorNotification::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    newEvent = parent -> readEvent();

    // In case of empty event, we can't do anything.
    if( newEvent.type == Shellminator::SHELL_EVENT_EMPTY ){
        return;
    }

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

}

void ShellminatorNotification::draw(){

    // Generic counter.
    int i;
    int x;
    int y;
    int widthCounter;
    int heightCounter;

    int borderWidth;
    int borderHeight;

    int xStart;
    int yStart;

    // Check for the necessary parameters.
    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    if( text == NULL ){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    borderWidth = textWidth + 4;
    borderHeight = textHeight + 2;

    if( borderWidth > width ){
        borderWidth = width;
    }

    if( borderHeight > height ){
        borderHeight = height;
    }

    xStart = width / 2 - borderWidth / 2;
    yStart = height / 2 - borderHeight / 2;

    parent -> format( channel, Shellminator::REGULAR, Shellminator::WHITE );

    // Set cursor to top left and print the instruction text.
    Shellminator::setCursorPosition( channel, xStart, yStart );

    channel -> print( __CONST_TXT__( "\u250C" ) );

    for( i = 0; i < borderWidth - 2; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" ) );
    }

    channel -> print( __CONST_TXT__( "\u2510" ) );


    i = 0;
    for( y = 0; y < borderHeight - 2; y++ ){
        Shellminator::setCursorPosition( channel, xStart, yStart + y + 1 );
        channel -> print( __CONST_TXT__( "\u2502 " ) );
        for( x = 0; x < borderWidth - 2; x++ ){
            if( text[ i ] == '\n' ){
                i++;
                break;
            }
            if( text[ i ] == '\0' ){
                break;
            }
            channel -> print( text[ i ] );
            i++;
        }
        Shellminator::setCursorPosition( channel, xStart + borderWidth - 2, yStart + y + 1 );
        channel -> print( __CONST_TXT__( " \u2502" ) );
    }

    // Set cursor to top left and print the instruction text.
    Shellminator::setCursorPosition( channel, xStart, yStart + borderHeight - 1 );

    channel -> print( __CONST_TXT__( "\u2514" ) );

    for( i = 0; i < borderWidth - 2; i++ ){
        channel -> print( __CONST_TXT__( "\u2500" )  );
    }

    channel -> print( __CONST_TXT__( "\u2139" ) );

}

