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
#ifndef SHELLMINATOR_TCPSOCKET_HPP_
#define SHELLMINATOR_TCPSOCKET_HPP_

    // Todo add hooks to connection and disconnection events!

    #include "Stream.h"
    #include "Shellminator-DefaultSettings.hpp"
    #include "Shellminator-Helpers.hpp"
    #include <string.h>

    #ifdef ARDUINO
        #include "Arduino.h"
    #endif

    #ifdef SHELLMINATOR_USE_WIFI_CLIENT

        #define SHELLMINATOR_TCP_DBG_OVERLOAD( _1, _2, NAME, ... ) NAME

        #define SHELLMINATOR_TCP_DBG_ENABLE

        #ifdef SHELLMINATOR_TCP_DBG_ENABLE
            #define SHELLMINATOR_TCP_DBG_1(   x ) if( dbg ){ dbg -> print( x );   }
            #define SHELLMINATOR_TCP_DBGLN_1( x ) if( dbg ){ dbg -> println( x ); }

            #define SHELLMINATOR_TCP_DBG_2(   x, y ) if( dbg ){ dbg -> print( x, y );   }
            #define SHELLMINATOR_TCP_DBGLN_2( x, y ) if( dbg ){ dbg -> println( x, y ); }
        #elif
            #define SHELLMINATOR_TCP_DBG_1(   x )
            #define SHELLMINATOR_TCP_DBGLN_1( x )

            #define SHELLMINATOR_TCP_DBG_2(   x, y )
            #define SHELLMINATOR_TCP_DBGLN_2( x, y )
        #endif

        #define SHELLMINATOR_TCP_DBG( ... )   SHELLMINATOR_TCP_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_TCP_DBG_2,   SHELLMINATOR_TCP_DBG_1   )( __VA_ARGS__ )
        #define SHELLMINATOR_TCP_DBGLN( ... ) SHELLMINATOR_TCP_DBG_OVERLOAD( __VA_ARGS__, SHELLMINATOR_TCP_DBGLN_2, SHELLMINATOR_TCP_DBGLN_1 )( __VA_ARGS__ )

        #define SHELLMINATOR_TCP_STREAM_BUFFER_SIZE      1000

        class ShellminatorTcpSocket : public Stream{

        public:

            typedef enum{
                TELNET_FIRST_BYTE_STATE,
                TELNET_COMMAND_BYTE_STATE,
                TELNET_OPTION_BYTE_STATE
            } TelnetNegotiationState_t;

            ShellminatorTcpSocket();
            ShellminatorTcpSocket( int port_p );

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

            void attachConnectCallback( void(*connectCallback_p)(ShellminatorTcpSocket* ) );
            void attachDisconnectCallback( void(*disconnectCallback_p)(ShellminatorTcpSocket* ) );

            void begin();
            void update();
            void attachDebugChannel( Stream* dbg_p );

        private:
            int port = 23;
            bool clientConnected = false;
            Stream* dbg = NULL;

            TelnetNegotiationState_t negotiationState;

            WiFiServer* server;
            WiFiClient client;

            uint8_t streamBuffer[ SHELLMINATOR_WS_STREAM_BUFFER_SIZE ];
            uint32_t streamBufferWritePointer;
            uint32_t streamBufferReadPointer;

            void resetVariables();
            void closeClient();

            void appendToCircularBuffer( uint8_t data );
            void appendToCircularBuffer( uint8_t* data, int dataSize );

            // https://www.omnisecu.com/tcpip/telnet-commands-and-options.php
            static const uint8_t TELNET_IAC_DONT_LINEMODE[3];
            static const uint8_t TELNET_IAC_WILL_ECHO[3];
            static const uint8_t TELNET_IAC_DONT_ECHO[3];
            static const uint8_t TELNET_IAC_WILL_SUPPRESS_GO_AHEAD[3];
            static const uint8_t TELNET_IAC_DO_SUPPRESS_GO_AHEAD[3];

            void(*connectCallback)(ShellminatorTcpSocket* parent) = NULL;
            void(*disconnectCallback)(ShellminatorTcpSocket* parent) = NULL;

        };

    #endif

#endif