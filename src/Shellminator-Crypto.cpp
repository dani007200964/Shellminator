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

#include "Shellminator.hpp"

// Original source can be found here:
// https://web.archive.org/web/20190108202303/http://www.hackersdelight.org/hdcodetxt/crc.c.txt
// I had to modify the original version a bit, to make it compatible
// with the system.
void crc32b( uint8_t *message, int messageSize, uint8_t *result, int resultSize ) {
    int i;
    int j;
    uint32_t byte;
    uint32_t crc;
    uint32_t mask;
    uint8_t* crcPointer = (uint8_t*)&crc;

    if( resultSize != sizeof( crc ) ){
        return;
    }

    i = 0;
    crc = 0xFFFFFFFF;
    for( i = 0; i < messageSize; i++ ){
        byte = message[ i ];            // Get next byte.
        crc = crc ^ byte;
        for( j = 7; j >= 0; j-- ) {    // Do eight times.
            mask = -( crc & 1 );
            crc = ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
        }
    }

    crc = ~crc;

    for( i = 0; i < sizeof( crc ); i++ ){
        result[ i ] = crcPointer[ sizeof( crc ) - i - 1 ];
    }

}

void Shellminator::setPassword( uint8_t* hashData, int hashSize ){
    passwordHash = hashData;
    passwordHashSize = hashSize;
    passwordHashFunc = crc32b;
}

void Shellminator::setPasswordHashFunction( void(*hashFunc_p)( uint8_t*, int, uint8_t*, int ) ){
    passwordHashFunc = hashFunc_p;
}

bool Shellminator::checkPassword( const char* password ){

    int passwordSize;
    int i;

    if( passwordHashSize < 1 ){
        return false;
    }

    if( passwordHash == NULL ){
        return false;
    }

    uint8_t resultComputed[ passwordHashSize ];

    passwordSize = strlen( password );

    passwordHashFunc( (uint8_t*)password, passwordSize, resultComputed, passwordHashSize );

    for( i = 0; i < passwordHashSize; i++ ){
        //channel -> print( resultComputed[ i ], HEX );
        //channel -> print( ", " );
        if( resultComputed[ i ] != passwordHash[ i ] ){
            return false;
        }
    }
    //channel -> println();

    return true;

}
