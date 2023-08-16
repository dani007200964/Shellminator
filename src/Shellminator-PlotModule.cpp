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

#include "Shellminator-PlotModule.hpp"

ShellminatorPlot::ShellminatorPlot( float* data_p, int dataSize_p, const char* name_p, int color_p ){
    data = data_p;
    dataSize = dataSize_p;
    name = name_p;
    color = color_p;
}

ShellminatorPlot::ShellminatorPlot(){
    data = NULL;
    dataSize = 0;
    name = NULL;
}


void ShellminatorPlot::init( Shellminator* parent_p ){
    parent = parent_p;
    
    bufferingEnabled = true;
    selectedChannel = parent -> getBufferedPrinter();

    if( selectedChannel == NULL ){
        selectedChannel = parent -> channel;
        bufferingEnabled = false;
    }
}

void ShellminatorPlot::update( int width_p, int  height_p ){
    // Save the arguments to internal variables.
    // It is required to make these parameters accessible
    // to the other member functions.
    width = width_p;
    height = height_p - 1;
}

bool ShellminatorPlot::redrawRequest(){
    return true;
}


void ShellminatorPlot::draw(){

    int i;
    int j;

    if( parent == NULL ){
        return;
    }

    if( selectedChannel == NULL ){
        return;
    }

    // Handle incorrect input data.
    if( data == NULL ){
        return;
    }

    // Handle incorrect input data size.
    if( dataSize <= 0 ){
        return;
    }

    if( name == NULL ){
        return;
    }

    // Calculate the length of the name.
    j = strlen( name );

    // Set the name text color.
    Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::UNDERLINE, Shellminator::WHITE );

    // Set cursor to make the text appear on the top center.
    Shellminator::setCursorPosition( selectedChannel, originX + width / 2 - j / 2, originY );

    // Print instructions and plot name only at first draw.
    selectedChannel -> print( name );

    // Set the cursor to the origin position.
    Shellminator::setCursorPosition( selectedChannel, originX, originY );

    // Set the name text color.
    Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::UNDERLINE, Shellminator::WHITE );

    // Set cursor to make the text appear on the top center.
    Shellminator::setCursorPosition( selectedChannel, originX + width / 2 - j / 2, originY );

    // Print instructions and plot name only at first draw.
    selectedChannel -> print( name );

    // Set the cursor to the origin position.
    Shellminator::setCursorPosition( selectedChannel, originX, originY );


    // Reset variables to a safe state.
    min = data[ 0 ];
    max = data[ 0 ];

    // Calculate the min and max values for the scale.
    for( i = 0; i < dataSize; i++ ){

        if( min > data[ i ] ){

            min = data[ i ];

        }

        if( max < data[ i ] ){

            max = data[ i ];

        }

    }

    // To round down
    min = ( (int)min ) - 1;

    // To round up
    max = ( (int)max ) + 1;

    // Draw the scaling grid with numbers.
    drawScale();

    // Draw the data points.
    drawPlot();

    if( bufferingEnabled ){
        selectedChannel -> flush();
    }

}

void ShellminatorPlot::drawScale(){

    int i;
    int j;

    float tmp;
    float tmpDecimalPart;

    // Set the correct style.
    Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::REGULAR, Shellminator::WHITE );
    Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::BOLD, Shellminator::WHITE );

    // Draw the scale.
    for( i = 0; i < height; i++ ){

        // Interpolate the value within the height steps.
        tmp = lerp( max, min, (float)i / (float)( height -1 ) );

        // Calculate the numbers after the decimal point.
        // It has to be always positive!
        tmpDecimalPart = tmp;
        if( tmpDecimalPart < 0.0 ){
            tmpDecimalPart *= -1.0;
        }

        // Generate the string of the number to the data buffer.
        // Just in case terminate the end of the buffer to avoid
        // any string problems.
        snprintf( valueTextBuffer, sizeof( valueTextBuffer ), "%3d.%d", (int)tmp, (int)( (int)( tmpDecimalPart * 10.0 ) ) % 10 );
        valueTextBuffer[ sizeof( valueTextBuffer ) - 1 ] = '\0';

        // Calculate the length of the number
        // string and store it to variable j.
        j = strlen( valueTextBuffer );

        // Basic maximum value finding.
        // If the current text size is greater than the value
        // in valueTextSizeMax, overwrite valueTextSizeMax with it.
        // It is required to find the size of the largest printed
        // number on the Y axes.
        if( j > valueTextSizeMax ){
            valueTextSizeMax = j;
        }

        // Set the position of the current line.
        Shellminator::setCursorPosition( selectedChannel, originX, originY + 1 + i );

        // Print the actual data string to the output stream.
        selectedChannel -> print( valueTextBuffer );
        selectedChannel -> print( "\033[0K" );

    }

    // Draw vertical scaling grid
    Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 1, originY + 1 );

    for( i = 0; i < height; i++ ){

        selectedChannel -> print( "\u2524" );
        // Arrow down + left
        if( i < ( height - 1 ) ){
            selectedChannel -> print( "\033[B\033[D" );
        }

    }

    // Generate the value string for the last element.
    // Save the last elements value to the tmp variable.
    tmp = data[ dataSize - 1 ];

    // Calculate the numbers after the decimal point.
    // It has to be always positive!
    tmpDecimalPart = tmp;
    if( tmpDecimalPart < 0.0 ){
        tmpDecimalPart *= -1.0;
    }

    // Generate string from the last element.
    snprintf( valueTextBuffer, sizeof( valueTextBuffer ), " %d.%d", (int)tmp, (int)( (int)( tmpDecimalPart * 10.0 ) ) % 10 );
    valueTextBuffer[ sizeof( valueTextBuffer ) - 1 ] = '\0';

    // Calculate the length of the result text.
    resultTextSize = strlen( valueTextBuffer );

    // Set the cursor to the result scaling grid position.
    Shellminator::setCursorPosition( selectedChannel, originX + width - resultTextSize - 2, originY + 1 );

    // Draw vertical scaling grid to the end for the result.
    for( i = 0; i < height; i++ ){

        selectedChannel -> print( "\u251C" );
        // Arrow down + left
        if( i < ( height - 1 ) ){
            selectedChannel -> print( "\033[B\033[D" );
        }

    }

}

void ShellminatorPlot::drawPlot(){

    int i;
    int j;
    int terminalWidth;

    float prevPointX;
    float prevPointY;

    float pointY;
    float pointX;

    int avgStartIndex;
    int avgEndIndex;

    float avg;

    Shellminator::setTerminalCharacterColor( selectedChannel, Shellminator::REGULAR, color );

    terminalWidth = width - ( valueTextSizeMax + 2 ) - ( resultTextSize + 3 );

    // We have less data points than 'pixels' in the terminal
    if( ( dataSize / terminalWidth ) == 0 ){

        for( i = 1; i < dataSize; i++ ){

            prevPointY = mapFloat( data[ i - 1 ], min, max, height, 2 );
            prevPointX = mapFloat( i - 1, 0, ( dataSize - 1 ), 0, terminalWidth );

            pointY = mapFloat( data[ i ], min, max, height, 2 );
            pointX = mapFloat( i, 0, ( dataSize - 1 ), 0, terminalWidth );

            Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 2 + prevPointX, originY + prevPointY );

            // Print the horizontal line
            for( j = 0; j < ( (int)pointX - (int)prevPointX ); j++ ){

                selectedChannel -> print( "\u2022" );

            }

            // Print vertical line everywhere except the last data point.
            if( i < ( dataSize - 1 ) ){
                
                // Check if we have to go down.
                if( (int)pointY > (int)prevPointY ){

                    for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){

                        selectedChannel -> print( "\033[B\033[D" );
                        selectedChannel -> print( "\u2022" );

                    }

                }

                // Check if we have to go Up.
                if( (int)prevPointY > (int)pointY ){

                    for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){

                        selectedChannel -> print( "\033[A\033[D" );
                        selectedChannel -> print( "\u2022" );

                    }

                }

            }

        }

    }

    // We have more points than 'pixels'.
    else{

        // Az elérhető szélességgel számolva minden pontot úgy rajzolunk ki, hogy
        // A ponthoz megkeressük a közvetlen környezetében lévő mintákhoz tartozó indexeket.
        // pédául a 3. pontot akarom kirajzolni, akkor annak a közvetlen környezete a 2. ponthoz tartozó
        // minta után és a 4. pointhoz tartó mintáig tart. Ezeknek veszem az átlagát és azt rajzolom ki.

        for( i = 1; i < terminalWidth; i++ ){

            // Finding the indexes for the left and right boundaries
            avgStartIndex = mapFloat( i - 1, 0, terminalWidth, 0, dataSize );
            avgEndIndex = mapFloat( i + 1, 0, terminalWidth, 0, dataSize );

            // Check and handle buffer overflow.
            if( avgStartIndex < 0 ){
                avgStartIndex = 0;
            }

            // Check and handle buffer overflow.
            if( avgEndIndex >= dataSize ){
                avgEndIndex = dataSize - 1;
            }

            // Averaging the numbers in the specified range
            avg = 0.0;
            for( j = avgStartIndex; j < avgEndIndex; j++ ){
                avg += data[ j ];
            }

            // Protect against zero division.
            if( ( avgEndIndex - avgStartIndex ) != 0 ){
                avg /= (float)( avgEndIndex - avgStartIndex );
            }
            else{
                return;
            }

            // Calculate coordinates for the current point.
            pointY = mapFloat( avg, min, max, height, 2 );
            pointX = i;

            // Do the same for the previous point
            // Finding the indexes for the laft and right boundaries
            avgStartIndex = mapFloat( i - 2, 0, terminalWidth, 0, dataSize );
            avgEndIndex = mapFloat( i, 0, terminalWidth, 0, dataSize );

            // Check and handle buffer overflow.
            if( avgStartIndex < 0 ){
                avgStartIndex = 0;
            }

            // Check and handle buffer overflow.
            if( avgEndIndex >= dataSize ){
                avgEndIndex = dataSize - 1;
            }

            // Averaging the numbers in the specified range
            avg = 0.0;
            for( j = avgStartIndex; j < avgEndIndex; j++ ){
                avg += data[ j ];
            }

            // Protect against zero division.
            if( ( avgEndIndex - avgStartIndex ) != 0 ){
                avg /= (float)( avgEndIndex - avgStartIndex );
            }
            else{
                return;
            }

            prevPointY = mapFloat( avg, min, max, height, 2 );

            Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 3 + i - 1, originY + prevPointY );
            selectedChannel -> print( "\u2022" );

            // Print vertical line everywhere except the last data point.
            if( i < ( dataSize - 1 ) ){
                
                // Check if we have to go down.
                if( (int)pointY > (int)prevPointY ){

                    for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){

                        selectedChannel -> print( "\033[B\033[D" );
                        selectedChannel -> print( "\u2022" );

                    }

                }

                // Check if we have to go Up.
                if( (int)prevPointY > (int)pointY ){

                    for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){

                        selectedChannel -> print( "\033[A\033[D" );
                        selectedChannel -> print( "\u2022" );

                    }

                }

            }

        }

    }

    selectedChannel -> print( "\033[C\033[C" );
    selectedChannel -> print( valueTextBuffer );

}

float ShellminatorPlot::lerp( float v0, float v1, float t ){
  return( 1.0 - t ) * v0 + t * v1;
}

float ShellminatorPlot::mapFloat( float x, float inStart, float inStop, float outStart, float outStop ){

  return outStart + ( outStop - outStart ) * ( ( x - inStart ) / ( inStop - inStart ) );

}