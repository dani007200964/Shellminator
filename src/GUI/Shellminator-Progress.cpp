/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

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

#include "Shellminator-Progress.hpp"

ShellminatorProgress::ShellminatorProgress(){
    redraw = true;
}

void ShellminatorProgress::init( Shellminator* parent_p, Stream* channel_p ){
    parent = parent_p;
    channel = channel_p;
}

void ShellminatorProgress::draw(){

    // Ratio used to draw[ 0.0 -1.0 ].
    float ratio;

    // Generic counter
    int i;

    // Used to calculate the target for the for loops.
    int limit;

    // The printed text size will be calculated to this variable.
    int textSize;

    // Used for remaining time calculation.
    int remainingTime;

    if( parent == NULL ){
        return;
    }

    if( channel == NULL){
        return;
    }

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }
    redraw = false;

    // Set cursor to top left.
    Shellminator::setCursorPosition( channel, originX, originY );

    // Copy percentage value to ratio.
    ratio = percentage;

    // Limit the ratio to a valid range.
    if( ratio > 100.0f ){
        ratio = 100.0f;
    }

    if( ratio < 0.0f ){
        ratio = 0.0f;
    }

    // For padding.
    if( ( (int)ratio ) < 100 ){
        channel -> print( ' ' );
    }

    if( ( (int)ratio ) < 10 ){
        channel -> print( ' ' );
    }
    
    // Print percentage.
    channel -> print( (int)ratio );
    channel -> print( '.' );
    channel -> print( (int)((float)ratio * 10.0) % 10 );
    channel -> print( "%\u2502" );

    // Transform ratio to 0.0 - 1.0.
    ratio = ratio / 100.0;

    // If we do not have a text or a format, we just
    // render the progress bar.
    if( ( text == NULL ) && ( format == NULL ) ){

        // Calculate how many 'bars' has to be rendered.
        limit = (float)( width - 9 ) * ratio + 9;

        // Set the color to the specified one.    
        Shellminator::setFormat_m( channel, color );

        // It has to start from 9 to avoid xTerm glitch.
        for( i = 9; i < limit; i++ ){

            // Decide to render a bar or a blank space.
            if( i < limit ){
                channel -> print( "\u2588" );
            }

            else{
                channel -> print( ' ' );
            }

        }

        // Set back formatting to regular.
        Shellminator::setFormat_m( channel, Shellminator::WHITE );

        // Draw progress bar end character.
        channel -> print( "\u2502" );

    }

    else{

        // Calculate how many 'bars' has to be rendered.
        limit = (float)( width / 2 - 10 ) * ratio + 10;

        // Set the color to the specified one.    
        Shellminator::setFormat_m( channel, color );

        // It has to start from 10 to avoid xTerm glitch.
        for( i = 10; i < ( width / 2 ); i++ ){

            // Decide to render a bar or a blank space.
            if( i < limit ){
                channel -> print( "\u2588" );
            }

            else{
                channel -> print( ' ' );
            }

        }

        // Set back formatting to regular.
        Shellminator::setFormat_m( channel, Shellminator::WHITE );

        // Draw progress bar end character.
        channel -> print( "\u2502 " );

        // Reset text size.
        textSize = 0;

        // Check if a format is specified or not.
        if( format != NULL ){

            // Go through every format character and print according.
            for( i = 0; i < strlen( format ); i++ ){

                switch( format[ i ] ){
                    
                    // Regular separator.
                    case '|':
                        textSize += channel -> print( '|' );
                        break;

                    // Blank space.
                    case ' ':
                        textSize += channel -> print( ' ' );
                        break;
                    
                    // Seconds since start event.
                    case 's':
                        textSize += channel -> print( ( millis() - timerStart ) / 1000 );
                        textSize += channel -> print( "s " );
                        break;

                    // Minutes since start event.
                    case 'm':
                        textSize += channel -> print( ( millis() - timerStart ) / 1000 / 60 );
                        textSize += channel -> print( "m " );
                        break;

                    // Time since start
                    case 't':
                        // I know, the naming is not the best, but I need a buffer variable.
                        remainingTime = ( ( millis() - timerStart ) / 1000 ); // <- seconds since start event.
                        textSize += channel -> print( ( millis() - timerStart ) / 1000 / 60 );

                        // Print remaining minutes.
                        if( ( remainingTime / 60 ) < 10 ){
                            // For padding.
                            textSize += channel -> print( '0' );
                        }
                        textSize += channel -> print( remainingTime / 60 );
                        textSize += channel -> print( ':' );
                        remainingTime %= 60;

                        // Print remaining seconds
                        if( ( remainingTime ) < 10 ){
                            // For padding.
                            textSize += channel -> print( '0' );
                        }
                        textSize += channel -> print( remainingTime );
                        textSize += channel -> print( ' ' );

                        break;

                    // Remaining time.
                    case 'r':
                        // We have to check if the ratio is within boundaries
                        // to avoid division by zero.
                        if( ( ratio > 0.0 ) && ( ratio < 1.0 ) ){

                            // Calculate remaining seconds.
                            remainingTime = ( ( millis() - timerStart ) / 1000 );
                            remainingTime = remainingTime * ( 1.0 / ratio ) - remainingTime;
                            
                            // Print remaining minutes.
                            if( ( remainingTime / 60 ) < 10 ){
                                // For padding.
                                textSize += channel -> print( '0' );
                            }
                            textSize += channel -> print( remainingTime / 60 );
                            textSize += channel -> print( ':' );
                            remainingTime %= 60;

                            // Print remaining seconds
                            if( ( remainingTime ) < 10 ){
                                // For padding.
                                textSize += channel -> print( '0' );
                            }
                            textSize += channel -> print( remainingTime );

                        }
                        else{
                            // If we are out of range print invalid timing data.
                            textSize += channel -> print( "-:-" );
                        }

                        // Finish the message.
                        textSize += channel -> print( " left " );
                        break;

                    default:
                        break;
                }

            }

        }

        // Check if we have a message to print.
        if( text != NULL ){

            // Calculate message size and add it to the
            // already existing size.
            textSize += strlen( text );

            // Calculate how many characters left in the row.
            limit = width - ( width / 2 );

            // Check if the limit is greater, than the text.
            if( limit > textSize ){
                // We can not print more characters than the
                // text contains.
                limit = textSize;
            }

            // Print as many characters from the text as possible.
            for( i = 0; i < limit; i++ ){
                // We have to check string termination.
                if( text[ i ] == '\0' ){
                    // If we reached string end, we clear the rest of
                    // of the line and return.
                    channel -> print( "\033[0K" );
                    return;
                }

                // Print next character from the string.
                channel -> print( text[ i ] );
            }

        }

        // Clear the rest of the line.
        channel -> print( "\033[0K" );

    }

}

void ShellminatorProgress::update( int width_p, int  height_p ){

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    width = width_p;
    height = height_p;

    if( height > 1 ){
        height = 1;
    }

    // Try to read the next event in the eventBuffer.
    newEvent = parent -> readEvent();

    if( newEvent.type == Shellminator::SHELL_EVENT_RESIZE ){
        redraw = true;
        return;
    }

}

void ShellminatorProgress::setText( const char* text_p ){
    text = text_p;
    redraw = true;
}

void ShellminatorProgress::setFormat( const char* format_p ){
    format = format_p;
    redraw = true;
}

void ShellminatorProgress::setPercentage( float percentage_p ){

    if( parent == NULL ){
        return;
    }

    if( ( (int)( percentage_p * 10.0 ) ) != ( (int)( percentage * 10.0 ) ) ){
        redraw = true;
        parent -> requestRedraw();
    }

    percentage = percentage_p;
}

void ShellminatorProgress::setStep( int current, int total ){
    percentage = (float)current / (float)total * 100.0;
}

void ShellminatorProgress::setColor( Shellminator::textColor_t color_p ){
    color = color_p;
}

void ShellminatorProgress::start(){
    timerStart = millis();
}
