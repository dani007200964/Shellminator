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
#include "Shellminator-TcpSocket.hpp"

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

    const uint8_t ShellminatorTcpSocket::TELNET_IAC_DONT_LINEMODE[]          = { 255, 254, 34 };
    const uint8_t ShellminatorTcpSocket::TELNET_IAC_WILL_ECHO[]              = { 255, 251, 1 };
    const uint8_t ShellminatorTcpSocket::TELNET_IAC_DONT_ECHO[]              = { 255, 254, 1 };
    const uint8_t ShellminatorTcpSocket::TELNET_IAC_WILL_SUPRESS_GO_AHEAD[]  = { 255, 251, 3 };
    const uint8_t ShellminatorTcpSocket::TELNET_IAC_DO_SUPRESS_GO_AHEAD[]    = { 255, 253, 3 };

    ShellminatorTcpSocket::ShellminatorTcpSocket( int port_p ){
        port = port_p;
        server = new WiFiServer( port );
        dbg = NULL;
        resetVariables();
    }

    ShellminatorTcpSocket::ShellminatorTcpSocket(){
        port = 23;
        server = new WiFiServer( port );
        dbg = NULL;
        resetVariables();
    }

    void ShellminatorTcpSocket::attachDebugChannel( Stream* dbg_p ){
        dbg = dbg_p;
    }

    void ShellminatorTcpSocket::begin(){
        server -> begin();
    }

    void ShellminatorTcpSocket::appendToCircularBuffer( uint8_t data ){
        streamBuffer[ streamBufferWritePointer ] = data;

        // Increment the write pointer and handle wrapping.
        streamBufferWritePointer++;
        if( streamBufferWritePointer >= SHELLMINATOR_TCP_STREAM_BUFFER_SIZE ){
            streamBufferWritePointer = 0;
        }

        // Detect buffer overflow
        if( streamBufferWritePointer == streamBufferReadPointer ){
            SHELLMINATOR_TCP_DBGLN( "TCP Circular Buffer Overflow!" );
            closeClient();
        }

    }

    void ShellminatorTcpSocket::appendToCircularBuffer( uint8_t* data, int dataSize ){
        int i;
        for( i = 0; i < dataSize; i++ ){
            appendToCircularBuffer( data[ i ] );
        }
    }

    void ShellminatorTcpSocket::update(){
        char newChar;
        WiFiClient newClient = server -> accept();

        if( newClient ){
            if( CLIENT_STATE ){
                // Reject a new client, because another one is alredy in use.
                newClient.stop();
                SHELLMINATOR_TCP_DBGLN( "TCP Client already in use. Rejecting new connection." );
            }
            else{
                // New connection.
                client = newClient;
                #ifndef ARDUINO_UNOWIFIR4
                    client.setNoDelay( true );
                #endif
                clientConnected = true;
                SHELLMINATOR_TCP_DBGLN( "New TCP Client." );
                SHELLMINATOR_TCP_DBGLN( "---- Sending Telnet Configuration ----" );     

                client.write( TELNET_IAC_DONT_LINEMODE, 3 );
                client.write( TELNET_IAC_WILL_ECHO, 3 );
                client.write( TELNET_IAC_DONT_ECHO, 3 );
                client.write( TELNET_IAC_WILL_SUPRESS_GO_AHEAD, 3 );
                client.write( TELNET_IAC_DO_SUPRESS_GO_AHEAD, 3 );
            }
        }

        // Check for disconnection event
        if( clientConnected && !CLIENT_STATE ){
            SHELLMINATOR_TCP_DBGLN( "TCP Disconnect Event Detected!" );
            closeClient();
        }

        // Process webscoket data.    
        if( clientConnected && CLIENT_STATE ){

            while( client.available() ){
                newChar = client.read();

                switch( negotiationState ){
                    case TELNET_FIRST_BYTE_STATE:
                        // Check if we found an ongoing Telnet command
                        if( newChar == 0xFF ){
                            // If so, we have to change state. Each Telnet
                            // command starts with 0xFF and after this,
                            // two bytes follow.
                            negotiationState = TELNET_COMMAND_BYTE_STATE;
                        }
                        else{
                            appendToCircularBuffer( newChar );
                        }
                        break;
                    case TELNET_COMMAND_BYTE_STATE:
                        // The current implementation does not care about the
                        // Telnet commands. But to not throw any garbage characters
                        // in front of the user, we read the command and option
                        // bytes to remove them from the buffer.
                        negotiationState = TELNET_OPTION_BYTE_STATE;
                        break;
                    case TELNET_OPTION_BYTE_STATE:
                        // See the comment above.
                        negotiationState = TELNET_FIRST_BYTE_STATE;
                        break;
                    default:
                        // We should not be here!
                        closeClient();
                        break;
                }

            }

        }

    }

    void ShellminatorTcpSocket::closeClient(){
        client.flush();
        delay( 100 );
        while( client.available() ){
            client.read();
        }
        client.stop();
        clientConnected = false;
        resetVariables();
        SHELLMINATOR_TCP_DBGLN( "Disconnecting TCP Client!" );
    }

    void ShellminatorTcpSocket::resetVariables(){
        memset( streamBuffer, 0, SHELLMINATOR_TCP_STREAM_BUFFER_SIZE );
        streamBufferWritePointer = 0;
        streamBufferReadPointer = 0;
        negotiationState = TELNET_FIRST_BYTE_STATE;
    }


    int ShellminatorTcpSocket::available(){
        if( !clientConnected ){
            return 0;
        }
        if( streamBufferWritePointer == streamBufferReadPointer ){
            return 0;
        }
        else if( streamBufferWritePointer > streamBufferReadPointer ){
            return streamBufferWritePointer - streamBufferReadPointer;
        }
        return SHELLMINATOR_TCP_STREAM_BUFFER_SIZE - streamBufferReadPointer + streamBufferWritePointer;
    }

    int ShellminatorTcpSocket::read(){
        int ret;
        if( !clientConnected ){
            return -1;
        }
        if( streamBufferWritePointer == streamBufferReadPointer ){
            return -1;
        }
        ret = (uint8_t) streamBuffer[ streamBufferReadPointer ];
        streamBufferReadPointer++;

        if( streamBufferReadPointer >= SHELLMINATOR_TCP_STREAM_BUFFER_SIZE ){
            streamBufferReadPointer = 0;
        }

        return ret;
    }

    int ShellminatorTcpSocket::peek(){
        if( !clientConnected ){
            return -1;
        }
        if( streamBufferWritePointer == streamBufferReadPointer ){
            return -1;
        }
        return (uint8_t) streamBuffer[ streamBufferReadPointer ];
    }

    void ShellminatorTcpSocket::flush(){
        // Hinestly I don't know what to do.
        // Arduino flush methods are wierd.
    }

    size_t ShellminatorTcpSocket::write( uint8_t b ){
        if( !clientConnected ){
            return 0;
        }
        return client.write( b );
    }

    size_t ShellminatorTcpSocket::write(const uint8_t *data, size_t size){
        if( !clientConnected ){
            return 0;
        }
        return client.write( data, size );
    }

#endif