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


void ShellminatorPlot::init(){
}

void ShellminatorPlot::setOrigin( int x, int y ){
    originX = x;
    originY = y;

    if( originX < 1 ){
        originX = 1;
    }

    if( originY < 1 ){
        originY = 1;
    }

}

void ShellminatorPlot::draw( Shellminator* parent_p, int width_p, int  height_p ){

    int i;
    int j;

    if( parent_p == NULL ){
        return;
    }

    if( parent_p -> channel == NULL ){
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

    // Save the arguments to internal variables.
    // It is required to make these parameters accessible
    // to the other member functions.
    parent = parent_p;
    width = width_p;
    height = height_p - 1;

    // Set the name text color.
    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::UNDERLINE, Shellminator::WHITE );

    // Calculate the length of the name.
    j = strlen( name );

    // Set cursor to make the text appear on the top center.
    parent -> setCursorPosition( originX + width / 2 - j / 2, originY );

    // Print instructions and plot name only at first draw.
    parent -> channel -> print( name );

    // Set the cursor to the origin position.
    parent -> setCursorPosition( originX, originY );

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

    drawScale();

    drawPlot();

}

void ShellminatorPlot::drawScale(){

    int i;
    int j;

    float tmp;
    float tmpDecimalPart;

    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::REGULAR, Shellminator::WHITE );
    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::BOLD, Shellminator::WHITE );

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
        parent -> setCursorPosition( originX, originY + 1 + i );

        // Print the actual data string to the output stream.
        parent -> channel -> print( valueTextBuffer );
        parent -> channel -> print( "\033[K" );

        // We have to print a new line only if we are not in the last iteration.
        if( i < ( height - 1 ) ){
            //parent -> channel -> println();
        }

    }

    // Draw vertical scaling grid
    parent -> setCursorPosition( originX + valueTextSizeMax + 1, originY + 1 );

    for( i = 0; i < height; i++ ){

        parent -> channel -> print( "\u2524" );

        if( i < ( height - 1 ) ){
            parent -> channel -> print( "\033[B\033[D" );
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
    parent -> setCursorPosition( originX + width - resultTextSize - 2, originY + 1 );

    // Draw vertical scaling grid to the end for the result.
    for( i = 0; i < height; i++ ){

        parent -> channel -> print( "\u251C" );
        
        if( i < ( height - 1 ) ){
            parent -> channel -> print( "\033[B\033[D" );
        }

  }


/*
  int i;
  int j;
  char yTextBuffer[ 15 ];
  float tmp;
  float tmpPos;

  shell -> setCursorPosition( 1, 1 );

  // Draw the numbers between min and max
  yTextSize = 0;
  for( i = 0; i < terminalSizeY; i++ ){

    tmp = lerp( max, min, (float)i / (float)( terminalSizeY -1 ) );
    tmpPos = tmp;
    if( tmpPos < 0.0 ){
      tmpPos *= -1.0;
    }
    snprintf( yTextBuffer, sizeof( yTextBuffer ), "%3d.%d", (int)tmp, (int)( (int)( tmpPos * 10.0 ) ) % 10 );
    j = strlen( yTextBuffer );
    if( j > yTextSize ){
      yTextSize = j;
    }

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "%s%s", yTextBuffer, ( i < ( terminalSizeY - 1 ) ) ? "\r\n" : "" );

    #else

    shell -> channel -> print( yTextBuffer );

    if( i < ( terminalSizeY - 1 ) ){
      shell -> channel -> println();
    }

    #endif

  }

  // Draw vertical scaling grid

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.printf( "\033[%d;%dH", 1, yTextSize + 2 );

  #else

  shell -> setCursorPosition( yTextSize + 2, 1 );

  #endif

  for( i = 0; i < terminalSizeY; i++ ){

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "\u2524%s", ( i < ( terminalSizeY - 1 ) ) ? "\033[B\033[D" : "" );

    #else

    shell -> channel -> print( "\u2524" );
    
    if( i < ( terminalSizeY - 1 ) ){
      shell -> channel -> print( "\033[B\033[D" );
    }

    #endif

  }

  // Draw result values
  resultTextSize = 0;

  // Calculate the width of the largest value in characters.
  for( i = 0; i < numberOfPlots; i++ ){

    snprintf( yTextBuffer, sizeof( yTextBuffer ), "%3d", (int)yDataF[i][ yDataSize - 1 ] );
    
    j = strlen( yTextBuffer );
    if( j > resultTextSize ){
      resultTextSize = j;
    }

  }

  // Required for decimal places.
  resultTextSize += 2;

  // Plot the values.
  for( i = 0; i < numberOfPlots; i++ ){

    // calculate the position of the last element on the y axis.
    tmp = mapFloat( yDataF[i][ yDataSize - 1 ], max, min, 1, terminalSizeY );
    j = tmp;

    // Save the actual value of the last element to tmp.
    tmp = yDataF[i][ yDataSize - 1 ];
    tmpPos = tmp;
    if( tmpPos < 0.0 ){
      tmpPos *= -1.0;
    }

    snprintf( yTextBuffer, sizeof( yTextBuffer ), "%3d.%d", (int)tmp, (int)( (int)( tmpPos * 10.0 ) ) % 10 );

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "\033[K\033[%d;%dm\033[%d;%dH", (int)Shellminator::REGULAR, plotColor[ i ], j, terminalSizeX - resultTextSize );
    bufferedPrinter.printf( "%s", yTextBuffer );

    #else

    // Erase to the end of line.
    shell -> channel -> print( "\033[K" );
    shell -> setTerminalCharacterColor( Shellminator::REGULAR, plotColor[ i ] );
    shell -> setCursorPosition( terminalSizeX - resultTextSize, j );
    shell -> channel -> print( yTextBuffer );

    #endif

  }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.printf( "\033[%d;%dH\033[%d;%dm", 1, terminalSizeX - resultTextSize - 1, (int)Shellminator::REGULAR, (int)Shellminator::WHITE );

  #else

  shell -> setCursorPosition( terminalSizeX - resultTextSize - 1, 1 );
  shell -> setTerminalCharacterColor( Shellminator::REGULAR, Shellminator::WHITE );

  #endif

  for( i = 0; i < terminalSizeY; i++ ){

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "\u251C%s", ( i < ( terminalSizeY - 1 ) ) ? "\033[B\033[D" : "" );

    #else

    shell -> channel -> print( "\u251C" );
    
    if( i < ( terminalSizeY - 1 ) ){
      shell -> channel -> print( "\033[B\033[D" );
    }

    #endif

  }
  */

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

    Shellminator::setTerminalCharacterColor( parent -> channel, Shellminator::REGULAR, color );
    // terminalWidth = width - ( valueTextSizeMax + 2 ) - ( resultTextSize + 3 );
    terminalWidth = width - ( valueTextSizeMax + 2 ) - ( resultTextSize + 3 );

    // We have less data points than 'pixels' in the terminal
    if( ( dataSize / terminalWidth ) == 0 ){

        for( i = 1; i < dataSize; i++ ){

            prevPointY = mapFloat( data[ i - 1 ], min, max, height, 2 );
            prevPointX = mapFloat( i - 1, 0, ( dataSize - 1 ), 0, terminalWidth );

            pointY = mapFloat( data[ i ], min, max, height, 2 );
            pointX = mapFloat( i, 0, ( dataSize - 1 ), 0, terminalWidth );

            //parent -> setCursorPosition( valueTextSizeMax + 3 + prevPointX, prevPointY );
            parent -> setCursorPosition( originX + valueTextSizeMax + 2 + prevPointX, originY + prevPointY );

            // Print the horizontal line
            for( j = 0; j < ( (int)pointX - (int)prevPointX ); j++ ){

                //parent -> channel -> print( "\u2500" );
                //parent -> channel -> print( "*" );
                parent -> channel -> print( "\u2022" );

            }

            // Print vertical line everywhere except the last data point.
            if( i < ( dataSize - 1 ) ){
                
                // Check if we have to go down.
                if( (int)pointY > (int)prevPointY ){

                    for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){
                        parent -> channel -> print( "\033[B\033[D" );
                        parent -> channel -> print( "\u2022" );
                    }

                }

                // Check if we have to go Up.
                if( (int)prevPointY > (int)pointY ){

                    for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){
                        parent -> channel -> print( "\033[A\033[D" );
                        parent -> channel -> print( "\u2022" );
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
            //prevPointX = i - 1;

            parent -> setCursorPosition( valueTextSizeMax + 3 + i - 1, prevPointY );

            // If the previous point have the same value as the current one, we just print it.
            /*
            if( (int)prevPointY == (int)pointY ){

                //parent -> channel -> print( "\u2022" );

            }
            

            // If the previous point is greater, than the current one, we have to draw upwards.
            else if( (int)pointY < (int)prevPointY  ){

                //bufferedPrinter.printf( "\033[%d;%dH\u256F", (int)prevPointY, (int)( yTextSize + 3 + i - 1 ) );

            }

            */

            parent -> channel -> print( "\u2022" );

            // Print vertical line everywhere except the last data point.
            if( i < ( dataSize - 1 ) ){
                
                // Check if we have to go down.
                if( (int)pointY > (int)prevPointY ){

                    for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){
                        parent -> channel -> print( "\033[B\033[D" );
                        parent -> channel -> print( "\u2022" );
                    }

                }

                // Check if we have to go Up.
                if( (int)prevPointY > (int)pointY ){

                    for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){
                        parent -> channel -> print( "\033[A\033[D" );
                        parent -> channel -> print( "\u2022" );
                    }

                }

            }

        }

    }

    parent -> channel -> print( "\033[C\033[C" );
    parent -> channel -> print( valueTextBuffer );
  
}

float ShellminatorPlot::lerp( float v0, float v1, float t ){
  return( 1.0 - t ) * v0 + t * v1;
}

float ShellminatorPlot::mapFloat( float x, float inStart, float inStop, float outStart, float outStop ){

  return outStart + ( outStop - outStart ) * ( ( x - inStart ) / ( inStop - inStart ) );

}