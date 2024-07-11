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
#include "Shellminator-WebSocket.hpp"

#ifdef SHELLMINATOR_USE_WIFI_CLIENT

const char* ShellminatorWebSocket::serverHeader =
  "HTTP/1.1 101 Switching Protocols\r\n"
  "Server: Shellminator\r\n"
  "Upgrade: websocket\r\n"
  "Connection: Upgrade\r\n"
  "Sec-WebSocket-Version: 13\r\n"
  "Sec-WebSocket-Accept: "
;

ShellminatorWebSocket::ShellminatorWebSocket( int port_p ){
    port = port_p;
    server = new WiFiServer( port );
    dbg = NULL;
    resetVariables();
}

ShellminatorWebSocket::ShellminatorWebSocket(){
    port = 443;
    server = new WiFiServer( port );
    dbg = NULL;
    resetVariables();
}

void ShellminatorWebSocket::attachDebugChannel( Stream* dbg_p ){
    dbg = dbg_p;
}

void ShellminatorWebSocket::begin(){
    // todo check network module firmware on Uno R4 and Pico W
    if( server == NULL ){
        return;
    }
    server -> begin();
    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS Server started." ) );
}

void ShellminatorWebSocket::appendToCircularBuffer( uint8_t data ){
    streamBuffer[ streamBufferWritePointer ] = data;

    // Increment the write pointer and handle wrapping.
    streamBufferWritePointer++;
    if( streamBufferWritePointer >= SHELLMINATOR_WS_STREAM_BUFFER_SIZE ){
        streamBufferWritePointer = 0;
    }

    // Detect buffer overflow
    if( streamBufferWritePointer == streamBufferReadPointer ){
        SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS Circular Buffer Overflow!" ) );
        closeClient();
    }

}

void ShellminatorWebSocket::appendToCircularBuffer( uint8_t* data, int dataSize ){
    int i;
    for( i = 0; i < dataSize; i++ ){
        appendToCircularBuffer( data[ i ] );
    }
}

bool ShellminatorWebSocket::sendFrame( ShellminatorWebSocket::wsDecodedHeaderType_t type, const uint8_t* data, uint32_t dataSize ){

    int i;
    bool ret;

    if( data == NULL ){
        return sendFrame125( type, data, dataSize );
    }

    for( i = 0; i < (int)( dataSize / 125 ); i++ ){
        ret = sendFrame125( type, data, 125 );
        if( !ret ){
            return false;
        }
        data += 125;
    }

    return sendFrame125( type, data, dataSize % 125 );
}

bool ShellminatorWebSocket::sendFrame125( ShellminatorWebSocket::wsDecodedHeaderType_t type, const uint8_t* data, uint8_t dataSize ){
    int i;
    uint8_t maskBuffer[ 4 ] = { 0 };
    uint8_t buffer[ WS_FRAME_HEADER_SIZE ] = { 0 };

    uint8_t headerSize = 2;

    if( dataSize > 125 ){
        return false;
    }

    // Byte 0
    buffer[ 0 ] = 0;
    buffer[ 0 ] |= 1UL << 7; // To set the Fin bit.
    buffer[ 0 ] |= type;

    // Byte 1
    buffer[ 1 ] = 0x00;
    buffer[ 1 ] |= dataSize;

    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- Header To Send ----" ) );
    for( i = 0; i < headerSize; i++ ){
        SHELLMINATOR_WS_DBG( i );
        SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t0x" ) );
        SHELLMINATOR_WS_DBG( buffer[ i ], HEX );
        SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t" ) );
        SHELLMINATOR_WS_DBGLN( (int)buffer[ i ] );
    }
    client.write( (const uint8_t*)buffer, headerSize );

    if( dataSize > 0 ){
        SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- Data To Send ----" ) );
        for( i = 0; i < dataSize; i++ ){
            SHELLMINATOR_WS_DBG( i );
            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t0x" ) );
            SHELLMINATOR_WS_DBG( data[ i ], HEX );
            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t" ) );
            SHELLMINATOR_WS_DBGLN( (int)data[ i ] );
        }
        client.write( (const uint8_t*)data, dataSize );
    }
    return true;
}

void ShellminatorWebSocket::update(){
    char newChar;
    WiFiClient newClient;

    if( server == NULL ){
        return;
    }

    newClient = server -> accept();

    if( newClient ){
        if( CLIENT_STATE ){
            // Reject a new client, because another one is alredy in use.
            newClient.stop();
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS Client already in use. Rejecting new connection." ) );
        }
        else{
            // New connection.
            client = newClient;
            #ifndef ARDUINO_UNOWIFIR4
                client.setNoDelay( true );
            #endif
            clientConnected = true;
            wsState = WS_HEADER_STATE;
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "New WS Client." ) );
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- Processing Client Header ----" ) );     
        }
    }

    // Check for disconnection event
    if( clientConnected && !CLIENT_STATE ){
        SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "Disconnect Event Detected!" ) );
        closeClient();
    }

    // Process webscoket data.    
    if( clientConnected && CLIENT_STATE ){

        while( client.available() ){
            newChar = client.read();
            if( wsState == WS_HEADER_STATE ){
                wsHeaderProcessing( newChar );
            }
            else if( wsState == WS_CONNECTED_STATE ){
                wsDataProcessing( newChar );
            }
        }

    }

}

void ShellminatorWebSocket::resetVariables(){
    clientBuffer[ 0 ] = '\0';
    clientBufferCounter = 0;
    wsState = WS_DISCONNECTED_STATE;

    httpGetLineFound = false;
    connectionUpgradeLineFound = false;
    upgradeWebsocketLineFound = false;
    clientKey[ 0 ] = '\0';
    clientVersion = 0;

    memset( streamBuffer, 0, SHELLMINATOR_WS_STREAM_BUFFER_SIZE );
    streamBufferWritePointer = 0;
    streamBufferReadPointer = 0;
}

void ShellminatorWebSocket::resetDataVariables(){
    clientBuffer[ 0 ] = '\0';
    clientBufferCounter = 0;

    decodedHdrFin = false;
    decodedHdrType = WS_CLOSE;
    decodedHdrMask = false;
    decodedHdrPayloadLen = 0;
    decodedHdrMaskKeys[ 0 ] = 0x00;
    decodedHdrMaskKeys[ 1 ] = 0x00;
    decodedHdrMaskKeys[ 2 ] = 0x00;
    decodedHdrMaskKeys[ 3 ] = 0x00;
    decodedHdrPayloadCntr = 0;
    decodedDataXorCntr = 0;
}

void ShellminatorWebSocket::finishDecoding(){
    // Todo Put all the data to a circular buffer and bind it with Stream functions.

    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- Data Reception Finished ----" ) );

    switch( decodedHdrType ){
        case WS_PONG:
            // Todo handle Pong stuff.
            break;
        case WS_PING:
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "Ping Event -> Sending Pong..." ) );
            sendFrame( WS_PONG );
            break;
        case WS_TXT:
            // The text handling is implemented before this function.
            break;
        case WS_CLOSE:
            closeClient( true );
            break;
        default:
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "Unhandled type" ) );
            break;
    }

    resetDataVariables();
}

void ShellminatorWebSocket::wsDataProcessing( char newChar ){

    uint8_t currentPayload;

    // Firstly we need the first two bytes of the Websocket frame.
    // This will determinate the size of the frame. This implementation
    // only supports maximum frame size of 125 characters( bytes, not
    // unicode characters ) in order to fit in small memory.
    if( clientBufferCounter < 2 ){
        clientBuffer[ clientBufferCounter ] = newChar;
        clientBufferCounter++;

        // Event when the second byte arrives.
        if( clientBufferCounter == 2 ){
            // Decode Byte 0
            decodedHdrFin = ( clientBuffer[ 0 ] >> 7 ) & 0x01;
            decodedHdrType = (wsDecodedHeaderType_t)( clientBuffer[ 0 ] & 0x0F );

            // Decode Byte 1
            decodedHdrMask = ( clientBuffer[ 1 ] >> 7 ) & 0x01;
            decodedHdrPayloadLen = ( clientBuffer[ 1 ] & 0x7F );

            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- Data Header Summary ----" ) );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\tFin:\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrFin );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\tType:\t" ) );
            switch( decodedHdrType ){
                case WS_COMM:
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "COMMUNICATION" ) );
                    break;
                case WS_TXT:
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "TEXT" ) );
                    break;
                case WS_CLOSE:
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "CLOSE" ) );
                    break;
                case WS_PING:
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "PING" ) );
                    break;
                case WS_PONG:
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "PONG" ) );
                    break;
                default:
                    SHELLMINATOR_WS_DBG( __CONST_TXT__( "UNKNOWN CODE: " ) );
                    SHELLMINATOR_WS_DBGLN( (int)decodedHdrType );
                    break;
            }

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\tMask:\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrMask );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\tLen:\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrPayloadLen );

            // The client MUST send mask and mask keys,
            // if not, we must close the connection.
            if( !decodedHdrMask ){
                closeClient();
            }

            // decodedHdrPayloadCntr is downcounting from decodedHdrPayloadLen
            decodedHdrPayloadCntr = decodedHdrPayloadLen;

        }
    }

    // After the first two bytes, the four mask key bytes are coming.
    else if( decodedHdrMask && ( clientBufferCounter < 6 ) ){
        clientBuffer[ clientBufferCounter ] = newChar;
        clientBufferCounter++;

        // Event when the sixth byte arrives.
        if( clientBufferCounter == 6 ){
            decodedHdrMaskKeys[ 0 ] = clientBuffer[ 2 ];
            decodedHdrMaskKeys[ 1 ] = clientBuffer[ 3 ];
            decodedHdrMaskKeys[ 2 ] = clientBuffer[ 4 ];
            decodedHdrMaskKeys[ 3 ] = clientBuffer[ 5 ];

            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "\tMask Keys:" ) );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrMaskKeys[ 0 ], HEX );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrMaskKeys[ 1 ], HEX );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrMaskKeys[ 2 ], HEX );

            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );
            SHELLMINATOR_WS_DBGLN( decodedHdrMaskKeys[ 3 ], HEX );

            decodedDataXorCntr = 0;

            if( decodedHdrPayloadLen == 0 ){
                finishDecoding();
            }
            else{
                SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "\tDecoded characters:" ) );
            }

        }

    }

    else if( decodedHdrPayloadCntr > 0 ){

        if( decodedHdrMask ){
            currentPayload = newChar ^ decodedHdrMaskKeys[ decodedDataXorCntr % 4 ];
            decodedDataXorCntr++;
        }
        else{
            currentPayload = newChar;
        }

        SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );
        SHELLMINATOR_WS_DBG( (char)currentPayload );
        SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t" ) );
        SHELLMINATOR_WS_DBG( currentPayload, HEX );
        SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "\tMASKED" ) );

        appendToCircularBuffer( currentPayload );

        decodedHdrPayloadCntr--;
        if( decodedHdrPayloadCntr == 0 ){
            finishDecoding();
        }

    }

}

void ShellminatorWebSocket::wsHeaderProcessing( char newChar ){
    int indexOfSeparator;
    bool valid;


    if( newChar == '\n' ){
        clientBuffer[ clientBufferCounter ] = '\0';
        clientBufferCounter = 0;
        indexOfSeparator = indexOf( clientBuffer, ':' );

        // Detect the end of the header. It consist of an empty line.
        if( clientBuffer[ 0 ] == '\0' ){
            // If we found the end of the header, we have to check if all
            // parameters are present and if they are valid.

            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS Client header received." ) );

            valid = true;
            valid &= connectionUpgradeLineFound;
            valid &= upgradeWebsocketLineFound;
            valid &= clientVersion == 13;
            if( valid ){
                SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS Client Header Valid." ) );
                if( !generateServerKey() ){
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "Server key generation failed!" ) );
                    closeClient();
                    return;
                }

                SHELLMINATOR_WS_DBG( __CONST_TXT__( "WS Server key generated: " ) );
                SHELLMINATOR_WS_DBGLN( serverKey );

                client.print( serverHeader );
                client.println( serverKey );

                // We do not send any Sec-WebSocket-Protocol, becouse this
                // implementation does not support any subprotocols.

                client.println();

                client.flush();

                // Switch state.
                wsState = WS_CONNECTED_STATE;

                // Reset Buffers for the new state.
                resetDataVariables();

                SHELLMINATOR_WS_DBG( __CONST_TXT__( "WS Connection Established!" ) );

            }
            else{
                closeClient();
            }

        }

        else if( startsWith( clientBuffer, "GET " ) ){
            SHELLMINATOR_WS_DBG( clientBuffer );
            SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );

            if( strstr( clientBuffer, "HTTP/1.1" ) != NULL ){
                httpGetLineFound = true;
            }

            if( httpGetLineFound ){
                SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ OK ]" ) );
            }
            else{
                SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ NOK ]" ) );
            }
        }

        else if( indexOfSeparator > 0 ){
            // Extract the name of the parameter.
            strncpy( clientLineNameBuffer, clientBuffer, SHELLMINATOR_WS_CLIENT_LINE_NAME_SIZE );

            // Check if the index of the separator fits in the buffer area.
            // If not, stop the processing.
            if( indexOfSeparator > SHELLMINATOR_WS_CLIENT_LINE_NAME_SIZE ){
                return;
            }
            // Terminate the string where the separator was.
            clientLineNameBuffer[ indexOfSeparator ] = '\0';

            // Extract the value of the parameter.
            strncpy( clientLineValueBuffer, clientBuffer + indexOfSeparator + 1, SHELLMINATOR_WS_CLIENT_LINE_VALUE_SIZE );
            // Just in case, terminate the end.
            clientLineValueBuffer[ SHELLMINATOR_WS_CLIENT_LINE_VALUE_SIZE - 1 ] = '\0';

            // Remove any whitespace from the beginning of the value - RFC2616
            while( ( clientLineValueBuffer[ 0 ] == ' ' ) || ( clientLineValueBuffer[ 0 ] == '\t' ) ){
                shiftStringLeft( clientLineValueBuffer );
            }

            tailEnd( clientLineValueBuffer );

            // Check for 'Connection: Upgrade' line.
            if( strcicmp( clientLineNameBuffer, "Connection" ) == 0 ){
                SHELLMINATOR_WS_DBG( clientBuffer );
                SHELLMINATOR_WS_DBG( __CONST_TXT__( "\t\t" ) );

                if( strcicmp( clientLineValueBuffer, "upgrade" ) == 0 ){
                    connectionUpgradeLineFound = true;
                }

                if( connectionUpgradeLineFound ){
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ OK ]" ) );
                }
                else{
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ NOK ]" ) );
                }
            }

            // Check for 'Upgrade: websocket' line.
            else if( strcicmp( clientLineNameBuffer, "Upgrade" ) == 0 ){
                SHELLMINATOR_WS_DBG( clientBuffer );
                SHELLMINATOR_WS_DBG( "\t\t" );

                if( strcicmp( clientLineValueBuffer, "websocket" ) == 0 ){
                    upgradeWebsocketLineFound = true;
                }

                if( upgradeWebsocketLineFound ){
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ OK ]" ) );
                }
                else{
                    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "[ NOK ]" ) );
                }
            }

            // Check for 'Sec-WebSocket-Key: XXXXXXXXXXXXXXXXXXXXXXXXX' line.
            else if( strcicmp( clientLineNameBuffer, "Sec-WebSocket-Key" ) == 0 ){
                SHELLMINATOR_WS_DBGLN( clientBuffer );
                strncpy( clientKey, clientLineValueBuffer, SHELLMINATOR_WS_CLIENT_KEY_SIZE );
            }

            // Check for 'Sec-WebSocket-Version: X' line.
            else if( strcicmp( clientLineNameBuffer, "Sec-WebSocket-Version" ) == 0 ){
                SHELLMINATOR_WS_DBGLN( clientBuffer );
                sscanf( clientLineValueBuffer, "%d", &clientVersion );
            }

            else{
                SHELLMINATOR_WS_DBG( __CONST_TXT__( "Undhandled parameter: " ) );
                SHELLMINATOR_WS_DBGLN( clientBuffer );
            }

            // There are other fields and I do not know if theye are
            // necessary in this implementation or not:
            // - Sec-WebSocket-Extensions
            // - Sec-WebSocket-Protocol
            // - Authorization

        }
        else{
            SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "WS parameter separator not found" ) );
        }

    }
    else if( newChar == '\r' ){
        return;
    }
    else{
        clientBuffer[ clientBufferCounter ] = newChar;
        clientBufferCounter++;
        if( clientBufferCounter >= SHELLMINATOR_WS_CLIENT_BUFFER_SIZE ){
            clientBufferCounter = SHELLMINATOR_WS_CLIENT_BUFFER_SIZE;
        }
    }

}

int ShellminatorWebSocket::indexOf( const char* data, const char x ){
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

bool ShellminatorWebSocket::startsWith( const char* original, const char* key ){
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

void ShellminatorWebSocket::shiftStringLeft( char* str ){
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

int ShellminatorWebSocket::strcicmp( const char* p1, const char* p2 ){
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

void ShellminatorWebSocket::tailEnd( char* str ){
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

bool ShellminatorWebSocket::generateServerKey(){
    int clientKeySize;
    int i;

    uint8_t sha1HashBytes[ 20 ];

    clientKeySize = strlen( clientKey );
    strncpy( clientKey + clientKeySize, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", SHELLMINATOR_WS_CLIENT_KEY_SIZE );
    clientKey[ SHELLMINATOR_WS_CLIENT_KEY_SIZE - 1 ] = '\0';

    if( !ShellminatorSHA1( sha1HashBytes, (uint8_t*)clientKey, strlen( clientKey ) ) ){
        return false;
    }

    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "---- SHA1 Hash Bytes ----" ) );
    for( i = 0; i < sizeof( sha1HashBytes ); i++ ){
        SHELLMINATOR_WS_DBG( sha1HashBytes[ i ], HEX );
    }
    SHELLMINATOR_WS_DBGLN( ' ' );

    if( 0 == ShellminatorBase64( sha1HashBytes, sizeof( sha1HashBytes ), serverKey, SHELLMINATOR_WS_SERVER_KEY_SIZE ) ){
        return false;
    }

    return true;

}

void ShellminatorWebSocket::closeClient( bool sendCloseFrame ){
    if( sendCloseFrame ){
        sendFrame( WS_CLOSE );
    }
    client.flush();
    delay( 100 );
    while( client.available() ){
        client.read();
    }
    client.stop();
    clientConnected = false;
    resetVariables();
    SHELLMINATOR_WS_DBGLN( __CONST_TXT__( "Disconnecting WS Client!" ) );

}

int ShellminatorWebSocket::available(){
    if( !clientConnected ){
        return 0;
    }
    if( streamBufferWritePointer == streamBufferReadPointer ){
        return 0;
    }
    else if( streamBufferWritePointer > streamBufferReadPointer ){
        return streamBufferWritePointer - streamBufferReadPointer;
    }
    return SHELLMINATOR_WS_STREAM_BUFFER_SIZE - streamBufferReadPointer + streamBufferWritePointer;
}

int ShellminatorWebSocket::read(){
    int ret;
    if( !clientConnected ){
        return -1;
    }
    if( streamBufferWritePointer == streamBufferReadPointer ){
        return -1;
    }
    ret = (uint8_t) streamBuffer[ streamBufferReadPointer ];
    streamBufferReadPointer++;

    if( streamBufferReadPointer >= SHELLMINATOR_WS_STREAM_BUFFER_SIZE ){
        streamBufferReadPointer = 0;
    }

    return ret;
}

int ShellminatorWebSocket::peek(){
    if( !clientConnected ){
        return -1;
    }
    if( streamBufferWritePointer == streamBufferReadPointer ){
        return -1;
    }
    return (uint8_t) streamBuffer[ streamBufferReadPointer ];
}

void ShellminatorWebSocket::flush(){
	// Honestly I don't know what to do.
	// Arduino flush methods are weird.
}

size_t ShellminatorWebSocket::write( uint8_t b ){
    if( !clientConnected ){
        return 0;
    }
    if( sendFrame( WS_TXT, &b, 1 ) ){
        return 1;
    }
    return 0;
}

size_t ShellminatorWebSocket::write(const uint8_t *data, size_t size){
    if( !clientConnected ){
        return 0;
    }
    if( sendFrame( WS_TXT, data, size ) ){
        return size;
    }
    return 0;
}

#endif