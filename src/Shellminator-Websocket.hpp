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
#ifndef SHELLMINATOR_WEBSOCKET_HPP_
#define SHELLMINATOR_WEBSOCKET_HPP_

    #include "Stream.h"
    #include "Shellminator-DefaultSettings.hpp"
    #include "Shellminator-Helpers.hpp"
    #include "Shellminator-Crypto.hpp"
    #include <string.h>

    // Todo add hooks to connection and disconnection events!

    #ifdef ARDUINO
        #include "Arduino.h"
    #endif

    #ifdef SHELLMINATOR_USE_WIFI_CLIENT

        // Macro magic for macro overloading.
        #define SHELLMINATOR_WS_DBG_OVERLOAD( _1, _2, NAME, ... ) NAME

        #define SHELLMINATOR_WS_DBG_ENABLE

        #ifdef SHELLMINATOR_WS_DBG_ENABLE
            #define SHELLMINATOR_WS_DBG_1(   x ) if( dbg ){ dbg -> print( x );   }
            #define SHELLMINATOR_WS_DBGLN_1( x ) if( dbg ){ dbg -> println( x ); }

            #define SHELLMINATOR_WS_DBG_2(   x, y ) if( dbg ){ dbg -> print( x, y );   }
            #define SHELLMINATOR_WS_DBGLN_2( x, y ) if( dbg ){ dbg -> println( x, y ); }
        #elif
            #define SHELLMINATOR_WS_DBG_1(   x )
            #define SHELLMINATOR_WS_DBGLN_1( x )

            #define SHELLMINATOR_WS_DBG_2(   x, y )
            #define SHELLMINATOR_WS_DBGLN_2( x, y )
        #endif

        /// Macro to print debug message.
        ///
        /// The syntax is the same as a regular Stream.print() function.
        /// It will check if a debug channel is attached and only then
        /// will print the debug message.
        #define SHELLMINATOR_WS_DBG( ... )   SHELLMINATOR_WS_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_WS_DBG_2,   SHELLMINATOR_WS_DBG_1   )( __VA_ARGS__ )
        #define SHELLMINATOR_WS_DBGLN( ... ) SHELLMINATOR_WS_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_WS_DBGLN_2, SHELLMINATOR_WS_DBGLN_1 )( __VA_ARGS__ )

        /// Macro to print debug message with new line.
        ///
        /// The syntax is the same as a regular Stream.println() function.
        /// It will check if a debug channel is attached and only then
        /// will print the debug message.
        #define WS_FRAME_HEADER_SIZE 6

        // We need to add 258EAFA5-E914-47DA-95CA-C5AB0DC85B11 to the end of the
        // received key and this needs space in the buffer.
        #define SHELLMINATOR_WS_CLIENT_KEY_SIZE         50 + 37

        // The Server key is generated from the result of an SH1 hash, which is 20
        // bytes long. The Base64 encoded key size is 28 characters, but we need a
        // termination character as well, so the minimum of required buffer size is
        // 29 characters.
        #define SHELLMINATOR_WS_SERVER_KEY_SIZE         30 

        class ShellminatorWebSocket : public Stream{

        public:

            typedef enum{
                WS_DISCONNECTED_STATE,
                WS_HEADER_STATE,
                WS_CONNECTED_STATE
            } wsState_t;

            typedef enum{
                WS_COMM = 0x00,
                WS_TXT = 0x01,
                WS_CLOSE = 0x08,
                WS_PING = 0x09,
                WS_PONG = 0x0A
            } wsDecodedHeaderType_t;

            ShellminatorWebSocket();
            ShellminatorWebSocket( int port_p );

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
            /// @returns The number of bytes that has been successfully written to the channel. Because it is the base class, it returns 0.
            size_t write( uint8_t b ) override;

            size_t write(const uint8_t *data, size_t size) override;

            void begin();
            void update();
            void attachDebugChannel( Stream* dbg_p );

        private:
            int port = 443;
            bool clientConnected = false;
            Stream* dbg = NULL;

            WiFiServer* server;
            WiFiClient client;

            uint8_t streamBuffer[ SHELLMINATOR_WS_STREAM_BUFFER_SIZE ];
            uint32_t streamBufferWritePointer;
            uint32_t streamBufferReadPointer;

            char clientBuffer[ SHELLMINATOR_WS_CLIENT_BUFFER_SIZE + 1 ];
            int clientBufferCounter = 0;

            char clientLineNameBuffer[ SHELLMINATOR_WS_CLIENT_LINE_NAME_SIZE ];
            char clientLineValueBuffer[ SHELLMINATOR_WS_CLIENT_LINE_VALUE_SIZE ];
            char clientKey[ SHELLMINATOR_WS_CLIENT_KEY_SIZE ];
            char serverKey[ SHELLMINATOR_WS_SERVER_KEY_SIZE ];

            static const char* serverHeader;

            void resetVariables();
            void resetDataVariables();
            void wsHeaderProcessing( char newChar );
            void wsDataProcessing( char newChar );

            void closeClient( bool sendCloseFrame = false );

            bool generateServerKey();
            void finishDecoding();
            bool sendFrame( wsDecodedHeaderType_t type, const uint8_t* data = NULL, uint32_t dataSize = 0 );
            bool sendFrame125( wsDecodedHeaderType_t type, const uint8_t* data = NULL, uint8_t dataSize = 0 );

            void appendToCircularBuffer( uint8_t data );
            void appendToCircularBuffer( uint8_t* data, int dataSize );

            int indexOf( const char* data, const char x );
            bool startsWith( const char* original, const char* key );
            void shiftStringLeft( char* str );
            int strcicmp( const char* p1, const char* p2 );
            void tailEnd( char* str );

            bool httpGetLineFound;
            bool connectionUpgradeLineFound;
            bool upgradeWebsocketLineFound;
            int clientVersion;

            bool decodedHdrFin;
            wsDecodedHeaderType_t decodedHdrType;
            bool decodedHdrMask;
            uint8_t decodedHdrPayloadLen;
            uint8_t decodedHdrMaskKeys[ 4 ];
            uint8_t decodedHdrPayloadCntr;
            uint8_t decodedDataXorCntr;

            wsState_t wsState;


        };

    #endif

#endif