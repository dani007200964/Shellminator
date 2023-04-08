#include "stdioStream.hpp"

/*
void stdioStream::push( uint8_t data ){

	buffer[ writePointer ] = data;
	writePointer++;
	if( writePointer >= STDIO_STREAM_BUFFER_SIZE ){
		writePointer = 0;
	}

}

void stdioStream::push( uint8_t* data, size_t size ){

	uint32_t i;

	for( i = 0; i < size; i++ ){

		buffer[ writePointer ] = data[ i ];
		writePointer++;
		if( writePointer >= SHELLMINATOR_WEBSOCKET_BUFFER_LEN ){
			writePointer = 0;
		}

	}

}
*/

int stdioStream::available(){

	if( writePointer == readPointer ){
		return 0;
	}

	else if( writePointer > readPointer ){
		return writePointer - readPointer;
	}

	else{

		return STDIO_STREAM_BUFFER_SIZE - readPointer + writePointer;

	}

}

int stdioStream::read(){

	int ret;

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		ret = (uint8_t)buffer[ readPointer ];
		readPointer++;

		if( readPointer >= STDIO_STREAM_BUFFER_SIZE ){
			readPointer = 0;
		}

	}

	return ret;

}

int stdioStream::peek(){

	if( writePointer == readPointer ){

		return -1;

	}

	else{

		return (uint8_t)buffer[ readPointer ];

	}

}

void stdioStream::flush(){

	// Todo Maybe clear the input buffer?

}

size_t stdioStream::write( uint8_t b ){

    printf( "%c", (char)b );
	return 1;

}

size_t stdioStream::write( const uint8_t *buff, size_t size ){

    uint32_t i;
    for( i = 0; i < size; i++ ){

        printf( "%c", (char)buff[ i ] );

    }
	return 1;

}

//---- print section ----//

size_t stdioStream::print( char c ){

    printf( "%c", (char)c );
	return 1;

}

size_t stdioStream::print( uint8_t b ){

	printf( "%u", (int)b );
	return 1;

}

size_t stdioStream::print( char *str ){

	uint32_t dataSize;

	dataSize = strlen( (const char*)str );
    printf( "%s", (const char*)str );

	return dataSize;

}

size_t stdioStream::print( const char *str ){

	uint32_t dataSize;

	dataSize = strlen( str );
    printf( "%s", str );

	return dataSize;

}