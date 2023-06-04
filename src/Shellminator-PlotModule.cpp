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

bool ShellminatorPlot::begin(){

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.allocate( SHELLMINATOR_PLOT_BUFF_SIZE );
  if( bufferedPrinter.getBufferSize() != SHELLMINATOR_PLOT_BUFF_SIZE ){

    return false;

  }

  #endif

  return true;

}

ShellminatorPlot::ShellminatorPlot( Shellminator* shell_p, float* y, int y_size, int color ){

  shell = shell_p;
  yDataF[ 0 ] = y;
  plotColor[ 0 ] = color;
  yDataSize = y_size;
  numberOfPlots = 1;
  bufferedPrinter = ShellminatorBufferedPrinter( shell -> channel );

}

bool ShellminatorPlot::addPlot( float* y, int y_size, int color ){

  if( numberOfPlots >= SHELLMINATOR_NUMBER_OF_PLOTS ){

    return false;

  }

  if( yDataSize != y_size ){

    return false;

  }

  yDataF[ numberOfPlots ] = y;

  if( color == 0 ){

    if( numberOfPlots < 6 ){

      color = colorTable[ numberOfPlots ];

    }

    else{

      color = colorTable[ 5 ];

    }

  }

  plotColor[ numberOfPlots ] = color;
  yDataSize = y_size;
  numberOfPlots++;

  return true;

}

void ShellminatorPlot::draw( bool redraw ){

  int i;
  int j;

  if( shell -> getTerminalSize( &terminalSizeX, &terminalSizeY ) == false ){
    terminalSizeX = 30;
    terminalSizeY = 30;
  }

  // Hide the cursor
  // Shellminator::hideCursor( shell -> channel );

  terminalSizeY -= 2;

  // Clearing the screen area
  if( redraw == false ){

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "\033[2K" );

    for( i = 0; i < terminalSizeY; i++ ){

      bufferedPrinter.printf( "\r\n" );

    }


    #else

    shell -> channel -> print( "\033[2K" );
    for( i = 0; i < terminalSizeY; i++ ){

      shell -> channel -> println();

    }

    #endif


  }

  else{

    #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

    bufferedPrinter.printf( "\033[%d;%dH\033[J", 1, 1 );

    #else

    shell -> setCursorPosition( 1, 1 );
    shell -> channel -> print( "\033[J" );

    #endif

  }

  if( yDataF[ 0 ]!= NULL ){

    min = yDataF[ 0 ][ 0 ];
    max = yDataF[ 0 ][ 0 ];

  }

  else{

    min = yDataI[ 0 ][ 0 ];
    max = yDataI[ 0 ][ 0 ];

  }

  // Calculating min and max values for scale.
  for( i = 0; i < numberOfPlots; i++ ){

    if( yDataF[ i ] != NULL ){

      for( j = 0; j < yDataSize; j++ ){

        if( min > yDataF[ i ][ j ] ){

          min = yDataF[ i ][ j ];

        }

        if( max < yDataF[ i ][ j ] ){

          max = yDataF[ i ][ j ];

        }

      }

    }

    else{

      for( j = 0; j < yDataSize; j++ ){

        if( min > yDataI[ i ][ j ] ){

          min = yDataI[ i ][ j ];

        }

        if( max < yDataI[ i ][ j ] ){

          max = yDataI[ i ][ j ];

        }

      }

    }

  }

  // To round down
  min = ( (int)min ) - 1;

  // To round up
  max = ( (int)max ) + 1;

  // Draw the scale
  drawScale();


  for( i = 0; i <numberOfPlots; i++ ){

    drawPlot( i );

  }

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.printf( "\033[%d;%dm\033[%d;%dH", (int)Shellminator::REGULAR, (int)Shellminator::WHITE, terminalSizeY + 1, 1 );
  bufferedPrinter.flush();

  #else

  Shellminator::setTerminalCharacterColor( shell -> channel, Shellminator::REGULAR, Shellminator::WHITE );

  shell -> setCursorPosition( 1, terminalSizeY + 1 );

  #endif

}

void ShellminatorPlot::drawScale(){

  int i;
  int j;
  char yTextBuffer[ 15 ];
  float tmp;
  float tmpPos;

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.printf( "\033[%d;%dH", 1, 1 );

  #else

  shell -> setCursorPosition( 1, 1 );

  #endif

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

  // Calculate the widht of the largest value in characters.
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

}

void ShellminatorPlot::drawPlot( uint8_t index ){

  int i;
  int j;
  int terminalWidth;

  int avgStartIndex;
  int avgEndIndex;

  float prevPointX;
  float prevPointY;

  float avg;

  float pointY;
  float pointX;

  #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

  bufferedPrinter.printf( "\033[%d;%dm", (int)Shellminator::REGULAR, plotColor[ index ] );
  bufferedPrinter.flush();

  #else

  Shellminator::setTerminalCharacterColor( shell -> channel, Shellminator::REGULAR, plotColor[ index ] );

  #endif

  terminalWidth = terminalSizeX - ( yTextSize + 2 ) - ( resultTextSize + 3 );

  // We have less data points than 'pixels' in the terminal
  if( ( yDataSize / terminalWidth ) == 0 ){

    for( i = 1; i < yDataSize; i++ ){

      prevPointY = mapFloat( yDataF[ index ][ i - 1 ], min, max, terminalSizeY, 1 );
      prevPointX = mapFloat( i - 1, 0, ( yDataSize - 1 ), 0, terminalWidth );

      pointY = mapFloat( yDataF[ index ][ i ], min, max, terminalSizeY, 1 );
      pointX = mapFloat( i, 0, ( yDataSize - 1 ), 0, terminalWidth );

      #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

      bufferedPrinter.printf( "\033[%d;%dH", (int)prevPointY, (int)( yTextSize + 3 + prevPointX ) );

      #else

      shell -> setCursorPosition( yTextSize + 3 + prevPointX, prevPointY );

      #endif

      // Print the horizontal line
      for( j = 0; j < ( (int)pointX - (int)prevPointX ); j++ ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\u2500" );

        #else

        shell -> channel -> print( "\u2500" );

        #endif

      }

    }

    // Print vertical lines with endings.
    for( i = 1; i < yDataSize; i++ ){

      prevPointY = mapFloat( yDataF[ index ][ i - 1 ], min, max, terminalSizeY, 1 );
      prevPointX = mapFloat( i - 1, 0, ( yDataSize - 1 ), 0, terminalWidth );

      pointY = mapFloat( yDataF[ index ][ i ], min, max, terminalSizeY, 1 );
      pointX = mapFloat( i, 0, ( yDataSize - 1 ), 0, terminalWidth );

      // Print the vertical line.
      // Prev point is smaller than the current.
      if( (int)pointY < (int)prevPointY ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[%d;%dH\u256D", (int)pointY, (int)( yTextSize + 3 + pointX ) );

        #else

        shell -> setCursorPosition( yTextSize + 3 + pointX, pointY );
        shell -> channel -> print( "\u256D" );

        #endif

        for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          //bufferedPrinter.printf( "\033[%d;%dH\u2502", (int)( pointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );
          bufferedPrinter.printf( "\033[B\033[D\u2502", (int)( pointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );

          #else

          //shell -> setCursorPosition( yTextSize + 3 + pointX, pointY + j + 1 );
          //shell -> channel -> print( "\u2502" );
          shell -> channel -> print( "\033[B\033[D\u2502" );

          #endif

        }

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        //bufferedPrinter.printf( "\033[%d;%dH\u256F", (int)( pointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );
        bufferedPrinter.printf( "\033[B\033[D\u256F", (int)( pointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );

        #else

        //shell -> setCursorPosition( yTextSize + 3 + pointX, pointY + j + 1 );
        //shell -> channel -> print( "\u256F" );
        shell -> channel -> print( "\033[B\033[D\u256F" );

        #endif

      }

      // Prev point is larger than the current.
      if( (int)pointY > (int)prevPointY ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[%d;%dH\u256E", (int)prevPointY, (int)( yTextSize + 3 + pointX ) );

        #else

        shell -> setCursorPosition( yTextSize + 3 + pointX, prevPointY );
        shell -> channel -> print( "\u256E" );

        #endif

        for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          //bufferedPrinter.printf( "\033[%d;%dH\u2502", (int)( prevPointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );
          bufferedPrinter.printf( "\033[B\033[D\u2502" );

          #else

          //shell -> setCursorPosition( yTextSize + 3 + pointX, prevPointY + j + 1 );
          //shell -> channel -> print( "\u2502" );
          shell -> channel -> print( "\033[B\033[D\u2502" );

          #endif

        }

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        //bufferedPrinter.printf( "\033[%d;%dH\u2570", (int)( prevPointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );
        bufferedPrinter.printf( "\033[B\033[D\u2570" );

        #else

        //shell -> setCursorPosition( yTextSize + 3 + pointX, prevPointY + j + 1 );
        //shell -> channel -> print( "\u2570" );
        shell -> channel -> print( "\033[B\033[D\u2570" );

        #endif

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

      // Finding the indexes for the laft and right boundaries
      avgStartIndex = mapFloat( i - 1, 0, terminalWidth, 0, yDataSize );
      avgEndIndex = mapFloat( i + 1, 0, terminalWidth, 0, yDataSize );

      // Check for buffer overflow
      if( avgStartIndex < 0 ){
        avgStartIndex = 0;
      }

      if( avgEndIndex >= yDataSize ){
        avgEndIndex = yDataSize - 1;
      }

      // Averaging the numbers in the specified range
      avg = 0.0;
      for( j = avgStartIndex; j < avgEndIndex; j++ ){
        avg += yDataF[ index ][ j ];
      }

      if( ( avgEndIndex - avgStartIndex ) != 0 ){
        avg /= (float)( avgEndIndex - avgStartIndex );
      }

      pointY = mapFloat( avg, min, max, terminalSizeY, 1 );
      pointX = i;

      // Do the same for the previous point
      // Finding the indexes for the laft and right boundaries
      avgStartIndex = mapFloat( i - 2, 0, terminalWidth, 0, yDataSize );
      avgEndIndex = mapFloat( i, 0, terminalWidth, 0, yDataSize );

      // Check for buffer overflow
      if( avgStartIndex < 0 ){
        avgStartIndex = 0;
      }

      if( avgEndIndex >= yDataSize ){
        avgEndIndex = yDataSize - 1;
      }

      // Averaging the numbers in the specified range
      avg = 0.0;
      for( j = avgStartIndex; j < avgEndIndex; j++ ){
        avg += yDataF[ index ][ j ];
      }

      if( ( avgEndIndex - avgStartIndex ) != 0 ){
        avg /= (float)( avgEndIndex - avgStartIndex );
      }

      prevPointY = mapFloat( avg, min, max, terminalSizeY, 1 );
      //prevPointX = i - 1;

      #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

      bufferedPrinter.printf( "\033[%d;%dH", (int)prevPointY, (int)( yTextSize + 3 + i - 1 ) );

      #else

      shell -> setCursorPosition( yTextSize + 3 + i - 1, prevPointY );

      #endif

      if( (int)prevPointY == (int)pointY ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\u2500" );

        #else

        shell -> channel -> print( "\u2500" );

        #endif

      }

      else if( (int)pointY < (int)prevPointY  ){

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[%d;%dH\u256F", (int)prevPointY, (int)( yTextSize + 3 + i - 1 ) );

        #else

        shell -> setCursorPosition( yTextSize + 3 + i - 1, prevPointY );
        shell -> channel -> print( "\u256F" );

        #endif

        for( j = 0; j < ( (int)prevPointY - (int)pointY ) - 1; j++ ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          //bufferedPrinter.printf( "\033[%d;%dH\u2502", (int)( prevPointY + j + 1 ), (int)( yTextSize + 3 + pointX ) );
          bufferedPrinter.printf( "\033[A\033[D\u2502" );

          #else

          shell -> channel -> print( "\033[A\033[D\u2502" );

          #endif

        }

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[A\033[D\u256D" );

        #else

        shell -> channel -> print( "\033[A\033[D\u256D" );

        #endif

      }

      else{

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[%d;%dH\u256E", (int)prevPointY, (int)( yTextSize + 3 + i - 1 ) );

        #else

        shell -> setCursorPosition( yTextSize + 3 + i - 1, prevPointY );
        shell -> channel -> print( "\u256E" );

        #endif

        for( j = 0; j < ( (int)pointY - (int)prevPointY ) - 1; j++ ){

          #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

          bufferedPrinter.printf( "\033[B\033[D\u2502" );

          #else

          shell -> channel -> print( "\033[B\033[D\u2502" );

          #endif

        }

        #ifdef SHELLMINATOR_ENABLE_HIGH_MEMORY_USAGE

        bufferedPrinter.printf( "\033[B\033[D\u2570" );

        #else

        shell -> channel -> print( "\033[B\033[D\u2570" );

        #endif

      }


    }



  }
  
}

// https://en.wikipedia.org/wiki/Linear_interpolation
float ShellminatorPlot::lerp( float v0, float v1, float t ){
  return( 1.0 - t ) * v0 + t * v1;
}

float ShellminatorPlot::mapFloat( float x, float inStart, float inStop, float outStart, float outStop ){

  return outStart + ( outStop - outStart ) * ( ( x - inStart ) / ( inStop - inStart ) );

}