/*
 * Created on July 07 2024
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified July 07 2024
*/

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

#include "Shellminator-Helpers.hpp"

int indexOf( const char* data, const char x ){
    int i;
    int size;

    size = strlen( data );
    for( i = 0; i < size; i++ ){
        if( data[ i ] == x ){
            return i;
        }
    }
    return -1;
}

bool startsWith( const char* original, const char* key ){
    int originalSize;
    int keySize;
    int size;

    originalSize = strlen( original );
    keySize = strlen( key );
    size = keySize;

    if( keySize > originalSize ){
        return false;
    }

    while( *key ){
        if( *key != *original ){
            return false;
        }
        key++;
        original++;
    }

    return true;
}

void shiftStringLeft( char* str ){
  int size;
  int i;

  size = strlen( str );

  if( size < 2 ){
    return;
  }

  for( i = 0; i < size; i++ ){
    str[ i ] = str[ i + 1 ];
  }
}

int strcicmp( const char* p1, const char* p2 ){
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;

    unsigned char c1;
    unsigned char c2;

    do{
        c1 = (unsigned char)tolower( *s1 );
        c2 = (unsigned char)tolower( *s2 );

        s1++;
        s2++;

        if( c1 == '\0' ){
            return c2 - c1;
        }

    } while( c1 == c2 );

    return c2 - c1;
}

void tailEnd( char* str ){
    int i;
    int size;

    size = strlen( str );

    while( ( str[ size - 1 ] == ' ' ) || ( str[ size - 1 ] == '\t' ) ){
        str[ size - 1 ] = '\0';
        size = strlen( str );
        if( size <= 0 ){
            return;
        }
    }
}


