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
    plotRedrawPeriod = 500;
}

ShellminatorPlot::ShellminatorPlot(){
    data = NULL;
    dataSize = 0;
    name = NULL;
}


void ShellminatorPlot::init( Shellminator* parent_p ){
    parent = parent_p;
    
    if( parent == NULL ){
        return;
    }

    bufferingEnabled = true;
    selectedChannel = parent -> getBufferedPrinter();

    if( selectedChannel == NULL ){
        selectedChannel = parent -> channel;
        bufferingEnabled = false;
    }
}

void ShellminatorPlot::update( int width_p, int  height_p ){

    // Save the widht and height to internal variables.
    width = width_p;
    height = height_p;

    Shellminator::shellEvent_t newEvent;

    if( parent == NULL ){
        return;
    }

    if( ( millis() - plotTimerStart ) > plotRedrawPeriod ){
        plotTimerStart = millis();
        parent -> requestRedraw();
        redraw = true;
    }

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

void ShellminatorPlot::draw(){

    int i;
    int j;

    int textWidth;
    int limit;

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

    // Only draw if resized event or timer event happened.
    if( !redraw ){
        return;
    }

    redraw = false;
    
    // Calculate the length of the name.
    textWidth = strlen( name );

    // Set the name text color.
    Shellminator::setFormat( selectedChannel, Shellminator::BOLD, Shellminator::WHITE );

    // Set cursor to make the text appear on the top center.
    //Shellminator::setCursorPosition( selectedChannel, originX + width / 2 - j / 2, originY );
    Shellminator::setCursorPosition( selectedChannel, originX, originY );

    // Calculate text starting position.
    // It has to be centered.
    limit = ( width - textWidth ) / 2;

    // Print as many spaces as needed, to make the text centered.
    // We print spaces to clear the unwanted garbage from the line.
    for( i = 0; i < limit; i++ ){
        selectedChannel -> print( ' ' );
    }

    // Print instructions and plot name only at first draw.
    selectedChannel -> print( name );

    // Calculate how many characters left until line end.
    limit = width - ( limit + textWidth );

    // Print as many spaces as needed, to make the text centered.
    // We print spaces to clear the unwanted garbage from the line.
    for( i = 0; i < limit; i++ ){
        selectedChannel -> print( ' ' );
    }

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
    int currentTextSize;

    float tmp;
    float tmpDecimalPart;
    char sign[2] = { '\0', '\0' };

    // Set the correct style.
    Shellminator::setFormat( selectedChannel, Shellminator::BOLD, Shellminator::WHITE );

    // Draw the scale.
    for( i = 0; i < ( height - 1 ); i++ ){

        // Interpolate the value within the height steps.
        tmp = lerp( max, min, (float)i / (float)( height - 2 ) );

        // Calculate the numbers after the decimal point.
        // It has to be always positive!
        tmpDecimalPart = tmp;
        if( tmpDecimalPart < 0.0 ){
            tmpDecimalPart *= -1.0;
        }

        sign[ 0 ] = ' ';
        if( tmp < 0.0f  ){
            if( (int)tmp == 0 ){
                sign[ 0 ] = '-';
            }
            else{
                sign[ 0 ] = '\0';
            }
        }

        // Generate the string of the number to the data buffer.
        // Just in case terminate the end of the buffer to avoid
        // any string problems.
        snprintf( valueTextBuffer, sizeof( valueTextBuffer ), "%s%d.%d", sign, (int)tmp, (int)( (int)( tmpDecimalPart * 10.0 ) ) % 10 );
        valueTextBuffer[ sizeof( valueTextBuffer ) - 1 ] = '\0';

        // Calculate the length of the number
        // string and store it to variable currentTextSize.
        currentTextSize = strlen( valueTextBuffer );

        // Basic maximum value finding.
        // If the current text size is greater than the value
        // in valueTextSizeMax, overwrite valueTextSizeMax with it.
        // It is required to find the size of the largest printed
        // number on the Y axes.
        if( currentTextSize > valueTextSizeMax ){
            valueTextSizeMax = currentTextSize;
        }

        // Set the position of the current line.
        Shellminator::setCursorPosition( selectedChannel, originX, originY + 1 + i );

        // Print the actual data string to the output stream.
        selectedChannel -> print( valueTextBuffer );
        //selectedChannel -> print( "\033[0K" );

    }

    // Draw vertical scaling grid
    Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 1, originY + 1 );

    for( i = 0; i < ( height - 1 ); i++ ){

        selectedChannel -> print( "\u2524" );
        // Arrow down + left
        if( i < ( height - 2 ) ){
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
    for( i = 0; i < ( height - 1 ); i++ ){

        selectedChannel -> print( "\u251C" );
        
        // Arrow down + left
        if( i < ( height - 2 ) ){
            selectedChannel -> print( "\033[B\033[D" );
        }

    }

}

/*
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

            prevPointY = mapFloat( data[ i - 1 ], min, max, height - 1, 2 );
            prevPointX = mapFloat( i - 1, 0, ( dataSize - 1 ), 0, terminalWidth );

            pointY = mapFloat( data[ i ], min, max, height - 1, 2 );
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
            pointY = mapFloat( avg, min, max, height - 1, 2 );
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

            prevPointY = mapFloat( avg, min, max, height - 1, 2 );

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

*/

float ShellminatorPlot::lerp( float v0, float v1, float t ){
  return( 1.0 - t ) * v0 + t * v1;
}

float ShellminatorPlot::mapFloat( float x, float inStart, float inStop, float outStart, float outStop ){

  return outStart + ( outStop - outStart ) * ( ( x - inStart ) / ( inStop - inStart ) );

}


void ShellminatorPlot::drawPlot(){

    int i;
    int j;
    int terminalWidth;

    //float prevPointX;
    //float prevPointY;

    //float pointY;
    //float pointX;

    //int avgStartIndex;
    //int avgEndIndex;

    //float avg;

    int horizontalIndex;
    int verticalIndex;

    int horizontalIndexNext;
    int verticalIndexNext;

    const char* nextChar;

    Shellminator::setFormat( selectedChannel, Shellminator::REGULAR, color );

    terminalWidth = width - ( valueTextSizeMax + 2 ) - ( resultTextSize + 2 );

    for( i = 0; i < ( height - 1 ); i++ ){

        Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 2, originY + 1 + i );

        for( j = 0; j < terminalWidth; j++ ){

            /*
            // Check if we have a vertical line.
            if( ( i > 0 ) && ( i < ( height - 2 ) ) && ( j < ( terminalWidth - 1 ) ) ){

                

            }*/

            // Find the actual data point for the current horizontal position.
            horizontalIndex = lerp( 0, dataSize, (float)j / terminalWidth );
            verticalIndex = mapFloat( data[ horizontalIndex ], min, max, height-1, 0 );
            //Shellminator::setCursorPosition( selectedChannel, originX + valueTextSizeMax + 3 + i - 1, originY + verticalIndex );
            //selectedChannel -> print( '*' );

            nextChar = clearCell;

            if( j < ( terminalWidth - 1 ) ){
                horizontalIndexNext = lerp( 0, dataSize, (float)( j + 1 ) / terminalWidth );
                verticalIndexNext = mapFloat( data[ horizontalIndexNext ], min, max, height-1, 0 );
            }

            if( verticalIndex == i ){
                //selectedChannel -> print( "\u2022" );
                nextChar = dotCell;
            }

            else if( ( verticalIndex < i ) && ( verticalIndexNext > i ) ){
                //selectedChannel -> print( '+' );
                nextChar = dotCell;
            }

            else if( ( verticalIndex > i ) && ( verticalIndexNext < i ) ){
                //selectedChannel -> print( '+' );
                nextChar = dotCell;
            }

            selectedChannel -> print( nextChar );


        }

        // Cursor right with one characters.
        selectedChannel -> print( "\033[C" );

        // Check if it is the most right data point.
        if( verticalIndex == i ){
            // If it is, print the result value.
            selectedChannel -> print( valueTextBuffer );
        }
        else{
            // If it is not, print as many whitespace
            // characters, as long the result text.
            for( j = 0; j < resultTextSize; j++ ){
                selectedChannel -> print( ' ' );
            }
        }

    }

}

void ShellminatorPlot::setColor( Shellminator::textColor_t color_p ){
    color = color_p;
}

const char ShellminatorPlot::clearCell[2] = " ";
const char ShellminatorPlot::dotCell[5] = "\u2022";
