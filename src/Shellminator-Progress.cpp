#include "Shellminator-Progress.hpp"

ShellminatorProgress::ShellminatorProgress( Shellminator* shell_p ){

    shell = shell_p;
    channel = NULL;

}

ShellminatorProgress::ShellminatorProgress( Stream* channel_p ){

    shell = NULL;
    channel = channel_p;

}

void ShellminatorProgress::drawProgressBarShell( float percentage, const char *text, const char* done, const char* todo, uint8_t doneColor, uint8_t todoColor ){

    float ratio;
    uint32_t i;
    bool colored = false;
    bool todoColorChange = false;

    if( percentage > 100.0f ){
        percentage = 100.0f;
    }

    if( percentage < 0.0f ){
        percentage = 0.0f;
    }

    if( shell == NULL ){
        return;
    }

    // If buffering is not available for the shell object, we just simply use the channel from the
    // shell object and print the progress bar to it.
    if( shell -> bufferMemoryAllocated == false ){
        drawProgressBarStream( shell -> channel, percentage, text, done, todo, doneColor, todoColor );
        return;
    }

    // Check if color formatting requested
    if( ( todoColor != 0 ) || ( todoColor != 0 ) ){
        colored = true;
    }

    ratio = percentage / 100.0;

    shell -> bufferedPrinter.printf( "\r\033[0K%3d.%d%% ", (int)percentage, (int)((float)percentage * 10.0) % 10 );

    if( colored ){
        Shellminator::setTerminalCharacterColor( &shell->bufferedPrinter, (uint8_t)Shellminator::REGULAR, doneColor );
    }

    for( i = 0; i < SHELLMINATOR_PROGRESS_BAR_SIZE; i++ ){

        if( ( (float)i / SHELLMINATOR_PROGRESS_BAR_SIZE ) < ratio ){

            shell -> bufferedPrinter.printf( "%s", done );

        }

        else{

            // Check done -> todo transition
            if( todoColorChange == false ){

                todoColorChange = true;

                if( colored ){

                    Shellminator::setTerminalCharacterColor( &shell->bufferedPrinter, (uint8_t)Shellminator::REGULAR, todoColor );

                }

            }

            shell -> bufferedPrinter.printf( "%s", todo );

        }
    }

    if( colored ){
        Shellminator::setTerminalCharacterColor( &shell->bufferedPrinter, (uint8_t)Shellminator::REGULAR, (uint8_t)Shellminator::WHITE );
    }

    shell -> bufferedPrinter.printf( " | %s", text );

    shell -> bufferedPrinter.flush();

}

void ShellminatorProgress::drawProgressBarStream( Stream* channel_p, float percentage, const char* text, const char* done, const char* todo, uint8_t doneColor, uint8_t todoColor ){

    float ratio;
    uint32_t i;
    bool colored = false;
    bool todoColorChange = false;

    if( percentage > 100.0f ){
        percentage = 100.0f;
    }

    if( percentage < 0.0f ){
        percentage = 0.0f;
    }

    if( channel_p == NULL ){
        return;
    }

    // Check if color formatting requested
    if( ( todoColor != 0 ) || ( todoColor != 0 ) ){
        colored = true;
    }

    ratio = percentage / 100.0;

    channel_p -> print( "\r\033[0K" );

    // For padding.
    if( ( (int)percentage ) < 100 ){

        channel_p -> print( ' ' );

    }

    if( ( (int)percentage ) < 10 ){

        channel_p -> print( ' ' );

    }
    
    channel_p -> print( (int)percentage );
    channel_p -> print( '.' );
    channel_p -> print( (int)((float)percentage * 10.0) % 10 );
    channel_p -> print( "% " );

    if( colored ){
        Shellminator::setTerminalCharacterColor( channel_p, (uint8_t)Shellminator::REGULAR, doneColor );
    }

    for( i = 0; i < SHELLMINATOR_PROGRESS_BAR_SIZE; i++ ){

        if( ( (float)i / SHELLMINATOR_PROGRESS_BAR_SIZE ) < ratio ){

            channel_p -> print( done );

        }

        else{

            // Check done -> todo transition
            if( todoColorChange == false ){

                todoColorChange = true;

                if( colored ){

                    Shellminator::setTerminalCharacterColor( channel_p, (uint8_t)Shellminator::REGULAR, todoColor );

                }

            }

            channel_p -> print( todo );

        }
    }

    if( colored ){
        Shellminator::setTerminalCharacterColor( channel_p, (uint8_t)Shellminator::REGULAR, (uint8_t)Shellminator::WHITE );
    }

    channel_p -> print( " | " );
    channel_p -> print( text );

}

void ShellminatorProgress::drawProgressBar( float percentage, const char *text, const char* done_p, const char* todo_p, uint8_t doneColor_p, uint8_t todoColo_p ){

    if( channel ){

        drawProgressBarStream( channel, percentage, text, done_p, todo_p, doneColor_p, todoColo_p );
        return;

    }

    if( shell ){

        drawProgressBarShell( percentage, text, done_p, todo_p, doneColor_p, todoColo_p );
        return;

    }

}

void ShellminatorProgress::drawProgressBar( float percentage, const char *text, const char* done_p, const char* todo_p ){

    if( channel ){

        drawProgressBarStream( channel, percentage, text, done_p, todo_p, 0, 0 );
        return;

    }

    if( shell ){

        drawProgressBarShell( percentage, text, done_p, todo_p, 0, 0 );
        return;

    }

}

void ShellminatorProgress::drawProgressBar( float percentage, const char *text ){

    if( channel ){

        drawProgressBarStream( channel, percentage, text, "#", "-", 0, 0 );
        return;

    }

    if( shell ){

        drawProgressBarShell( percentage, text, "#", "-", 0, 0 );
        return;

    }

}