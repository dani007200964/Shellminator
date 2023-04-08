#ifndef STDIO_STREAM_HPP__
#define STDIO_STREAM_HPP__

#include "Stream.h"
#include <conio.h>
#include <stdint.h>

#define STDIO_STREAM_BUFFER_SIZE 1024

class stdioStream : public Stream{

private:

  void push( uint8_t data );
  void push( uint8_t* data, size_t size );

  char buffer[ STDIO_STREAM_BUFFER_SIZE ];
	uint32_t readPointer = 0;
	uint32_t writePointer = 0;

public:

  void update();

  /// Available bytes in the channel.
  ///
  /// @returns The available bytes in the channel.
  int    available() override;

  /// Read one byte form the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will be removed from the channel.
  int    read() override;

  /// Peek the firtst byte from the channel.
  ///
  /// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel.
  int    peek() override;

  /// Flush the channel.
  void   flush() override;

  /// Write one byte to the channel.
  ///
  /// @param b The value that has to be written to the channel.
  /// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
  size_t write( uint8_t b ) override;

  size_t write( const uint8_t *buff, size_t size ) override;

  /// Print one character to the channel.
  ///
  /// @param c The character that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( char c );

  /// Print one byte to the channel.
  ///
  /// @param b The value that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( uint8_t b );

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( char *str );

  /// Print c-string to the channel.
  ///
  /// @param str The string that has to be printed to the channel.
  /// @returns The number of bytes that has been sucessfully printed to the channel. Because it is the base class, it returns 0.
  size_t print( const char *str );

};


#endif