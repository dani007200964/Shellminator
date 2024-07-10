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
#include "Shellminator-Crypto.hpp"


bool ShellminatorSHA1( uint8_t *result, const uint8_t *data, int dataSize ){
    // https://en.wikipedia.org/wiki/SHA-1
    // https://github.com/CTrabant/teeny-sha1
    uint32_t W[ 80 ];

    uint32_t h[] = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f = 0;
    uint32_t k = 0;

    uint32_t idx;
    uint32_t lidx;
    uint32_t widx;
    uint32_t didx = 0;

    int32_t wcount;
    uint32_t temp;
    uint64_t databits = ( (uint64_t) dataSize ) * 8; // Referred as m1 in the original algorythm.
    uint32_t numOfIterations = ( dataSize + 8 ) / 64 + 1;
    uint32_t tailbytes = 64 * numOfIterations - dataSize;
    uint8_t datatail[ 128 ] = {0};

    if( !result ){
        return false;
    }

    if( !data ){
        return false;
    }

    /* Pre-processing of data tail (includes padding to fill out 512-bit chunk):
        Add bit '1' to end of message (big-endian)
        Add 64-bit message length in bits at very end (big-endian) */
    datatail[ 0] = 0x80;
    datatail[ tailbytes - 8 ] = (uint8_t) (databits >> 56 & 0xFF);
    datatail[ tailbytes - 7 ] = (uint8_t) (databits >> 48 & 0xFF);
    datatail[ tailbytes - 6 ] = (uint8_t) (databits >> 40 & 0xFF);
    datatail[ tailbytes - 5 ] = (uint8_t) (databits >> 32 & 0xFF);
    datatail[ tailbytes - 4 ] = (uint8_t) (databits >> 24 & 0xFF);
    datatail[ tailbytes - 3 ] = (uint8_t) (databits >> 16 & 0xFF);
    datatail[ tailbytes - 2 ] = (uint8_t) (databits >> 8 & 0xFF);
    datatail[ tailbytes - 1 ] = (uint8_t) (databits >> 0 & 0xFF);

    /* Process each 512-bit chunk */
    for( lidx = 0; lidx < numOfIterations; lidx++ ){
        /* Compute all elements in W */
        memset( W, 0, 80 * sizeof (uint32_t) );

        /* Break 512-bit chunk into sixteen 32-bit, big endian words */
        for( widx = 0; widx <= 15; widx++ ){
            wcount = 24;

            /* Copy byte-per byte from specified buffer */
            while( didx < dataSize && wcount >= 0 ){
                W[widx] += (((uint32_t)data[didx]) << wcount);
                didx++;
                wcount -= 8;
            }
            /* Fill out W with padding as needed */
            while( wcount >= 0 ){
                W[widx] += (((uint32_t)datatail[didx - dataSize]) << wcount);
                didx++;
                wcount -= 8;
            }
        }

        /* Extend the sixteen 32-bit words into eighty 32-bit words, with potential optimization from:
        "Improving the Performance of the Secure Hash Algorithm (SHA-1)" by Max Locktyukhin */
        for( widx = 16; widx <= 31; widx++ ){
            W[widx] = SHELLMINATOR_SHA1_ROTATE_LEFT ((W[widx - 3] ^ W[widx - 8] ^ W[widx - 14] ^ W[widx - 16]), 1);
        }
        for( widx = 32; widx <= 79; widx++ ){
            W[widx] = SHELLMINATOR_SHA1_ROTATE_LEFT ((W[widx - 6] ^ W[widx - 16] ^ W[widx - 28] ^ W[widx - 32]), 2);
        }

        /* Main loop */
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];

        for( idx = 0; idx <= 79; idx++ ){
        if( idx <= 19 ){
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if( idx >= 20 && idx <= 39 ){
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if( idx >= 40 && idx <= 59 ){
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else if( idx >= 60 && idx <= 79 ){
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        temp = SHELLMINATOR_SHA1_ROTATE_LEFT (a, 5) + f + e + k + W[ idx ];
        e = d;
        d = c;
        c = SHELLMINATOR_SHA1_ROTATE_LEFT (b, 30);
        b = a;
        a = temp;
        }

        h[ 0 ] += a;
        h[ 1 ] += b;
        h[ 2 ] += c;
        h[ 3 ] += d;
        h[ 4 ] += e;

    }

    /* Store binary digest in supplied buffer */
    for( idx = 0; idx < 5; idx++ ){
        result[ idx * 4 + 0 ] = (uint8_t) ( h[ idx ] >> 24);
        result[ idx * 4 + 1 ] = (uint8_t) ( h[ idx ] >> 16);
        result[ idx * 4 + 2 ] = (uint8_t) ( h[ idx ] >> 8);
        result[ idx * 4 + 3 ] = (uint8_t) ( h[ idx ] );
    }

    return true;
}

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

// Base 64 table
const char B64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

// |               State 0                 |               State 1                 |               State 2                 |               State 0                 |
// | 07 . 06 . 05 . 04 . 03 . 02 . 01 . 00 | 07 . 06 . 05 . 04 . 03 . 02 . 01 . 00 | 07 . 06 . 05 . 04 . 03 . 02 . 01 . 00 | 07 . 06 . 05 . 04 . 03 . 02 . 01 . 00 |
// | 05 . 04 . 03 . 02 . 01 . 00 | 05 . 04 . 03 . 02 . 01 . 00 | 05 . 04 . 03 . 02 . 01 . 00 | 05 . 04 . 03 . 02 . 01 . 00 | 05 . 04 . 03 . 02 . 01 . 00 | 05 . 04 |
int ShellminatorBase64( const uint8_t *data, int dataSize, char *result, int resultSize ){
    // https://en.wikipedia.org/wiki/Base64

	int i;
	int bitState;
	int resultCounter;
	uint8_t c;
	uint8_t prevData;
    uint8_t tableIndex;

    int calculatedResultSize;
    int resultSize10;

    // Calculate required result buffer size.
    resultSize10 = (int) dataSize * 10 * 4 / 3;
    calculatedResultSize = (int) resultSize10 / 10;
    if( ( resultSize10 % 10 ) > 0 ){
        calculatedResultSize++;
    }
    if( ( dataSize % 3 ) == 1 ){
        calculatedResultSize += 2;
    }
    else if( ( dataSize % 3 ) == 2 ){
        calculatedResultSize++;
    }

    // One extra character is required for string terminator.
    calculatedResultSize++;

    // Check if the result buffer is large enough for the result.
    if( resultSize < calculatedResultSize ){
        return 0;
    }


	bitState = 0;
	prevData = 0;
    resultCounter = 0;

	for( i = 0; i < dataSize; i++ ) {
		c = data[ i ];

		switch( bitState ) {
            case 0:
                // In state 0, we are interested in the upper 6 bits of the input data byte.
                // We have to shot it down to the lower 6 bits in the result.
                tableIndex = ( c >> 2 ) & 0b00111111;
                result[ resultCounter ] = B64_table[ tableIndex ];
                resultCounter++;
                bitState++;                
                break;

            case 1:
                // In state 1, we are interested in the lower 2 bits of the previous input data byte and
                // this will be the upper two bits of the result. The lower 4 bits of the result will be
                // the upper 6 bits of the input data.
                tableIndex = ( ( prevData & 0b00000011 ) << 4 ) | ( ( c >> 4 ) & 0b00001111 );
                result[ resultCounter ] = B64_table[ tableIndex ];
                resultCounter++;
                bitState++;
                break;

            case 2:
                // This is a tricky state, because we can process two output bytes at the same time.
                // Firstly, we interested in the lower 4 bits of the previouts input data byte and
                // this will be the upper 4 bits of the result. The lower two bits of the result
                // will be the upper two bits of the current input data byte.
                result[ resultCounter ] = B64_table[ ( ( prevData & 0b00001111 ) << 2 ) | ( ( c >> 6 ) & 0b00000011 ) ];
                resultCounter++;

                // Finally, the next output byte will be simply the lower 6 bits of the current data byte.
                result[ resultCounter ] = B64_table[ c & 0b00111111 ];
                resultCounter++;
                
                // Start over
                bitState = 0;
                break;

		}

		prevData = c;

	}

    // Finally, we have to process the leftover bits that left from the previous data byte.
    // State 0 should not be present at the end.
	switch( bitState ) {
        case 1:
            result[ resultCounter ] = B64_table[ ( prevData & 0b00000011 ) << 4 ];
            resultCounter++;

            // In this state, two padding characters are needed.
            result[ resultCounter ] = SHELLMINATOR_BASE64_PAD;
            resultCounter++;

            result[ resultCounter ] = SHELLMINATOR_BASE64_PAD;
            resultCounter++;
            break;

        case 2:
            result[ resultCounter ] = B64_table[( prevData & 0xF ) << 2];
            resultCounter++;


            // In this state, one padding character is needed.
            result[ resultCounter ] = SHELLMINATOR_BASE64_PAD;
            resultCounter++;
            break;
	}

    // Terminate the result string.
	result[ resultCounter ] = '\0';

	return resultCounter;
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
        if( resultComputed[ i ] != passwordHash[ i ] ){
            return false;
        }
    }

    return true;

}
